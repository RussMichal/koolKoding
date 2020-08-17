#include "MEpch.h"

// GLEW
#define GLEW_STATIC

#include "Application.h"
#include "Log.h"
#include "Event.h"
#include "ApplicationEvent.h"
#include "MouseEvent.h"

namespace MYENGINE
{
	//my types
	typedef unsigned int uint;
#define BIND_EVENT(x) std::bind(&x,this,std::placeholders::_1)
#define BIND_EVENT_SCENESTEPPINGSTONE(x) std::bind(&x,currScene,std::placeholders::_1)

    //init statics
	Application* Application::s_instance = nullptr;
    PlayerList Application::m_players = { std::vector<Player>{Player(PlayerNumber::Player1, Team::Team1, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f))}, 0 };

	Application::Application()
		: m_running(true)
        , m_time(0.0f)
		, clearLayerStack(false)
        , Network(WindowsNetCode())
        , m_isHost(true)
    {
		Log::Init();
		
		ME_CORE_TRACE("Starting...");

		//set instance
		ME_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_instance = this;

		// Create a GLFWwindow object that we can use for GLFW's functions
		m_window = std::unique_ptr<Window>(Window::Create()); //glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
		m_window->SetEventCallback(BIND_EVENT(Application::OnEvent));

		//int screenWidth, screenHeight;
		//GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		//const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		//privates.window = glfwCreateWindow(mode->width, mode->height, "LearnOpenGL", monitor, nullptr);
		//glfwGetFramebufferSize(privates.window, &screenWidth, &screenHeight);

		/*if (nullptr == m_window)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();

			return;// EXIT_FAILURE;
		}

		glfwMakeContextCurrent(privates.window);*/

		// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
		glewExperimental = GL_TRUE;

		// Initialize GLEW to setup the OpenGL Function pointers
		if (GLEW_OK != glewInit())
		{
			std::cout << "Failed to initialize GLEW" << std::endl;
			return;// EXIT_FAILURE;
		}

		//glfwSwapInterval(1);

		// Define the viewport dimensions
		glViewport(0, 0, m_window->GetWidth(), m_window->GetHeight());

#pragma region oldCodeMovedOutToScene/mat.
		/*//Camera cam;
		//cam.Translate(glm::vec3(1.0f, 0.0f, 0.0f));

		//glm::mat4 model = glm::mat4(1.0f);
		//glm::mat4 view = cam.GetView();
		//glm::mat4 proj = glm::ortho<float>(-2.0f, 2.0f, -1.0f, 1.0f, -1.0f, 1.0f);

		Shaders shader("..//res//shaders//Basic.shader");
		shader.bind();

		shader.SetUniform4f("u_color", 1.0f, 0.5f, 0.2f, 1.0f);
		shader.SetUniform1i("u_texture", 0);

		Texture texMetal("..//res//Textures//Metal.png");
		texMetal.Bind();

		// Set up vertex data (and buffer(s)) and attribute pointers
		
			float vertices[] =
			{
				-0.5f, -0.5f, 0.0f, 0.0, 0.0, // Left
				0.5f, -0.5f, 0.0f, 1.0, 0.0, // Right
				0.0f,  0.5f, 0.0f, 0.5,1.0  // Top
			};

			unsigned int indies[] =
			{
				0,1,2
			};

			GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
			GLCall(glEnable(GL_BLEND));

			// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).

			VertexBuffer vb(vertices, 3 * 5 * sizeof(float));
			//vb.Bind();

			VertexArray va;
			VertexBufferLayout vbl;
			vbl.Push<float>(3);
			vbl.Push<float>(2);
			va.AddBuffer(vb, vbl);


			IndexBuffer ib(indies, 3);
			ib.Bind();

			vb.Unbind(); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
			va.Unbind(); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)

			//imgui vars
			ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.0f);
			bool show_demo_window = true;
			bool show_another_window = false;
			static float f = 0.0f;
			glm::vec3 pos(0.0, 0.0, 0.0);//*/

#pragma endregion


		

			//init imgui
			//ImGui::CreateContext();
			//IMGUI_CHECKVERSION();
			//ImGui::CreateContext();
			//ImGuiIO& io = ImGui::GetIO(); (void)io;
			//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
			//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

			const char* glsl_version = "#version 130";
			//ImGui_ImplOpenGL3_Init(glsl_version);
			//ImGui::StyleColorsDark();

			//scene menu
			//menu.RegisterWorld<CommanderLayer>("CommanderLayer");
			//menu.RegisterScene<Scene::Map>("Map");
			//menu.RegisterWorld<Scene::Projection>("Projection");
			//menu.RegisterWorld<Scene::Scale>("Scale");

			//scenes
			/*Scene::Commander commander(1.0f,0.0f,0.0f,1.0f);//*/
            GameLobby* lobby = new GameLobby();
            //lobby->SetEventCallback(BIND_EVENT(Application::OnEvent));

			PushLayer(lobby);
    }

	Application::~Application()
	{
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Event& loc_e)
	{
        float time = glfwGetTime();
        m_storedEvents[time]= &loc_e;//NEED TO DELETE POINTER WHEN CLEARING THIS LIST
	}
    void Application::ProcessEvent(Event* e)
    {
        EventDispatcher dispatcher(*e);
        dispatcher.Dispatch<WindowClosedEvent>(BIND_EVENT(Application::OnWindowClosedEvent));
        //dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_SCENESTEPPINGSTONE(Scene::Scene::OnWindowResizeEvent));
        //dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_SCENESTEPPINGSTONE(Scene::Scene::OnMouseButtonPress));
        //dispatcher.Dispatch<MouseButtonReleasedEvent>(BIND_EVENT_SCENESTEPPINGSTONE(Scene::Scene::OnMouseButtonRelease));

        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
        {
            (*--it)->OnEvent(*e);
            if (e->handled)
                break;
        }
        ME_CORE_INFO("{0}", *e);
    }

    void Application::ProcessEvents(const std::map<float, Event*>& orderedEvents)
    {
        for (std::pair<float,Event*> ep : orderedEvents)
        {
            ProcessEvent(ep.second);
        }
    }

	// The MAIN function, from here we start the application and run the game loop
	void Application::Run()
	{
			// Game loop
			//while (!glfwWindowShouldClose(privates.window))
        while (m_running)
        {
            std::map<float, Event*> orderedEvents = Network.Recv(m_players);
            ProcessEvents(orderedEvents);
            if (m_isHost)
            {
                //Network.Send(orderedEvents);
            }

            Network.Send(m_storedEvents);
            {
                auto it = orderedEvents.begin();
                while (it != orderedEvents.end())
                {
                    delete it->second;
                    it->second = nullptr;
                    orderedEvents.erase(it);
                    it = orderedEvents.begin();
                }
            }
            {
                auto it = m_storedEvents.begin();
                while (it != m_storedEvents.end())
                {
                    delete it->second;
                    it->second = nullptr;
                    m_storedEvents.erase(it);
                    it = m_storedEvents.begin();
                }
            }
            float time = (float)glfwGetTime();
            float DeltaTime = m_time > 0.0f ? (time - m_time) : (1.0f / 60.0f);
            m_time = time;

			for (Layer* layer : m_LayerStack)
			{
				int layerStackSize = m_LayerStack.size();

				layer->OnEngineUpdate(DeltaTime);
				layer->OnUpdate(DeltaTime);
				layer->OnRender();

				if (layerStackSize != m_LayerStack.size()|| clearLayerStack)
				{
					break;
				}
			}
			if (clearLayerStack)
			{
				m_LayerStack.clear();
				PushOverlay(new GameLobby());
				clearLayerStack = false;
			}

			// Swap the screen buffers
			// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
			m_window->OnUpdate();
		}
		//ImGui_ImplOpenGL3_Shutdown();
		//ImGui_ImplGlfw_Shutdown();
		//ImGui::DestroyContext();

		//glfwDestroyWindow(privates.window);

		// Terminate GLFW, clearing any resources allocated by GLFW.
		//glfwTerminate();
		m_window->TerminateAllWindows();
		//return EXIT_SUCCESS;
	}

	bool Application::OnWindowClosedEvent(WindowClosedEvent& e)
	{
        m_running = false;
		return true;
	}

	/*template<class scene>
	void Application::AppRegisterScene(const std::string& title)
	{
		menu.RegisterScene<scene>(title);
	}*/

}