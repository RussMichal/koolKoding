#pragma once
#include <map>
#include <winsock2.h>
#include "Events/MouseEvent.h"
#include "Events/KeyEvent.h"
#include "Events/ApplicationEvent.h"

namespace MYENGINE
{
    class WindowsNetCode
    {
    public:
        WindowsNetCode();
        ~WindowsNetCode();

        bool CheckConnection(const SOCKET& s);
        std::map<float, Event*> Recv(PlayerList& playerIDToPlayer);
        void Send(const std::map<float, Event*>& Events);
    private:
        struct AppSocket
        {
            int protocol;
            SOCKADDR_IN dest_addr;
            WSADATA winsock_data;
            SOCKET sock;
        };

        const Player DeSerializePlayer(const char* buff, unsigned short& messagePos, const PlayerList& playerIDToPlayer) const;

        Event* GetEvent(const EventType& et, const char* buffer, unsigned short& messagePos, PlayerList& playerIDToPlayer);

        AppSocket CreateSocket();

        const unsigned short PORT = 9999;
        std::vector<AppSocket> m_appSockets;
        SOCKADDR_IN m_localAddress;
        AppSocket m_pendingAppSocket;

        const std::unordered_map<EventType, unsigned int> m_eventToSize = {
            {EventType::None, 0 },
            {EventType::WindowClose,           sizeof(WindowClosedEvent)},
            {EventType::WindowResized,         sizeof(WindowResizeEvent)},
            //{EventType::Window,                sizeof(Window)},
            //{EventType::Focus,                 sizeof(FocusEvent)},
            //{EventType::WindowLostFocus,       sizeof(WindowLostFocusEvent)},
            //{EventType::WindowMoved,           sizeof(WindowMovedEvent)},

            {EventType::AppTick,               sizeof(AppTickEvent)},
            {EventType::AppUpdate,             sizeof(AppUpdateEvent)},
            {EventType::AppRender,             sizeof(AppRenderEvent)},

            {EventType::KeyPressed,            sizeof(KeyPressedEvent)},
            {EventType::KeyReleased,           sizeof(KeyReleasedEvent)},
            {EventType::KeyTyped,              sizeof(KeyTypedEvent)},

            {EventType::MouseButtonPressed,    sizeof(MouseButtonPressedEvent)},
            {EventType::MouseButtonReleased,   sizeof(MouseButtonReleasedEvent)},
            {EventType::MouseMoved,            sizeof(MouseMovedEvent)},
            {EventType::MouseScrolled,         sizeof(MouseScrolledEvent)}
        };
    };
}