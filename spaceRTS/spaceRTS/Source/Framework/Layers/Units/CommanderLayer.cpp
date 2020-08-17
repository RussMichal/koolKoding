#include "MEpch.h"

#include "Layers\Units\CommanderLayer.h"
#include "Layers\Units\CommanderIMGUILayer.h"

#include "Drawling\GameObject.h"
#include "ObjectLoading\FBXFileParser.h"

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\quaternion.hpp"
//#include <glm/gtx/quaternion.hpp>

namespace MYENGINE
{
	CommanderLayer::CommanderLayer(const Player& mainPlayer, Renderer::ShaderType sh, float cc0, float cc1, float cc2, float cc3 ):WorldLayerBase(sh,mainPlayer, cc0,cc1,cc2,cc3)
		, m_IMGUILayer(new CommanderIMGUILayer())
	{
		std::vector<float> vertexData =
		{	//vertices				tex coord		normals
			-0.5f, -0.5f, 0.0f,     0.0f, 0.0f,     0.2f,0.0f,-0.2f, // Left
				0.5f, -0.5f, 0.0f,     1.0f, 0.0f,    -0.2f,0.0f,-0.2f, // Right
				0.0f,  0.5f, 0.0f,     0.5f, 1.0f,     0.0f,0.2f,-0.2f  // Top
		};

		std::vector<unsigned int> vertexIndices =
		{
			0,1,2
		};
		 
		PushOverlay(m_IMGUILayer);
		//OBJParser o = OBJParser("Commander\\baseMod.obj");
		//FBXFileParser("test.txt");

		//m_objects.push_back(new GameObject(o.vertexData, o.vertexIndices, "..//res//OBJModels//Textures//sh3.jpg", glm::vec4(0.1f, 0.4f, 1.0f, 1.0f)));
		m_objects.push_back(new GameObject(al.Assets[Asset::OGCommander], m_mainPlayer));
		//m_objects[0]->SetOrientation(glm::quat(glm::vec3(0.0,0.0,45.0)));
		m_objects[0]->SetScale(glm::vec3(110,110,110));
		glEnable(GL_DEPTH_TEST);
	}
		
	CommanderLayer::~CommanderLayer()
	{
	}

	void CommanderLayer::OnUpdate(const float& deltaTime)
	{
		m_renderer.clearColor[0] = m_IMGUILayer->clearColor[0];
		m_renderer.clearColor[1] = m_IMGUILayer->clearColor[1];
		m_renderer.clearColor[2] = m_IMGUILayer->clearColor[2];
		m_renderer.clearColor[3] = m_IMGUILayer->clearColor[3];

		m_renderer.SetConstUniforms(glm::vec3(m_IMGUILayer->lightPosX, m_IMGUILayer->lightPosY, m_IMGUILayer->lightPosZ), m_IMGUILayer->lightPower, 2.2f);

		m_cam.Identity();
		m_cam.Rotate(m_IMGUILayer->unitRotZ, glm::vec3(0.0f, 0.0f, 1.0f));
		m_cam.Rotate(m_IMGUILayer->unitRotY, glm::vec3(0.0f, 1.0f, 0.0f));
		m_cam.Rotate(m_IMGUILayer->unitRotX, glm::vec3(1.0f, 0.0f, 0.0f));
		m_objects[0]->SetScale(glm::vec3(m_IMGUILayer->unitScale, m_IMGUILayer->unitScale, m_IMGUILayer->unitScale));
		//m_objects[0]->SetPosition(glm::vec3(px,py,pz));
	}

	void CommanderLayer::OnRender()
	{
		glm::mat4 projView =  *m_projection * m_cam.GetView();
		m_renderer.Clear();
		for (GameObject*& currObj : m_objects)
		{
			m_renderer.Draw(*currObj, projView, m_mainPlayer.playerNum);
		}
        m_renderer.Draw(*m_primitiveObjects[0], *m_projection * m_cam.GetView(), m_mainPlayer.playerNum);

	}
}