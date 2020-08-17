#include "MEpch.h"
#include "WindowsWindow.h"

#include "imgui\imgui.h"
#include "imgui\imgui_impl_glfw.h"
//#include "imgui\imgui_impl_opengl3.h"

//events
#include "ApplicationEvent.h"
#include "MouseEvent.h"
#include "KeyEvent.h"

namespace MYENGINE {

	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		ME_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::InitImGui()
	{
		ImGui_ImplGlfw_InitForOpenGL(m_window, false);
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
        //TODO: anti aliesing setting set in render also
        if (true)
        {
            glfwWindowHint(GLFW_SAMPLES, 4);
        }

        // Set all the required options for GLFW
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

		m_data.title = props.title;
		m_data.width = props.width;
		m_data.height = props.height;

		ME_CORE_INFO("Creating window {0} ({1}, {2})", props.title, props.width, props.height);

		if (!s_GLFWInitialized)
		{
			// TODO: glfwTerminate on system shutdown
			int success = glfwInit();
			ME_CORE_ASSERT(success, "Could not intialize GLFW!");

			s_GLFWInitialized = success;
		}

		m_window = glfwCreateWindow((int)props.width, (int)props.height, m_data.title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_window);
		glfwSetWindowUserPointer(m_window, &m_data);
		SetVSync(true);
		//ImGui::CreateContext();
		//IMGUI_CHECKVERSION();
		//ImGui_ImplGlfw_InitForOpenGL(m_window, true);

		//set GLFW callback events
		glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.width = width;
			data.height = height;

			WindowResizeEvent* event = new WindowResizeEvent(width, height);
			data.EventCallback(*event);
		});

		glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			WindowClosedEvent* event = new WindowClosedEvent();
			data.EventCallback(*event);
		});

		glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
			case GLFW_PRESS:
			{
				KeyPressedEvent* event = new KeyPressedEvent(key, 0);
				data.EventCallback(*event);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleasedEvent* event = new KeyReleasedEvent(key);
				data.EventCallback(*event);
				break;
			}
			case GLFW_REPEAT:
			{
				KeyPressedEvent* event = new KeyPressedEvent(key, 1);
				data.EventCallback(*event);
				break;
			}
			}
		});

		glfwSetCharCallback(m_window, [](GLFWwindow* window, unsigned int keycode)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			KeyTypedEvent* event = new KeyTypedEvent(keycode);
			data.EventCallback(*event);
		});

		glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods)
		{
			double xPos;
			double yPos;
			glfwGetCursorPos(window, &xPos, &yPos);
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
			case GLFW_PRESS:
			{


				MouseButtonPressedEvent* event= new MouseButtonPressedEvent(button, xPos, yPos);
				data.EventCallback(*event);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent* event = new MouseButtonReleasedEvent(button, xPos, yPos);
				data.EventCallback(*event);
				break;
			}
			}
		});

		glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent* event = new MouseScrolledEvent((float)xOffset, (float)yOffset);
			data.EventCallback(*event);
		});

		glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent* event = new MouseMovedEvent((float)xPos, (float)yPos);
            //MouseMovedEvent event = *std::make_shared<MouseMovedEvent>((float)xPos, (float)yPos);

			data.EventCallback(*event);
		});
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_window);
	}

	void WindowsWindow::TerminateAllWindows()
	{
		glfwTerminate();
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_window);
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_data.VSync;
	}

}