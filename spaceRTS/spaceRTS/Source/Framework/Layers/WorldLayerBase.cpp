#include "MEpch.h"

#include "WorldLayerBase.h"
#include "MouseButtonCodes.h"
#include "imgui/imgui.h"
#include "ApplicationEvent.h"
#include "Application.h"

namespace MYENGINE
{
	WorldLayerBase::WorldLayerBase(Renderer::ShaderType sh,
        Player mainPlayer,
		const float cc0,
		const float cc1,
		const float cc2,
		const float cc3)
		: m_renderer(Renderer(sh, cc0, cc1, cc2, cc3))
		, m_octTree(OctTree())
		, inputPulling(WindowsInput())
        , m_mainPlayer(mainPlayer)
	{
		m_renderer.SetBlend();
		m_renderer.SetConstUniforms(glm::vec3(0, 0, -6), 40.0f, 2.2f);

		AssetProperties box = al.Assets[Asset::Square];
		box.texture = "";
		box.indices = { 0,1,2,3 };

		m_primitiveObjects.push_back(new PrimitiveObject(box, mainPlayer.color));
		m_primitiveObjects[0]->m_mainObject.m_drawMode = GL_LINE_LOOP;
	}

	WorldLayerBase::~WorldLayerBase()
	{
		while (m_objects.size() > 0)
		{
			delete m_objects[0];
			m_objects.erase(m_objects.begin());
		}
		while (m_primitiveObjects.size() > 0)
		{
			delete m_primitiveObjects[0];
			m_primitiveObjects.erase(m_primitiveObjects.begin());
		}

		m_renderer.SetDrawBuffers(GL_FRONT_AND_BACK);
		m_renderer.Clear();
		m_renderer.SetDrawBuffers(GL_BACK);
	}
	void WorldLayerBase::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT(WorldLayerBase::OnWindowResizeEvent));
		dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT(WorldLayerBase::OnMouseButtonPress));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(BIND_EVENT(WorldLayerBase::OnMouseButtonRelease));
	}
	void WorldLayerBase::PushLayer(Layer* layer)
	{
		Application& app = Application::Get();
		app.PushLayer(layer);
	}
	void WorldLayerBase::PushOverlay(Layer* overlay)
	{
		Application& app = Application::Get();
		app.PushOverlay(overlay);
	}
	bool WorldLayerBase::OnWindowResizeEvent(const WindowResizeEvent& e)
	{
        //only resize window if main player
        if (e.GetPlayer().playerNum == m_mainPlayer.playerNum)
        {
            m_windowHeight = (unsigned int)e.GetHeight();

            glViewport(0, 0, e.GetWidth(), e.GetHeight());
            float projScale = m_windowHeight / (1080 / 350);
            m_projection = std::make_unique<glm::mat4>(glm::ortho<float>(-projScale * 16 / 9, projScale * 16 / 9, -projScale, projScale, -200.0f, 200.0f));

            m_octTree = OctTree(BoundingBox(glm::vec3(-projScale * 16 / 9, -projScale, -30), glm::vec3(projScale * 16 / 9, projScale, 20)));
            for (GameObject* obj : m_objects)
            {
                m_octTree.m_pendingInsertion.push(obj);
            }
            m_octTree.UpdateTree();

            return true;
        }
        else
        {
            return false;
        }
	}

	void WorldLayerBase::OnEngineUpdate(const float& deltaTime)
	{
        m_octTree.m_treeBuilt = false;
        for (int i=0; i< m_objects.size();i++)
        {
            if (m_objects[i]->health <= 0)
            {
                delete m_objects[i];
                m_objects[i] = nullptr;
                m_objects.erase(m_objects.begin() + i);
                i--;
                continue;
            }
            m_octTree.m_pendingInsertion.push(m_objects[i]);
        }
        m_octTree.UpdateTree();

		for (GameObject* go : m_objects)
		{
            go->UpdateGameObject(m_octTree, deltaTime);
		}

        //use instant events for all ui functions. not just input so there is not a delay on resving the mouse button pressed events
		if (inputPulling.IsMouseButtonPressed(ME_MOUSE_BUTTON_LEFT))
		{
			std::pair<float,float> currentMousePos = inputPulling.GetMousePosition();
			glm::vec3 currentMousePosWorld = CalculateMouseWorldPoint(currentMousePos.first, currentMousePos.second);

			float deltaX = currentMousePosWorld.x - m_mouseLeftPressPos[m_mainPlayer.playerNum].x;
			float deltaY = currentMousePosWorld.y - m_mouseLeftPressPos[m_mainPlayer.playerNum].y;
			m_primitiveObjects[0]->SetScale(glm::vec3(deltaX / 2, deltaY / 2, 1.0f));
			m_primitiveObjects[0]->SetPosition(glm::vec3(deltaX / 2+ m_mouseLeftPressPos[m_mainPlayer.playerNum].x, deltaY / 2+ m_mouseLeftPressPos[m_mainPlayer.playerNum].y, 1.0f));
		}
	}

	bool WorldLayerBase::OnMouseButtonPress(const MouseButtonPressedEvent& e)
	{
		//TODO:: enum mouse buttons? glfw already does it do we include all of glfw for enum or redefine to work with other window apis?
		//left click
		if (e.GetMouseButton() == ME_MOUSE_BUTTON_LEFT)
		{
			LeftClick( e);
		}
		return true;
	}
	void WorldLayerBase::LeftClick(const MouseButtonPressedEvent& e)
	{
		m_mouseLeftPressPos[e.GetPlayer().playerNum] = CalculateMouseWorldPoint(e.GetX(), e.GetY());
	}

	bool WorldLayerBase::OnMouseButtonRelease(const MouseButtonReleasedEvent& e)
	{
		//TODO:: enum mouse buttons? glfw already does it do we include all of glfw for enum or redefine to work with other window apis?
		//left click

		if (e.GetMouseButton() == ME_MOUSE_BUTTON_LEFT)
		{
			LeftRelease(e);
		}
		else if (e.GetMouseButton() == ME_MOUSE_BUTTON_RIGHT)
		{
			RightRelease(e);
		}
		return true;
	}

	void WorldLayerBase::LeftRelease(const MouseButtonReleasedEvent& e)
	{
		glm::vec3 pressRel = CalculateMouseWorldPoint(e.GetX(), e.GetY());
		ME_CORE_INFO("left pressPos:{0}, {1}, {2}", m_mouseLeftPressPos[e.GetPlayer().playerNum].x, m_mouseLeftPressPos[e.GetPlayer().playerNum].y, m_mouseLeftPressPos[e.GetPlayer().playerNum].z);
		ME_CORE_INFO("left pressRel:{0}, {1}, {2}", pressRel.x, pressRel.y, pressRel.z);

		std::vector<glm::vec3> points;
		points.push_back(m_mouseLeftPressPos[e.GetPlayer().playerNum]);
		points.push_back(glm::vec3(pressRel.x, m_mouseLeftPressPos[e.GetPlayer().playerNum].y, m_mouseLeftPressPos[e.GetPlayer().playerNum].z));
		points.push_back(glm::vec3(m_mouseLeftPressPos[e.GetPlayer().playerNum].x, pressRel.y, m_mouseLeftPressPos[e.GetPlayer().playerNum].z));
		points.push_back(glm::vec3(pressRel.x, pressRel.y, m_mouseLeftPressPos[e.GetPlayer().playerNum].z));
		points.push_back(pressRel);
		points.push_back(glm::vec3(pressRel.x, m_mouseLeftPressPos[e.GetPlayer().playerNum].y, pressRel.z));
		points.push_back(glm::vec3(m_mouseLeftPressPos[e.GetPlayer().playerNum].x, pressRel.y, pressRel.z));
		points.push_back(glm::vec3(pressRel.x, pressRel.y, pressRel.z));
		float minX = std::numeric_limits<float>::max();
		float minY = std::numeric_limits<float>::max();
		float minZ = std::numeric_limits<float>::max();

		float maxX = -std::numeric_limits<float>::max();
		float maxY = -std::numeric_limits<float>::max();
		float maxZ = -std::numeric_limits<float>::max();

		for (glm::vec3 point : points)
		{
			minX = std::min(point.x, minX);
			maxX = std::max(point.x, maxX);

			minY = std::min(point.y, minY);
			maxY = std::max(point.y, maxY);

			minZ = std::min(point.z, minZ);
			maxZ = std::max(point.z, maxZ);
		}
		glm::vec3 min = glm::vec3(minX, minY, minZ);
		glm::vec3 max = glm::vec3(maxX, maxY, maxZ);

		const BoundingFrustum bf(min, max);
		std::vector<IntersectionRecord> selected = m_octTree.GetIntersectionInPlayer(&bf, (unsigned int)e.GetPlayer().playerNum);
		for (GameObject* go: m_objects)
		{
			unsigned int curID = go->ID;
			std::vector<IntersectionRecord>::iterator it = find_if(selected.begin(), selected.end(), 
				[curID](const IntersectionRecord& s) 
			{ 
				return s.GetGameObject()->ID == curID;
			});

			if (it != selected.end())
			{
				go->m_isSelected[e.GetPlayer().playerNum] = true;
				selected.erase(it);
			}
			else
			{
				go->m_isSelected[e.GetPlayer().playerNum] = false;
			}
		}
	}

	void WorldLayerBase::RightRelease(const MouseButtonReleasedEvent& e)
	{
		glm::vec3 pressRel = CalculateMouseWorldPoint(e.GetX(), e.GetY());
		ME_CORE_INFO("right pressRel:{0}, {1}, {2}", pressRel.x, pressRel.y, pressRel.z);

		for (GameObject* go : m_objects)
		{
			if (go->m_isSelected[e.GetPlayer().playerNum])
			{
				float angle = -1 * (glm::radians(90.0f) - atan2(pressRel.y - go->GetPosition().y, pressRel.x - go->GetPosition().x));
				float degAngle = glm::degrees(angle);
				go->SetOrientation(glm::quat(glm::vec3(0.0f, 0.0f, angle)));
                go->destination = pressRel;
			}
		}
	}
}