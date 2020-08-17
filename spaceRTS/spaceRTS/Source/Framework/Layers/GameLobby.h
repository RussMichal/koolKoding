#pragma once
#include "Framework\Layers\Layer.h"
#include "Framework\Layers\ImGuiLayer.h"
#include "Framework\Drawling\Renderer.h"

namespace MYENGINE {
	class GameLobbyGuiLayer : public ImGuiLayer
	{
	public:
		GameLobbyGuiLayer(std::vector<std::pair<std::string, std::function<Layer*(Player p)>>>& worlds, Layer*& guiCurWorld);
		~GameLobbyGuiLayer();
		void OnUpdateGUI(const float& deltaTime) override;

	private:
		std::vector<std::pair<std::string, std::function<Layer*(Player p)>>>& m_guiWorlds;
		Layer*& m_guiCurWorld;
        std::function<void(Event&)> m_callback;
        bool m_ConnectOpenned;
        std::string m_ip;
	};

	class GameLobby :public Layer
	{
	public:

		GameLobby();
		~GameLobby();

        void OnUpdate(const float& deltaTime) override;
		void OnRender() override;

		template<typename T>
		void RegisterWorld(const std::string& name)
		{
			ME_CORE_TRACE("Register World " + name);

			m_worlds.push_back(std::make_pair(name, [](Player p) {return new T(p); }));
		}
		void OnEvent(Event& event) override;
		void OnAttach() override;

	private:
		GameLobbyGuiLayer* GameLobbyGui;
		Layer* m_curWorld;
		std::vector<std::pair<std::string, std::function<Layer*(Player p)>>> m_worlds;
		bool m_newWorld;
	};
}