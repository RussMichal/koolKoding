#pragma once
#include "Core.h"
#include "Framework\Scenes\Scene.h"
#include "Framework\Layers\GameLobby.h"
#include "Window.h"
#include "Framework\Layers\LayerStack.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Framework/Layers/WindowsNetCode.h"

// GLFW
#include <Vender\\glfw-3.2.1.bin.WIN32\\include\\GLFW\\glfw3.h>

namespace MYENGINE
{
	// Window dimensions
	const GLuint WIDTH = 800, HEIGHT = 400;

	class MYENGINE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		/*template <class scene>
		void AppRegisterScene(const std::string& title) {
			menu.RegisterScene<scene>(title);
		}

		template <class world>
		void AppRegisteWorld(const std::string& title) {
			menu.RegisterWorld<world>(title);
		}*/

		void OnEvent(Event& loc_e);
        void ProcessEvent(Event* e);
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_window; }
		inline static Application& Get() { return *s_instance; }
        inline static PlayerList& GetPlayers() { return m_players; }
        inline static Player& GetPlayer() { return m_players.GetMainPlayer(); }

		bool clearLayerStack;
	private:
		bool OnWindowClosedEvent(WindowClosedEvent& e);
        void ProcessEvents(const std::map<float, Event*>& orderedEvents);

		LayerStack m_LayerStack;
		static Application* s_instance;

		std::unique_ptr<Window> m_window;
        WindowsNetCode Network;
		bool m_running;
        float m_time;
        static PlayerList m_players;

        std::map<float, Event*> m_storedEvents;
        bool m_isHost;
	};

	//template MYENGINE_API void Application::AppRegisterScene<Scene::SceneSteppingStone>(const std::string& s);

	//Defined in CLIENT
	Application* CreateApp();
}