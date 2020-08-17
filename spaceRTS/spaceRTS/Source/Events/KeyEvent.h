#pragma once

#include "Event.h"

#include<sstream>

namespace MYENGINE
{
	class MYENGINE_API KeyEvent : public Event
	{
	public:
		inline int GetKeyCode() const { return m_keyCode; }

        virtual void Serialize(std::string& retBuff) const override
        {
            unsigned short eventPos = 0;
            retBuff.resize(sizeof(int) + sizeof(short));

            memcpy(&retBuff[eventPos], &m_keyCode, sizeof(m_keyCode)); // "Serialize"
            eventPos += sizeof(m_keyCode);

            std::string baseData;
            SerializeBase(baseData);
            memcpy(&retBuff[eventPos], &baseData[0], baseData.length()); // "Serialize"
        }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		KeyEvent(int keycode)
			:m_keyCode(keycode) {}
        KeyEvent(int keycode, const Player& p)
            : Event(p)
            , m_keyCode(keycode) {}
		int m_keyCode;
	};

	class MYENGINE_API KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(const int keyCode, const int repeatCount)
			: KeyEvent(keyCode)
			, m_repeatCount(repeatCount){}

        KeyPressedEvent(const int keyCode, const int repeatCount, const Player& p)
            : KeyEvent(keyCode, p)
            , m_repeatCount(repeatCount) {}

		inline int GetRepeatCount() const { return m_repeatCount; }

        virtual void Serialize(std::string& retBuff) const override
        {
            unsigned short eventPos = 0;
            retBuff.resize(2 * sizeof(int) + sizeof(short));

            memcpy(&retBuff[eventPos], &m_keyCode, sizeof(m_keyCode)); // "Serialize"
            eventPos += sizeof(m_keyCode);

            memcpy(&retBuff[eventPos], &m_repeatCount, sizeof(m_repeatCount)); // "Serialize"
            eventPos += sizeof(m_repeatCount);

            std::string baseData;
            SerializeBase(baseData);
            memcpy(&retBuff[eventPos], &baseData[0], baseData.length()); // "Serialize"
        }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_keyCode << " (" << m_repeatCount << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed);
	private:
		int m_repeatCount;
	};

	class MYENGINE_API KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(const int keyCode)
			:KeyEvent(keyCode) {}

        KeyReleasedEvent(const int keyCode, const Player& p)
            :KeyEvent(keyCode, p) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_keyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased);
	};

	class MYENGINE_API KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(const int keycode)
			: KeyEvent(keycode) {}

        KeyTypedEvent(const int keycode, const Player& p)
            : KeyEvent(keycode, p) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_keyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};
}