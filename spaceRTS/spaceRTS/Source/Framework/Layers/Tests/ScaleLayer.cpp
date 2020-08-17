#include "MEpch.h"

#include "Layers/Tests/ScaleLayer.h"
#include "Drawling\GameObject.h"
#include "VertexBufferLayout.h"
#include "Scenes\Scene.h"

#include "imgui\imgui.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\quaternion.hpp"
//#include <glm/gtx/quaternion.hpp>

namespace MYENGINE
{
	ScaleLayer::ScaleLayer(const Player& mainPlayer, Renderer::ShaderType sh, float cc0, float cc1, float cc2, float cc3) :WorldLayerBase(sh, mainPlayer, cc0, cc1, cc2, cc3)
		, m_IMGUILayer(new ScaleIMGUILayer())
	{
		PushOverlay(m_IMGUILayer);

		AssetProperties tri = al.Assets[Asset::Triangle];
        m_mainPlayer.color = glm::vec4(0.1f, 0.4f, 1.0f, 1.0f);
		m_objects.push_back(new GameObject(tri, m_mainPlayer));
        m_mainPlayer.color = glm::vec4(0.1f, 1.0f, 0.4f, 1.0f);
		m_objects.push_back(new GameObject(tri, m_mainPlayer));
        m_mainPlayer.color = glm::vec4(1.0f, 0.1f, 0.4f, 1.0f);
		m_objects.push_back(new GameObject(tri, m_mainPlayer));

		m_objects[0]->SetPositionAndDestination(glm::vec3( 0.0f,  15.0f, 0.0f));
		m_objects[1]->SetPositionAndDestination(glm::vec3( 0.0f, -15.0f, 0.0f));

		m_objects[2]->SetPositionAndDestination(glm::vec3(200.0f, -100.0f, 0.0f));
		m_objects[2]->SetScale(glm::vec3(40.0f, 40.0f, 1.0f));


		//m_objects[0].m_orientation = glm::quat(glm::vec3(0.0, 0.0, 45.0));
	}

	ScaleLayer::~ScaleLayer()
	{
	}

	void ScaleLayer::OnUpdate(const float& deltaTime)
	{
		//m_objects.back()->SetOrientation(glm::quat(glm::vec3(0.0f,0.0f, glm::radians(m_IMGUILayer->unitRotZ))));

		m_cam.Identity();
		//m_cam.Translate(glm::vec3(0, 0, -1));
		m_cam.Rotate(m_IMGUILayer->camRotZ, glm::vec3(0.0f, 0.0f, 1.0f));
		m_cam.Rotate(m_IMGUILayer->camRotY, glm::vec3(0.0f, 1.0f, 0.0f));
		m_cam.Rotate(m_IMGUILayer->camRotX, glm::vec3(1.0f, 0.0f, 0.0f));
		//m_cam.Translate(glm::vec3(0, 0, 1));

        if (m_objects.size() > 0)
        {
            m_objects[0]->SetScale(glm::vec3(m_IMGUILayer->bullitScale, m_IMGUILayer->bullitScale, m_IMGUILayer->bullitScale));
            m_objects[1]->SetScale(glm::vec3(m_IMGUILayer->unitScale, m_IMGUILayer->unitScale, m_IMGUILayer->unitScale));
        }
	}

	void ScaleLayer::OnRender()
	{
		m_renderer.Clear();
		for (GameObject*& currObj : m_objects)
		{
			m_renderer.Draw(*currObj, *m_projection * m_cam.GetView(), m_mainPlayer.playerNum);
		}
	}
}
