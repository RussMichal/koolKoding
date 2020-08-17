#include "MEpch.h"

#include "Layers\Tests\ProjectionLayer.h"

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
	ProjectionLayer::ProjectionLayer(const Player& mainPlayer, Renderer::ShaderType sh, float cc0, float cc1, float cc2, float cc3) :WorldLayerBase(sh, mainPlayer, cc0, cc1, cc2, cc3)
		, m_IMGUILayer(new ProjectionIMGUILayer())
	{
		PushOverlay(m_IMGUILayer);

		AssetProperties square = al.Assets[Asset::Square];
        m_mainPlayer.color = glm::vec4(1.0f, 0.4f, 0.1f, 1.0f);
		m_objects.push_back(new GameObject(square, m_mainPlayer));
        m_mainPlayer.color = glm::vec4(0.1f, 0.4f, 1.0f, 1.0f);
		m_objects.push_back(new GameObject(square, m_mainPlayer));
        m_mainPlayer.color = glm::vec4(0.1f, 0.4f, 1.0f, 1.0f);
		m_objects.push_back(new GameObject(square, m_mainPlayer));
        m_mainPlayer.color = glm::vec4(1.0f, 0.4f, 0.1f, 1.0f);
		m_objects.push_back(new GameObject(square, m_mainPlayer));

		AssetProperties tri = al.Assets[Asset::Triangle];
        m_mainPlayer.color = glm::vec4(0.1f, 1.0f, 0.4f, 1.0f);
		m_objects.push_back(new GameObject(tri, m_mainPlayer));

		/*m_objects.push_back(new GameObject(vertices, indies, "..//res//Textures//Metal.png", glm::vec4(1.0f, 0.4f, 0.1f, 1.0f)));
		m_objects.push_back(new GameObject(vertices, indies, "..//res//Textures//Metal.png", glm::vec4(0.1f, 0.4f, 1.0f, 1.0f)));
		m_objects.push_back(new GameObject(vertices, indies, "..//res//Textures//Metal.png", glm::vec4(0.1f, 0.4f, 1.0f, 1.0f)));
		m_objects.push_back(new GameObject(vertices, indies, "..//res//Textures//Metal.png", glm::vec4(1.0f, 0.4f, 0.1f, 1.0f)));
		m_objects.push_back(new GameObject(verticesTri, indiesTri, "..//res//Textures//Metal.png", glm::vec4(0.1f, 1.0f, 0.4f, 1.0f)));*/

		float platfromSize = 20.0f;
		m_objects[0]->SetPositionAndDestination(glm::vec3(-platfromSize, platfromSize, 0.0f));//top lelf
		m_objects[0]->SetScale(glm::vec3(platfromSize, platfromSize, platfromSize));//top lelf

		m_objects[1]->SetPositionAndDestination(glm::vec3(platfromSize, platfromSize, 0.0f));//top right
		m_objects[1]->SetScale(glm::vec3(platfromSize, platfromSize, platfromSize));//top right

		m_objects[2]->SetPositionAndDestination(glm::vec3(-platfromSize, -platfromSize, 0.0f));//lower lelft
		m_objects[2]->SetScale(glm::vec3(platfromSize, platfromSize, platfromSize));//lower lelft

		m_objects[3]->SetPositionAndDestination(glm::vec3(platfromSize, -platfromSize, 0.0f));//lower right
		m_objects[3]->SetScale(glm::vec3(platfromSize, platfromSize, platfromSize));//lower right

		m_objects[4]->SetScale(glm::vec3(platfromSize/2, platfromSize/2, platfromSize / 2));//unit

		//m_objects[0].m_orientation = glm::quat(glm::vec3(0.0, 0.0, 45.0));
	}

	ProjectionLayer::~ProjectionLayer()
	{
	}

	void ProjectionLayer::OnUpdate(const float& deltaTime)
	{
		m_cam.Identity();
		//m_cam.Translate(glm::vec3(0, 0, -1));
		m_cam.Rotate(m_IMGUILayer->camRotZ, glm::vec3(0.0f, 0.0f, 1.0f));
		m_cam.Rotate(m_IMGUILayer->camRotY, glm::vec3(0.0f, 1.0f, 0.0f));
		m_cam.Rotate(m_IMGUILayer->camRotX, glm::vec3(1.0f, 0.0f, 0.0f));
		//m_cam.Translate(glm::vec3(0, 0, 1));
	}

	void ProjectionLayer::OnRender()
	{
		m_renderer.Clear();
		for (GameObject*& currObj : m_objects)
		{
			m_renderer.Draw(*currObj, *m_projection * m_cam.GetView(), m_mainPlayer.playerNum);
		}
	}
}
