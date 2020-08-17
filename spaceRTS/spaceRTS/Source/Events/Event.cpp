#include "MEpch.h"
#include "Event.h"
#include "Application.h"
namespace MYENGINE
{
    Event::Event()
        : eventMainPlayer(Application::Get().GetPlayer())
    {
    }

    Event::Event(const Player & p)
        : eventMainPlayer(p)
    {
    }

    void Event::SerializeBase(std::string& buffer) const
    {
        char buff[sizeof(short)];

        short player= (short)eventMainPlayer.playerNum;
        memcpy(buff, &player, sizeof(short));

        buffer = buff;
        buffer.resize(2);
        //std::replace(buffer.begin(), buffer.end(), '\0', '0');
        //memcpy(&buffer[0], &player, size);

    }
}
