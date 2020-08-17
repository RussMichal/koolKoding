#include "MEpch.h"

#include "Scene.h"
#include "imgui/imgui.h"
#include "ApplicationEvent.h"

namespace MYENGINE
{
	namespace Scene
	{
		SceneSteppingStone::SceneSteppingStone(Renderer::ShaderType sh,
			const float cc0,
			const float cc1,
			const float cc2,
			const float cc3)
			: m_renderer(Renderer(sh, cc0, cc1, cc2, cc3))
			, m_octTree(OctTree())
			, m_leftMouseDown(false)
		{
			GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
			GLCall(glEnable(GL_BLEND));

			m_renderer.SetConstUniforms(glm::vec3(0, 0, -6), 40.0f, 2.2f);
		}

		SceneSteppingStone::~SceneSteppingStone()
		{
			while (m_objects.size() > 0)
			{
				delete m_objects[0];
				m_objects.erase(m_objects.begin());
			}
			m_renderer.SetDrawBuffers(GL_FRONT_AND_BACK);
			m_renderer.Clear();
			m_renderer.SetDrawBuffers(GL_BACK);
		}

		bool SceneSteppingStone::OnWindowResizeEvent(WindowResizeEvent& e)
		{
			m_windowHeight = (unsigned int)e.GetHeight();

			glViewport(0, 0, e.GetWidth(), e.GetHeight());
			float projScale = m_windowHeight / (1080 / 350);
			m_projection = std::make_unique<glm::mat4>(glm::ortho<float>(-projScale * 16 / 9, projScale * 16 / 9, -projScale, projScale, -200.0f, 200.0f));

			m_octTree = OctTree(BoundingBox(glm::vec3(-projScale * 16 / 9, -projScale,-30), glm::vec3(projScale * 16 / 9, projScale, 20)));
			for (GameObject* obj : m_objects)
			{
				m_octTree.m_pendingInsertion.push(obj);
			}
			m_octTree.UpdateTree();

			return true;
		}

		void SceneSteppingStone::OnEngineUpdate(float deltaTime)
		{
			if (m_leftMouseDown)
			{

			}
		}

		bool SceneSteppingStone::OnMouseButtonPress(MouseButtonPressedEvent& e)
		{
			//TODO:: enum mouse buttons? glfw already does it do we include all of glfw for enum or redefine to work with other window apis?
			//left click
			if (e.GetMouseButton() == 0)
			{
				LeftClick(e.GetX(), e.GetY());
			}
			return true;
		}
		void SceneSteppingStone::LeftClick(double x, double y)
		{
			
			m_mouseLeftPressPos = CalculateMouseWorldPointScene(x, y);;
			m_leftMouseDown = true;
		}

		bool SceneSteppingStone::OnMouseButtonRelease(MouseButtonReleasedEvent& e)
		{
			//TODO:: enum mouse buttons? glfw already does it do we include all of glfw for enum or redefine to work with other window apis?
			//left click

			if (e.GetMouseButton() == 0)
			{
				LeftRelease(e.GetX(), e.GetY());
			}
			else if (e.GetMouseButton() == 1)
			{
				RightRelease(e.GetX(), e.GetY());
			}
			return true;
		}

		void SceneSteppingStone::LeftRelease(double x, double y)
		{
			glm::vec3 pressRel = CalculateMouseWorldPointScene(x, y);
			ME_CORE_INFO("left pressPos:{0}, {1}, {2}", m_mouseLeftPressPos.x, m_mouseLeftPressPos.y, m_mouseLeftPressPos.z);
			ME_CORE_INFO("left pressRel:{0}, {1}, {2}", pressRel.x, pressRel.y, pressRel.z);

			std::vector<glm::vec3> points;
			points.push_back(m_mouseLeftPressPos);
			points.push_back(glm::vec3(pressRel.x, m_mouseLeftPressPos.y, m_mouseLeftPressPos.z));
			points.push_back(glm::vec3(m_mouseLeftPressPos.x, pressRel.y, m_mouseLeftPressPos.z));
			points.push_back(glm::vec3(pressRel.x, pressRel.y, m_mouseLeftPressPos.z));
			points.push_back(pressRel);
			points.push_back(glm::vec3(pressRel.x, m_mouseLeftPressPos.y, pressRel.z));
			points.push_back(glm::vec3(m_mouseLeftPressPos.x, pressRel.y, pressRel.z));
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

			BoundingFrustum bf(min, max);
			std::vector<IntersectionRecord> selected = m_octTree.GetIntersection(bf);
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
					go->m_isSelected = true;
					selected.erase(it);
				}
				else
				{
					go->m_isSelected = false;
				}
			}
			m_leftMouseDown = false;
		}

		void SceneSteppingStone::RightRelease(double x, double y)
		{
			glm::vec3 pressRel = CalculateMouseWorldPointScene(x, y);
			ME_CORE_INFO("right pressRel:{0}, {1}, {2}", pressRel.x, pressRel.y, pressRel.z);

			for (GameObject* go : m_objects)
			{
				if (go->m_isSelected)
				{
					float angle = -1 * (glm::radians(90.0f) - atan2(pressRel.y - go->GetPosition().y, pressRel.x - go->GetPosition().x));
					float degAngle = glm::degrees(angle);
					go->SetOrientation(glm::quat(glm::vec3(0.0f, 0.0f, angle)));
				}
			}
		}

		SceneMenu::SceneMenu(Scene *& currSence)
			:m_curScene(currSence)
		{

		}

		SceneMenu::~SceneMenu()
		{
		}

		void SceneMenu::OnImGuiRender()
		{
			for (std::pair<std::string, std::function<Scene*()>>& scene : m_scenes)
			{
				if (ImGui::Button(scene.first.c_str()))
				{
					m_curScene = scene.second();
				}
			}
		}
	}
}