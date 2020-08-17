#include "MEpch.h"

#include "ImGuiLayer.h"
#include "KeyCodes.h"

#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"

#include "Application.h"

namespace MYENGINE {

	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
		, m_imguiContext(nullptr)
	{
	}

	ImGuiLayer::ImGuiLayer(const std::string& layerName, const bool& isStaticLayer)
		: Layer(layerName, isStaticLayer)
	{
	}

	ImGuiLayer::~ImGuiLayer()
	{
		ImGui::SetCurrentContext(m_imguiContext);
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnAttach()
	{
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

		//const char* glsl_version = "#version 130";
		//ImGui_ImplOpenGL3_Init(glsl_version);
		ImGuiContext* oldContext = ImGui::GetCurrentContext();
		m_imguiContext = ImGui::CreateContext();
		ImGui::SetCurrentContext(m_imguiContext);
		ImGui_ImplOpenGL3_DestroyFontsTexture();
		Application& app = Application::Get();
		app.GetWindow().InitImGui();

		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		// TEMPORARY: should eventually use Hazel key codes
		io.KeyMap[ImGuiKey_Tab] = ME_KEY_TAB;
		io.KeyMap[ImGuiKey_LeftArrow] = ME_KEY_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = ME_KEY_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = ME_KEY_UP;
		io.KeyMap[ImGuiKey_DownArrow] = ME_KEY_DOWN;
		io.KeyMap[ImGuiKey_PageUp] = ME_KEY_PAGE_UP;
		io.KeyMap[ImGuiKey_PageDown] = ME_KEY_PAGE_DOWN;
		io.KeyMap[ImGuiKey_Home] = ME_KEY_HOME;
		io.KeyMap[ImGuiKey_End] = ME_KEY_END;
		io.KeyMap[ImGuiKey_Insert] = ME_KEY_INSERT;
		io.KeyMap[ImGuiKey_Delete] = ME_KEY_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = ME_KEY_BACKSPACE;
		io.KeyMap[ImGuiKey_Space] = ME_KEY_SPACE;
		io.KeyMap[ImGuiKey_Enter] = ME_KEY_ENTER;
		io.KeyMap[ImGuiKey_Escape] = ME_KEY_ESCAPE;
		io.KeyMap[ImGuiKey_A] = ME_KEY_A;
		io.KeyMap[ImGuiKey_C] = ME_KEY_C;
		io.KeyMap[ImGuiKey_V] = ME_KEY_V;
		io.KeyMap[ImGuiKey_X] = ME_KEY_X;
		io.KeyMap[ImGuiKey_Y] = ME_KEY_Y;
		io.KeyMap[ImGuiKey_Z] = ME_KEY_Z;//*/

		ImGui_ImplOpenGL3_Init("#version 410");
		ImGui_ImplOpenGL3_NewFrame();
		if(oldContext!=nullptr)
		ImGui::SetCurrentContext(oldContext);
	}

	void ImGuiLayer::OnDetach()
	{

	}

	void ImGuiLayer::OnRender()
	{
		ImGui::SetCurrentContext(m_imguiContext);
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

		float time = (float)glfwGetTime();
		io.DeltaTime = m_Time > 0.0f ? (time - m_Time) : (1.0f / 60.0f);
		m_Time = time;

		ImGui::NewFrame();

		OnUpdateGUI(io.DeltaTime);
		//static bool show = true;
		//ImGui::ShowDemoWindow(&show);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiLayer::OnEvent(Event& event)
	{
		ImGui::SetCurrentContext(m_imguiContext);

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT(ImGuiLayer::OnMouseButtonPressedEvent));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(BIND_EVENT(ImGuiLayer::OnMouseButtonReleasedEvent));
		dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT(ImGuiLayer::OnMouseMovedEvent));
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT(ImGuiLayer::OnMouseScrolledEvent));
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT(ImGuiLayer::OnKeyPressedEvent));
		dispatcher.Dispatch<KeyTypedEvent>(BIND_EVENT(ImGuiLayer::OnKeyTypedEvent));
		dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT(ImGuiLayer::OnKeyReleasedEvent));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT(ImGuiLayer::OnWindowResizeEvent));
	}

	bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButton()] = true;

		return false;
	}

	bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButton()] = false;

		return false;
	}

	bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(e.GetX(), e.GetY());

		return false;
	}

	bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheelH += e.GetXOffset();
		io.MouseWheel += e.GetYOffset();

		return false;
	}

	bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.GetKeyCode()] = true;

		io.KeyCtrl = io.KeysDown[ME_KEY_LEFT_CONTROL] || io.KeysDown[ME_KEY_RIGHT_CONTROL];
		io.KeyShift = io.KeysDown[ME_KEY_LEFT_SHIFT] || io.KeysDown[ME_KEY_RIGHT_SHIFT];
		io.KeyAlt = io.KeysDown[ME_KEY_LEFT_ALT] || io.KeysDown[ME_KEY_RIGHT_ALT];
		io.KeySuper = io.KeysDown[ME_KEY_LEFT_SUPER] || io.KeysDown[ME_KEY_RIGHT_SUPER];
		return false;
	}

	bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.GetKeyCode()] = false;

		return false;
	}

	bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		int keycode = e.GetKeyCode();
		if (keycode > 0 && keycode < 0x10000)
			io.AddInputCharacter((unsigned short)keycode);

		return false;
	}

	bool ImGuiLayer::OnWindowResizeEvent(WindowResizeEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(e.GetWidth(), e.GetHeight());
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
		glViewport(0, 0, e.GetWidth(), e.GetHeight());

		return false;
	}

}