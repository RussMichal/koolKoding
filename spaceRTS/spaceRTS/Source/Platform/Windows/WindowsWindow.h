#pragma once

#include "Window.h"

#include <glfw-3.2.1.bin.WIN32/include/GLFW/glfw3.h>

namespace MYENGINE {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void TerminateAllWindows() override;

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_data.width; }
		inline unsigned int GetHeight() const override { return m_data.height; }

		// Window attributes
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		inline virtual void* GetNativeWindow() const { return m_window; }

	private:
		virtual void Init(const WindowProps& props);
		virtual void InitImGui();
		virtual void Shutdown();
	private:
		GLFWwindow* m_window;

		struct WindowData
		{
			std::string title;
			unsigned int width, height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_data;
	};

}