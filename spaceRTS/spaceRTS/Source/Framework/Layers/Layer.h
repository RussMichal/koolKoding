#pragma once

#include "Core.h"
#include "Events/Event.h"

namespace MYENGINE {

	class MYENGINE_API Layer
	{
	public:
		Layer(const std::string& name = "Layer", const bool& i_staticLayer = false);
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnEngineUpdate(const float& deltaTime) {}
		virtual void OnUpdate(const float& deltaTime) {}
		virtual void OnRender() {}
		//virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_debugName; }

		bool staticLayer;
	protected:
		std::string m_debugName;
	};

}