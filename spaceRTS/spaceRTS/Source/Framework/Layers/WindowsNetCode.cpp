#include "MEpch.h"

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "WindowsNetCode.h"
#include "Core.h"

#pragma comment(lib, "ws2_32.lib") 

#define maxMsgSize 36
#define singleHeaderSize sizeof(MYENGINE::EventType)

namespace MYENGINE
{
    //floating point rounding is probably not deterministic but it should be ok because not all events will be max size events
    const unsigned short SOCKET_BUFFER_SIZE = 1024;
    const unsigned short HEADERSIZE  = (short)((SOCKET_BUFFER_SIZE - sizeof(short)) / (maxMsgSize + singleHeaderSize)) * singleHeaderSize + sizeof(short);
    const unsigned short MESSAGESIZE = (short)((SOCKET_BUFFER_SIZE - sizeof(short)) / (maxMsgSize + singleHeaderSize)) * maxMsgSize;

    WindowsNetCode::WindowsNetCode()
    {
        m_localAddress.sin_family = AF_INET;
        m_localAddress.sin_port = htons(PORT);
        m_localAddress.sin_addr.s_addr = INADDR_ANY;

        AppSocket as = CreateSocket();
        if (as.sock != INVALID_SOCKET)
        {
            ME_CORE_ERROR("socket create failed!! undefined behavior may occure!!!");
            m_appSockets.push_back(as);
            m_pendingAppSocket = as;
        }
    }
    WindowsNetCode::~WindowsNetCode()
    {
        for (AppSocket as : m_appSockets)
        {
            int result = closesocket(as.sock);
            if (result == SOCKET_ERROR)
            {
                ME_CORE_ERROR("closesocket failed: %d", WSAGetLastError());
                WSACleanup();
            }
        } 
        WSACleanup();
    }

    WindowsNetCode::AppSocket WindowsNetCode::CreateSocket()
    {
        AppSocket as;
        WORD winsock_version = 0x202;
        if (WSAStartup(winsock_version, &as.winsock_data))
        {
            ME_CORE_ERROR("WSAStartup failed: %d", WSAGetLastError());
            return as;
        }

        int address_family = AF_INET;//ipv4 af_inet6 is ipv6
        int type = SOCK_STREAM; //SOCK_DGRAM;
        as.protocol = IPPROTO_TCP; //IPPROTO_UDP;
        as.sock = socket(address_family, type, as.protocol);

        if (as.sock == INVALID_SOCKET)
        {
            ME_CORE_ERROR("socket failed: %d", WSAGetLastError());
            return as;
        }

        struct timeval tv;
        tv.tv_sec = 10;//mil sec
        tv.tv_usec = 0;
        int optOn = 1;

        if (setsockopt(as.sock, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof(tv)) < 0 ||
            setsockopt(as.sock, SOL_SOCKET, SO_REUSEADDR, /*| SO_REUSEPORT*/(const char *)&optOn, sizeof(optOn)) < 0 ||//SO_REUSEPORT not one windows
            setsockopt(as.sock, SOL_SOCKET, TCP_NODELAY, (const char *)&optOn, sizeof(optOn)) < 0)
        {
            ME_CORE_ERROR("Error setting socket options");
        }
        
        if (bind(as.sock, (SOCKADDR*)&m_localAddress, sizeof(m_localAddress)) == SOCKET_ERROR)
        {
            as.sock = INVALID_SOCKET;
            ME_CORE_ERROR("socket bind failed: %d", WSAGetLastError());
            return as;
        }

        if (as.protocol == IPPROTO_TCP)
        {
            if (listen(as.sock, 10) < 0)//size of connection queue
            {
                ME_CORE_ERROR("socket listen failed: %d", WSAGetLastError());
            }
        }

       return as;
    }

    bool WindowsNetCode::CheckConnection(const SOCKET& s)
    {
        struct timeval tv;
        tv.tv_sec = 10;//mil sec
        tv.tv_usec = 0;

        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(s, &readfds);

        if (select(0, &readfds, NULL, NULL, &tv) > 0)
        {
            int sizeofAddr = sizeof(m_localAddress);
            AppSocket as = m_pendingAppSocket;
            m_appSockets.push_back(as);

            if ((accept(as.sock, (SOCKADDR*)&m_localAddress, &sizeofAddr)) < 0)
            {
                ME_CORE_ERROR("socket accept failed: %d", WSAGetLastError());
                return false;
            }
            return true;
        }
        return false;
    }

