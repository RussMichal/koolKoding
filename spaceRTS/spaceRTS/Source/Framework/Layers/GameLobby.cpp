#include "MEpch.h"
#include "GameLobby.h"
#include"Application.h"
#include "Core.h"
#include "Framework/Layers/Units/CommanderLayer.h"
#include "Framework/Layers/Tests/ScaleLayer.h"
#include "Framework/Layers/Tests/ProjectionLayer.h"
#include "Framework/Layers/Tests/CombatLayer.h"

#include "Framework/Layers/IMGUILayer.h"

#include "imgui/imgui.h"
#include "imgui\imgui_impl_opengl3.h"
#include "imgui\imgui_impl_glfw.h"

namespace MYENGINE
{
	GameLobby::GameLobby(): Layer("GameLobby")
	, m_curWorld(nullptr)

    {

	}

	GameLobby::~GameLobby()
	{
	}

	GameLobbyGuiLayer::GameLobbyGuiLayer(std::vector<std::pair<std::string, std::function<Layer*(Player p)>>>& worlds, Layer*& guiCurWorld) : ImGuiLayer("GameLobbyGuiLayer")
	, m_callback(NULL)
    , m_guiWorlds(worlds)
	, m_guiCurWorld(guiCurWorld)
    , m_ConnectOpenned(false)
    , m_ip("")
    {
        Application& app = Application::Get();
        m_callback = std::bind(&Application::OnEvent, &app, std::placeholders::_1);
    }

	GameLobbyGuiLayer::~GameLobbyGuiLayer() {}

	void GameLobbyGuiLayer::OnUpdateGUI(const float& deltaTime)
	{
		ImGui::Begin("World");
		if (m_guiCurWorld == nullptr)
		{
            Application& app = Application::Get();
            PlayerList pl = app.GetPlayers();
            static std::string name = "name";

			for (std::pair<std::string, std::function<Layer*(Player p)>>& world : m_guiWorlds)
			{
				if (ImGui::Button(world.first.c_str()))
				{
					m_guiCurWorld = world.second(pl.currentPlayers[pl.playerIndex]);
				}
			}
            ImGui::End();

            ImGui::Begin("Players");
            for (int i = 0; i < pl.currentPlayers.size(); i++)
            {
                ImGui::PushID(i);
                ImGui::Text(std::to_string(NUM(pl.currentPlayers[i].playerNum)+1).c_str());
                ImGui::SameLine();
                ImGui::PushItemWidth(100);
                ImGui::InputText("##label", &name[0], 10);

                ImGui::PopItemWidth();
                ImGui::SameLine();
                ImVec4 c(pl.currentPlayers[i].color.r, pl.currentPlayers[i].color.g, pl.currentPlayers[i].color.b, pl.currentPlayers[i].color.a);
                if(ImGui::ColorButton("##color", c))
                {
                    // Store current color and open a picker
                    ImGui::OpenPopup("picker");
                }

                if (ImGui::BeginPopup("picker"))
                {
                    ImGui::ColorPicker4("##picker", &c.x);
                    ImGui::EndPopup();
                }

                if (glm::vec4(c.x,c.y,c.z,c.w)!= pl.currentPlayers[i].color)
                {
                    AppChangeColorEvent* e = new AppChangeColorEvent(c.x,c.y,c.z);
                    m_callback(*e);
                }

                const char* items[] = { "1", "2", "3", "4" };

                ImGui::SameLine();
                std::string current_item = std::to_string(NUM(pl.currentPlayers[i].team)+1);
                if (ImGui::BeginCombo("##combo", current_item.c_str())) // The second parameter is the label previewed before opening the combo.
                {
                    for (int n = 0; n < IM_ARRAYSIZE(items); n++)
                    {
                        bool is_selected = (current_item.c_str() == items[n]); // You can store your selection however you want, outside or inside your objects
                        if (ImGui::Selectable(items[n], is_selected))
                        {
                            current_item = items[n];
                        }
                        if (is_selected)
                        {
                            ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
                        }
                    }
                    if (current_item.c_str() != NULL)
                    {
                        Team newTeam = (Team)BIT(std::stoi(current_item.c_str())-1);
                        if (newTeam != pl.currentPlayers[i].team)
                        {
                            AppChangeTeamEvent* e = new AppChangeTeamEvent(newTeam);
                            m_callback(*e);
                            //Application& app = Application::Get();
                            //app.OnEvent(*e);
                        }
                    }
                    ImGui::EndCombo();
                }
                ImGui::PopID();


            }

            if (ImGui::Button("Connect"))
            {
                // Store current color and open a picker
                ImGui::OpenPopup("Connect");
            }

            if (ImGui::BeginPopup("Connect"))
            {
                ImGui::InputText("IP:", &m_ip[0], 10);
                ImGui::EndPopup();
                m_ConnectOpenned = true;
            }
            else if (m_ConnectOpenned)
            {
                AppConnectEvent* e = new AppConnectEvent(m_ip.length(), m_ip);
                m_callback(*e);
                
                m_ConnectOpenned = false;
            }
            ImGui::End();

		}
		else
		{
			if (ImGui::Button("Back"))
			{
				delete m_guiCurWorld;
				m_guiCurWorld = nullptr;

				Application& app = Application::Get();
				app.clearLayerStack = true;
			}
            ImGui::End();
		}
	}

	void GameLobby::OnEvent(Event& event)
	{
		if (m_curWorld != nullptr)
		{
			m_curWorld->OnEvent(event);
		}
	}
	void GameLobby::OnAttach()
	{
		Application& app = Application::Get();
		app.PushOverlay(new GameLobbyGuiLayer(m_worlds,m_curWorld));
		RegisterWorld<CommanderLayer>("CommanderLayer");
		RegisterWorld<ScaleLayer>("ScaleLayer");
        RegisterWorld<ProjectionLayer>("ProjectionLayer");
        RegisterWorld<CombatLayer>("CombatLayer");
	}

	void GameLobby::OnUpdate(const float& deltaTime)
	{
		if (m_newWorld && m_curWorld != nullptr)
		{
			//window created event to set init proj
			Application& app = Application::Get();
			WindowResizeEvent windowCreatedEvent(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());
			app.ProcessEvent(&windowCreatedEvent);
		}

		//ImGuiIO& io = ImGui::GetIO();
		//Application& app = Application::Get();
		//io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

		//ImGui_ImplOpenGL3_NewFrame();
		//ImGui_ImplGlfw_NewFrame();
		//ImGui::NewFrame();





			//ImGui::Begin("World");

			if (m_curWorld != nullptr)
			{
				m_curWorld->OnEngineUpdate(deltaTime);
				m_curWorld->OnUpdate(deltaTime);
				m_curWorld->OnRender();

				m_newWorld = false;
			}
			else
			{
				m_newWorld = true;			
			}
		
	}
    void GameLobby::OnRender()
    {
        //static clear or make renderer a member TODO
        Renderer r;
        r.Clear();
        if (m_curWorld != nullptr)
        {
            m_curWorld->OnRender();
        }
    }
}