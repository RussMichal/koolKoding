#pragma once
#include "Core.h"
#include <string>
#include <functional>
#include "Framework/Drawling/Player.h"

namespace MYENGINE {
	enum class EventType
	{
		None =0,
		WindowClose,WindowResized,Window,Focus,WindowLostFocus, WindowMoved,
		AppChangeColor,AppChangeTransparency, AppChangeTeam, AppConnect, AppConnectConfirmed, AppTick,AppUpdate,AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory
	{
		None=0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput = BIT(1),
		EventCategoryKeyboard = BIT(2),
		EventCategoryMouse = BIT(3),
		EventCategoryMouseButton = BIT(4),
	};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType(){return EventType::##type;}\
								virtual EventType GetEventType() const override{return GetStaticType();}\
								virtual const char* GetName() const override{return #type;}

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category;}

	class MYENGINE_API Event
	{
	public:
        virtual EventType GetEventType() const { return EventType::None; }// = 0;
        virtual const char* GetName() const { return nullptr; }// = 0;//debug only??
        virtual int GetCategoryFlags() const { return -1; }//= 0;//debug only??
		virtual std::string ToString() const { return GetName(); }//debug only??
        virtual void Serialize(std::string& retBuff) const { }

        inline Player GetPlayer() const { return eventMainPlayer; }
		inline bool IsInCategoryFlags(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}

        bool handled = false;
    protected:
        Event();
        Event(const Player& p);
        void SerializeBase(std::string& buffer) const;
    private:
        Player eventMainPlayer;
	};

	class EventDispatcher
	{
		template<typename T>
		using EventFn = std::function<bool(T&)>;
	public:
		EventDispatcher(Event& event)
			: m_Event(event)
		{}

		template<typename T>
		bool Dispatch(EventFn<T> func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				//buffer
				m_Event.handled = func(*(T*)&m_Event);
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}