    std::map<float, Event*> WindowsNetCode::Recv(PlayerList& playerIDToPlayer)
    {
        char buffer[SOCKET_BUFFER_SIZE];
        int flags = 0;
        SOCKADDR_IN from;
        int from_size = sizeof(from);

        std::map<float, Event*> orderedEvents;
        for (AppSocket as : m_appSockets)
        {
            int bytes_received=1;
            while (bytes_received != 0)
            {
                bytes_received = recvfrom(as.sock, buffer, SOCKET_BUFFER_SIZE, flags, (SOCKADDR*)&from, &from_size);

                if (bytes_received == 0)
                {
                    break;
                }
                else if (bytes_received == SOCKET_ERROR)
                {
                    int err = WSAGetLastError();
                    //if not time out
                    if (err != 10060)
                    {
                        ME_CORE_ERROR("recvfrom returned SOCKET_ERROR, WSAGetLastError() {0}", WSAGetLastError());
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                    ME_CORE_TRACE(" {0}. {1}. {2}. {3}: {4} -  {5}", from.sin_addr.S_un.S_un_b.s_b1, from.sin_addr.S_un.S_un_b.s_b2, from.sin_addr.S_un.S_un_b.s_b3, from.sin_addr.S_un.S_un_b.s_b4, from.sin_port, buffer);

                    unsigned short NumOfEvents = 0;
                    unsigned short headerPos = 0;
                    unsigned short messagePos = 0;

                    memcpy(&NumOfEvents, buffer, sizeof(NumOfEvents)); // "Deserialize"
                    headerPos = sizeof(NumOfEvents);
                    messagePos = NumOfEvents * singleHeaderSize + sizeof(NumOfEvents);

                    for (int i = 0; i < NumOfEvents; i++)
                    {
                        EventType et = EventType::None;
                        memcpy(&et, &buffer[headerPos], sizeof(et)); // "Deserialize"
                        headerPos += sizeof(et);

                        float time = 0.0f;
                        memcpy(&time, &buffer[messagePos], sizeof(time)); // "Deserialize"
                        messagePos += sizeof(time);
                        
                        orderedEvents[time] = GetEvent(et, buffer, messagePos, playerIDToPlayer);
                    }
                }
            }
        }
        return orderedEvents;

    }

    void WindowsNetCode::Send(const std::map<float, Event*>& Events)
    {
        char header[HEADERSIZE];
        char message[MESSAGESIZE];
        char buffer[SOCKET_BUFFER_SIZE];

        unsigned short headerPos = 0;
        unsigned short messagePos = 0;

        short eventsToSend = Events.size();
        memcpy(&header[headerPos], &eventsToSend, sizeof(eventsToSend)); // "Serialize"
        headerPos += sizeof(eventsToSend);

        std::vector<std::string> connections;

        for (std::pair<float, Event*> ep : Events)
        {
            EventType et = ep.second->GetEventType();
            float time = ep.first;

            if (et == EventType::AppConnect)
            {
                AppConnectEvent* connectEventPtr = (AppConnectEvent*) ep.second;
                connections.push_back(connectEventPtr->GetIP());
            }

            memcpy(&header[headerPos], &et, sizeof(et)); // "Serialize"
            headerPos += sizeof(et);

            memcpy(&message[messagePos], &ep.first, sizeof(ep.first)); // "Serialize"
            messagePos += sizeof(ep.first);

            std::string serializedEvent;
            ep.second->Serialize(serializedEvent);
            memcpy(&message[messagePos], serializedEvent.c_str(), serializedEvent.length()); // "Serialize"
            messagePos += serializedEvent.length();
        }
        //std::copy(header, header + headerPos, buffer);
        //std::copy(message, message + messagePos, buffer + headerPos);
        memcpy(&buffer[0], header, sizeof(header)); // no need to divied by size of header[0] becuase it is a char which is one byte
        memcpy(&buffer[headerPos], message, sizeof(message)); // no need to divied by size of message[0] becuase it is a char which is one byte

        int flags = 0;
        for (AppSocket as : m_appSockets)
        {
            for (std::string ip : connections)
            {
                as.dest_addr.sin_addr.S_un.S_addr = inet_addr(ip.c_str());

                if (connect(as.sock, (struct sockaddr *)&as.dest_addr, sizeof(as.dest_addr)) < 0)
                {
                    ME_CORE_ERROR("connect failed: %d", WSAGetLastError());
                }
            }

            if (sendto(as.sock, buffer, headerPos + messagePos, flags, (SOCKADDR*)&as.dest_addr, sizeof(as.dest_addr)) == SOCKET_ERROR)
            {
                ME_CORE_ERROR("sendto failed: %d", WSAGetLastError());
                return;
            }
        }
    }

    const Player WindowsNetCode::DeSerializePlayer(const char* buff, unsigned short& messagePos, const PlayerList& playerIDToPlayer) const
    {
        short player=0;
        memcpy(&player, buff, sizeof(short));
        messagePos += sizeof(short);

        return playerIDToPlayer.currentPlayers[NUM(player)];
        //return Player(Player1, Team1, glm::vec4(1.0,0.0,0.0,1.0));
    }

    Event* WindowsNetCode::GetEvent(const EventType& et, const char* buffer, unsigned short& messagePos, PlayerList& playerIDToPlayer)
    {
        Event* e = nullptr;
        switch (et)
        {
        case EventType::WindowClose:
        {
            Player p = DeSerializePlayer(&buffer[messagePos], messagePos, playerIDToPlayer);

            e = new WindowClosedEvent(p);//owned by in orderedEvents app
            break;
        }
        case EventType::WindowResized:
        {
            unsigned short eventPos = 0;
            float w, h = 0.0f;

            memcpy(&w, &buffer[messagePos], sizeof(float)); // "Deserialize"
            messagePos += sizeof(float);

            memcpy(&h, &buffer[messagePos], sizeof(float)); // "Deserialize"
            messagePos += sizeof(float);

            Player p = DeSerializePlayer(&buffer[messagePos], messagePos, playerIDToPlayer);

            e = new WindowResizeEvent(w, h, p);//owned by in orderedEvents app
            break;
        }
        case EventType::AppChangeColor:
        {
            unsigned short eventPos = 0;
            unsigned short r, g, b = 0;

            memcpy(&r, &buffer[messagePos], sizeof(r)); // "Deserialize"
            messagePos += sizeof(r);

            memcpy(&g, &buffer[messagePos], sizeof(g)); // "Deserialize"
            messagePos += sizeof(g);

            memcpy(&b, &buffer[messagePos], sizeof(b)); // "Deserialize"
            messagePos += sizeof(b);

            Player p = DeSerializePlayer(&buffer[messagePos], messagePos, playerIDToPlayer);
            float a = playerIDToPlayer.currentPlayers[NUM(p.playerNum)].color.a;
            playerIDToPlayer.currentPlayers[NUM(p.playerNum)].color = glm::vec4(r,g,b,a);

            e = new AppChangeColorEvent(r, g, b, p);//owned by in orderedEvents app

            break;
        }
        case EventType::AppChangeTransparency:
        {
            unsigned short eventPos = 0;
            unsigned short t = 0;

            memcpy(&t, &buffer[messagePos], sizeof(t)); // "Deserialize"
            messagePos += sizeof(t);

            Player p = DeSerializePlayer(&buffer[messagePos], messagePos, playerIDToPlayer);

            e = new AppChangeTransparencyEvent(t, p);//owned by in orderedEvents app
            break;
        }
        case EventType::AppChangeTeam:
        {
            unsigned short eventPos = 0;
            unsigned short team = 0;

            memcpy(&team, &buffer[messagePos], sizeof(team)); // "Deserialize"
            messagePos += sizeof(team);

            Player p = DeSerializePlayer(&buffer[messagePos], messagePos, playerIDToPlayer);
            playerIDToPlayer.currentPlayers[NUM(p.playerNum)].team = (Team)team;
            e = new AppChangeTeamEvent(team, p);//owned by in orderedEvents app
            break;
        }
        case EventType::AppConnect:
        {
            unsigned short eventPos = 0;
            std::string ip;
            unsigned short size=0;

            memcpy(&size, &buffer[messagePos], sizeof(size)); // "Deserialize"
            messagePos += sizeof(size);

            ip.resize(size);
            memcpy(&ip[0], &buffer[messagePos], sizeof(size)); // "Deserialize"
            messagePos += sizeof(size);

            Player p = DeSerializePlayer(&buffer[messagePos], messagePos, playerIDToPlayer);

            e = new AppConnectEvent(size, ip, p);//owned by in orderedEvents app
            break;
        }
        case EventType::AppConnectConfirmed:
        {
            unsigned short eventPos = 0;
            unsigned short playerNum = 0;

            memcpy(&playerNum, &buffer[messagePos], sizeof(playerNum)); // "Deserialize"
            messagePos += sizeof(playerNum);

            Player p = DeSerializePlayer(&buffer[messagePos], messagePos, playerIDToPlayer);

            e = new AppConnectConfirmedEvent(playerNum, p);//owned by in orderedEvents app
            break;
        }
        case EventType::AppTick:
        {
            AppTickEvent ate = AppTickEvent();
            memcpy(&ate, &buffer[messagePos], m_eventToSize.at(et)); // "Deserialize"
            messagePos += m_eventToSize.at(et);
            e = &ate;
            break;
        }
        case EventType::AppUpdate:
        {
            AppUpdateEvent aue = AppUpdateEvent();
            memcpy(&aue, &buffer[messagePos], m_eventToSize.at(et)); // "Deserialize"
            messagePos += m_eventToSize.at(et);
            e = &aue;
            break;
        }
        case EventType::AppRender:
        {
            AppRenderEvent are = AppRenderEvent();
            memcpy(&are, &buffer[messagePos], m_eventToSize.at(et)); // "Deserialize"
            messagePos += m_eventToSize.at(et);
            e = &are;
            break;
        }
        case EventType::KeyPressed:
        {
            unsigned short eventPos = 0;
            int keycode, repeatcount = 0;

            memcpy(&keycode, &buffer[messagePos], sizeof(int)); // "Deserialize"
            messagePos += sizeof(int);

            memcpy(&repeatcount, &buffer[messagePos], sizeof(int)); // "Deserialize"
            messagePos += sizeof(int);

            Player p = DeSerializePlayer(&buffer[messagePos], messagePos, playerIDToPlayer);

            e = new KeyPressedEvent(keycode, repeatcount, p);//owned by in orderedEvents app
            break;
        }
        case EventType::KeyReleased:
        {
            unsigned short eventPos = 0;
            int keycode = 0;

            memcpy(&keycode, &buffer[messagePos], sizeof(int)); // "Deserialize"
            messagePos += sizeof(int);

            Player p = DeSerializePlayer(&buffer[messagePos], messagePos, playerIDToPlayer);

            e = new KeyReleasedEvent(keycode, p);//owned by in orderedEvents app
            break;
        }
        case EventType::KeyTyped:
        {
            unsigned short eventPos = 0;
            int keycode = 0;

            memcpy(&keycode, &buffer[messagePos], sizeof(int)); // "Deserialize"
            messagePos += sizeof(int);

            Player p = DeSerializePlayer(&buffer[messagePos], messagePos, playerIDToPlayer);

            e = new KeyTypedEvent(keycode, p);//owned by in orderedEvents app
            break;
        }
        case EventType::MouseButtonPressed:
        {
            unsigned short eventPos = 0;
            int button;
            float x, y = 0.0f;

            memcpy(&button, &buffer[messagePos], sizeof(int)); // "Deserialize"
            messagePos += sizeof(int);

            memcpy(&x, &buffer[messagePos], sizeof(float)); // "Deserialize"
            messagePos += sizeof(float);

            memcpy(&y, &buffer[messagePos], sizeof(float)); // "Deserialize"
            messagePos += sizeof(float);

            Player p = DeSerializePlayer(&buffer[messagePos], messagePos, playerIDToPlayer);

            e = new MouseButtonPressedEvent(button, x, y, p);//owned by in orderedEvents app
            break;
        }
        case EventType::MouseButtonReleased:
        {
            unsigned short eventPos = 0;
            int button;
            float x, y = 0.0f;

            memcpy(&button, &buffer[messagePos], sizeof(int)); // "Deserialize"
            messagePos += sizeof(int);

            memcpy(&x, &buffer[messagePos], sizeof(float)); // "Deserialize"
            messagePos += sizeof(float);

            memcpy(&y, &buffer[messagePos], sizeof(float)); // "Deserialize"
            messagePos += sizeof(float);

            Player p = DeSerializePlayer(&buffer[messagePos], messagePos, playerIDToPlayer);

            e = new MouseButtonReleasedEvent(button, x, y, p);//owned by in orderedEvents app
            break;
        }
        case EventType::MouseMoved:
        {
            unsigned short eventPos = 0;
            float x, y = 0.0f;

            memcpy(&x, &buffer[messagePos], sizeof(float)); // "Deserialize"
            messagePos += sizeof(float);

            memcpy(&y, &buffer[messagePos], sizeof(float)); // "Deserialize"
            messagePos += sizeof(float);

            Player p = DeSerializePlayer(&buffer[messagePos], messagePos, playerIDToPlayer);

            e = new MouseMovedEvent(x, y, p);//owned by in orderedEvents app
            break;
        }
        case EventType::MouseScrolled:
        {
            unsigned short eventPos = 0;
            float x, y = 0.0f;

            memcpy(&x, &buffer[messagePos], sizeof(float)); // "Deserialize"
            messagePos += sizeof(float);

            memcpy(&y, &buffer[messagePos], sizeof(float)); // "Deserialize"
            messagePos += sizeof(float);

            Player p = DeSerializePlayer(&buffer[messagePos], messagePos, playerIDToPlayer);

            e = new MouseScrolledEvent(x, y, p);//owned by in orderedEvents app
            break;
        }
        }

        return e;
    }
}