#pragma once
#pragma once

#include "Event.h"

#include<sstream>

namespace MYENGINE
{
	class MYENGINE_API WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(const unsigned int width, const unsigned int height)
			: m_width(width)
			, m_height(height) {}

        WindowResizeEvent(const unsigned int width, const unsigned int height, const Player& p)
            : Event(p)
            , m_width(width)
            , m_height(height) {}

		inline int GetWidth() const { return m_width; }
		inline int GetHeight() const { return m_height; }

        void Serialize(std::string& retBuff) const override
        {
            unsigned short eventPos = 0;
            retBuff.resize(2 * sizeof(float) + sizeof(short));

            memcpy(&retBuff[eventPos], &m_width, sizeof(m_width)); // "Serialize"
            eventPos += sizeof(m_width);

            memcpy(&retBuff[eventPos], &m_height, sizeof(m_height)); // "Serialize"
            eventPos += sizeof(m_height);

            std::string baseData;
            SerializeBase(baseData);
            memcpy(&retBuff[eventPos], &baseData[0], baseData.length()); // "Serialize"
        }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResize: " << m_width << ", " << m_height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResized);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);
	private:
		unsigned int m_width;
		unsigned int m_height;
	};

	class MYENGINE_API WindowClosedEvent : public Event
	{
	public:
        WindowClosedEvent()
        {}
        WindowClosedEvent(const Player& p)
            : Event(p)
        {}
		EVENT_CLASS_TYPE(WindowClose);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);
	};

    class MYENGINE_API AppChangeColorEvent : public Event
    {
    public:
        AppChangeColorEvent(const unsigned short r, const unsigned short g, const unsigned short b)
            : m_r(r)
            , m_g(g)
            , m_b(b) {}

        AppChangeColorEvent(const unsigned short r, const unsigned short g, const unsigned short b, const Player& p)
            : Event(p)
            , m_r(r)
            , m_g(g)
            , m_b(b) {}

        inline int GetR() const { return m_r; }
        inline int GetG() const { return m_g; }
        inline int GetB() const { return m_b; }

        void Serialize(std::string& retBuff) const override
        {
            unsigned short eventPos = 0;
            retBuff.resize(3 * sizeof(unsigned short) + sizeof(short));

            memcpy(&retBuff[eventPos], &m_r, sizeof(m_r)); // "Serialize"
            eventPos += sizeof(m_r);

            memcpy(&retBuff[eventPos], &m_g, sizeof(m_g)); // "Serialize"
            eventPos += sizeof(m_g);

            memcpy(&retBuff[eventPos], &m_b, sizeof(m_b)); // "Serialize"
            eventPos += sizeof(m_b);

            std::string baseData;
            SerializeBase(baseData);
            memcpy(&retBuff[eventPos], &baseData[0], baseData.length()); // "Serialize"
        }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "AppChangeColorEvent: " << m_r << ", " << m_g << ", " << m_b;
            return ss.str();
        }

        EVENT_CLASS_TYPE(AppChangeColor);
        EVENT_CLASS_CATEGORY(EventCategoryApplication);
    private:
        unsigned short m_r;
        unsigned short m_g;
        unsigned short m_b;
    };

    class MYENGINE_API AppChangeTransparencyEvent : public Event
    {
    public:
        AppChangeTransparencyEvent(const unsigned short a)
            : m_a(a){}

        AppChangeTransparencyEvent(const unsigned short a, const Player& p)
            : Event(p)
            , m_a(a) {}

        inline int GetA() const { return m_a; }

        void Serialize(std::string& retBuff) const override
        {
            unsigned short eventPos = 0;
            retBuff.resize(sizeof(unsigned short) + sizeof(short));

            memcpy(&retBuff[eventPos], &m_a, sizeof(m_a)); // "Serialize"
            eventPos += sizeof(m_a);

            std::string baseData;
            SerializeBase(baseData);
            memcpy(&retBuff[eventPos], &baseData[0], baseData.length()); // "Serialize"
        }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "AppChangeTransparency: " << m_a;
            return ss.str();
        }

        EVENT_CLASS_TYPE(AppChangeTransparency);
        EVENT_CLASS_CATEGORY(EventCategoryApplication);
    private:
        unsigned short m_a;
    };

    class MYENGINE_API AppChangeTeamEvent : public Event
    {
    public:
        AppChangeTeamEvent(const unsigned short t)
            : m_team(t) {}

        AppChangeTeamEvent(const unsigned short t, const Player& p)
            : Event(p)
            , m_team(t) {}

        inline int GetTeam() const { return m_team; }

        void Serialize(std::string& retBuff) const override
        {
            unsigned short eventPos = 0;
            retBuff.resize(sizeof(unsigned short) + sizeof(short));

            memcpy(&retBuff[eventPos], &m_team, sizeof(m_team)); // "Serialize"
            eventPos += sizeof(m_team);

            std::string baseData;
            SerializeBase(baseData);
            memcpy(&retBuff[eventPos], &baseData[0], baseData.length()); // "Serialize"
        }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "AppChangeTransparency: " << m_team;
            return ss.str();
        }

        EVENT_CLASS_TYPE(AppChangeTeam);
        EVENT_CLASS_CATEGORY(EventCategoryApplication);
    private:
        unsigned short m_team;
    };

    class MYENGINE_API AppConnectEvent : public Event
    {
    public:
        AppConnectEvent(const unsigned short size, const std::string& ip)
            : m_size(size)
            , m_ip(ip){}

        AppConnectEvent(const unsigned short size, const std::string& ip, const Player& p)
            : Event(p)
            , m_size(size)
            , m_ip(ip){}

        inline std::string GetIP() const { return m_ip; }

        void Serialize(std::string& retBuff) const override
        {
            unsigned short eventPos = 0;
            retBuff.resize(sizeof(unsigned short) + sizeof(short));

            memcpy(&retBuff[eventPos], &m_ip, sizeof(m_ip)); // "Serialize"
            eventPos += sizeof(m_ip);

            std::string baseData;
            SerializeBase(baseData);
            memcpy(&retBuff[eventPos], &baseData[0], baseData.length()); // "Serialize"
        }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "AppConnect: " << m_size <<", "<< m_ip;
            return ss.str();
        }

        EVENT_CLASS_TYPE(AppConnect);
        EVENT_CLASS_CATEGORY(EventCategoryApplication);
    private:
        unsigned short m_size;
        std::string m_ip;
    };
    class MYENGINE_API AppConnectConfirmedEvent : public Event
    {
    public:
        AppConnectConfirmedEvent(const unsigned short t)
            : m_playerNum(t) {}

        AppConnectConfirmedEvent(const unsigned short t, const Player& p)
            : Event(p)
            , m_playerNum(t) {}

        inline int GetTeam() const { return m_playerNum; }

        void Serialize(std::string& retBuff) const override
        {
            unsigned short eventPos = 0;
            retBuff.resize(sizeof(unsigned short) + sizeof(short));

            memcpy(&retBuff[eventPos], &m_playerNum, sizeof(m_playerNum)); // "Serialize"
            eventPos += sizeof(m_playerNum);

            std::string baseData;
            SerializeBase(baseData);
            memcpy(&retBuff[eventPos], &baseData[0], baseData.length()); // "Serialize"
        }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "AppConnectConfirmed: " << m_playerNum;
            return ss.str();
        }

        EVENT_CLASS_TYPE(AppConnectConfirmed);
        EVENT_CLASS_CATEGORY(EventCategoryApplication);
    private:
        unsigned short m_playerNum;
    };
	class MYENGINE_API AppTickEvent : public Event
	{
	public:
		AppTickEvent()
		{}


		EVENT_CLASS_TYPE(AppTick);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);
	};

	class MYENGINE_API AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent()
		{}


		EVENT_CLASS_TYPE(AppUpdate);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);
	};

	class MYENGINE_API AppRenderEvent : public Event
	{
	public:
		AppRenderEvent()
		{}


		EVENT_CLASS_TYPE(AppRender);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);
	};
}