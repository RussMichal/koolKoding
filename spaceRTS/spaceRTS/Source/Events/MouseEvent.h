#pragma once

#include "Event.h"

#include<sstream>

namespace MYENGINE
{
	class MYENGINE_API MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(float x, float y)
			: m_mouseX(x)
			, m_mouseY(y)
		{}

        MouseMovedEvent(float x, float y, Player p)
            : Event(p)
            , m_mouseX(x)
            , m_mouseY(y)
        {}

		inline float GetX() const { return m_mouseX; }
		inline float GetY() const { return m_mouseY; }
        void Serialize(std::string& retBuff) const override
        {
            unsigned short eventPos = 0;
            retBuff.resize(2 * sizeof(float) + sizeof(short));

            memcpy(&retBuff[eventPos], &m_mouseX, sizeof(m_mouseX)); // "Serialize"
            eventPos += sizeof(m_mouseX);

            memcpy(&retBuff[eventPos], &m_mouseY, sizeof(m_mouseY)); // "Serialize"
            eventPos += sizeof(m_mouseY);

            std::string baseData;
            SerializeBase(baseData);
            memcpy(&retBuff[eventPos], &baseData[0], baseData.length()); // "Serialize"
        }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: " << m_mouseX << ", " << m_mouseY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryMouse|EventCategoryInput)
	private:
		float m_mouseX;
		float m_mouseY;
	};

	class MYENGINE_API MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(const float xOffset, const float yOffset)
			: m_xOffset(xOffset)
			, m_yOffset(yOffset)
		{}
        MouseScrolledEvent(const float xOffset, const float yOffset, const Player& p)
            : Event(p)
            , m_xOffset(xOffset)
            , m_yOffset(yOffset)
        {}

		inline float GetXOffset() const { return m_xOffset; }
		inline float GetYOffset() const { return m_yOffset; }

        void Serialize(std::string& retBuff) const override
        {
            unsigned short eventPos = 0;
            retBuff.resize(2 * sizeof(float) + sizeof(short));

            memcpy(&retBuff[eventPos], &m_xOffset, sizeof(m_xOffset)); // "Serialize"
            eventPos += sizeof(m_xOffset);

            memcpy(&retBuff[eventPos], &m_yOffset, sizeof(m_yOffset)); // "Serialize"
            eventPos += sizeof(m_yOffset);

            std::string baseData;
            SerializeBase(baseData);
            memcpy(&retBuff[eventPos], &baseData[0], baseData.length()); // "Serialize"
        }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << GetXOffset() << ", " << GetYOffset();
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		float m_xOffset;
		float m_yOffset;
	};

	class MYENGINE_API MouseButtonEvent : public Event
	{
	public:
		inline int GetMouseButton() const { return m_button; }
		inline float GetX() const { return m_mouseX; }
		inline float GetY() const { return m_mouseY; }

        void Serialize(std::string& retBuff) const override
        {
            unsigned short eventPos = 0;
            retBuff.resize(sizeof(int) + 2 * sizeof(float) + sizeof(short));

            memcpy(&retBuff[eventPos], &m_button, sizeof(m_button)); // "Serialize"
            eventPos += sizeof(m_button);

            memcpy(&retBuff[eventPos], &m_mouseX, sizeof(m_mouseX)); // "Serialize"
            eventPos += sizeof(m_mouseX);

            memcpy(&retBuff[eventPos], &m_mouseY, sizeof(m_mouseY)); // "Serialize"
            eventPos += sizeof(m_mouseY);

            std::string baseData;
            SerializeBase(baseData);
            memcpy(&retBuff[eventPos], &baseData[0], baseData.length()); // "Serialize"
        }

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	protected:
		MouseButtonEvent(const int button, const float x, const float y)
			: m_button(button)
			, m_mouseX(x)
			, m_mouseY(y)
		{}
        MouseButtonEvent(const int button, const float x, const float y, const Player& p)
            : Event(p)
            , m_button(button)
            , m_mouseX(x)
            , m_mouseY(y)
        {}

		int m_button;
		float m_mouseX;
		float m_mouseY;
	};

	class MYENGINE_API MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(const int button, const float x, const float y)
			: MouseButtonEvent(button, x, y)
		{}

        MouseButtonPressedEvent(const int button, const float x, const float y, const Player& p)
            : MouseButtonEvent(button, x, y, p)
        {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << m_button << ", " << m_mouseX << ", " << m_mouseY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class MYENGINE_API MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(const int button, const float x, const float y)
			: MouseButtonEvent(button, x, y)
		{}
        MouseButtonReleasedEvent(const int button, const float x, const float y, const Player& p)
            : MouseButtonEvent(button, x, y, p)
        {}
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << m_button << ", " << m_mouseX << ", " << m_mouseY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};
}