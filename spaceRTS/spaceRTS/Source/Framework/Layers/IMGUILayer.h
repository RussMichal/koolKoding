#pragma once

#include "FrameWork/Layers/Layer.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Events/ApplicationEvent.h"
#include "Vender/imgui/imgui.h"

namespace MYENGINE {

	class MYENGINE_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer(const std::string& layerName, const bool& isStaticLayer = false);
		ImGuiLayer();

		~ImGuiLayer();

		void OnAttach() override;
		void OnDetach() override;
		void OnEvent(Event& event) override;
		void OnRender() override;
		virtual void OnUpdateGUI(const float& dt) {}
	private:
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
		bool OnMouseMovedEvent(MouseMovedEvent& e);
		bool OnMouseScrolledEvent(MouseScrolledEvent& e);
		bool OnKeyPressedEvent(KeyPressedEvent& e);
		bool OnKeyReleasedEvent(KeyReleasedEvent& e);
		bool OnKeyTypedEvent(KeyTypedEvent& e);
		bool OnWindowResizeEvent(WindowResizeEvent& e);

		float m_Time = 0.0f;
		ImGuiContext* m_imguiContext;
	};

}