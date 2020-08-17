#include "MEpch.h"

#include "Scenes\Maps\Map.h"
#include "Drawling\Object.h"
#include "VertexBufferLayout.h"
#include "Scenes\Scene.h"

#include "imgui\imgui.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\quaternion.hpp"
//#include <glm/gtx/quaternion.hpp>

namespace MYENGINE
{
	namespace Scene
	{
		Map::Map(Renderer::ShaderType sh, float cc0, float cc1, float cc2, float cc3) :SceneSteppingStone(sh, cc0, cc1, cc2, cc3)
		{
			std::vector<float> vertices =
			{	//vertices				tex coord		normals
				-0.5f, -0.5f, 0.0f,     0.0f, 0.0f,     0.2f,0.0f,-0.2f, // Lower Left
				 0.5f, -0.5f, 0.0f,     1.0f, 0.0f,    -0.2f,0.0f,-0.2f, // Lower Right
				 0.5f,  0.5f, 0.0f,     1.0f, 1.0f,     0.0f,0.2f,-0.2f,  // Top Right
				-0.5f,  0.5f, 0.0f,     0.0f, 1.0f,     0.0f,0.2f,-0.2f  // Top Left
			};

			std::vector<unsigned int> indies =
			{
				0,1,2,
				0,2,3
			};

			std::vector<float> verticesTri =
			{	//vertices				tex coord		normals
				-0.5f, -0.5f, 0.0f,     0.0f, 0.0f,     0.2f,0.0f,-0.2f, // Left
				 0.5f, -0.5f, 0.0f,     1.0f, 0.0f,    -0.2f,0.0f,-0.2f, // Right
				 0.0f,  0.5f, 0.0f,     0.5f, 1.0f,     0.0f,0.2f,-0.2f  // Top
			};

			std::vector<unsigned int> indiesTri =
			{
				0,1,2
			};
			float projscale = 4.0f;
			float laDist = (float)sqrt(1 / 3);
			m_projection = std::make_unique<glm::mat4>(glm::ortho<float>(-projscale, projscale, -projscale, projscale, laDist -2.0f, laDist +2.0f));
			//m_cam.LookAt(glm::vec3(laDist, laDist, laDist), glm::vec3(0,0,0), glm::vec3(0.0f,1.0f,0.0f));
			//m_cam.Rotate(45.0,glm::vec3(0.0f, 1.0f, 0.0f));

			m_objects.push_back(new GameObject(vertices, indies, "..//res//Textures//Metal.png", glm::vec4(1.0f, 0.4f, 0.1f, 1.0f)));
			m_objects.push_back(new GameObject(vertices, indies, "..//res//Textures//Metal.png", glm::vec4(0.1f, 0.4f, 1.0f, 1.0f)));
			m_objects.push_back(new GameObject(vertices, indies, "..//res//Textures//Metal.png", glm::vec4(0.1f, 0.4f, 1.0f, 1.0f)));
			m_objects.push_back(new GameObject(vertices, indies, "..//res//Textures//Metal.png", glm::vec4(1.0f, 0.4f, 0.1f, 1.0f)));
			m_objects.push_back(new GameObject(verticesTri, indiesTri, "..//res//Textures//Metal.png", glm::vec4(0.1f, 1.0f, 0.4f, 1.0f)));

			/*m_objects[0]->m_position = glm::vec3(-0.5f,  0.5f, 0.0f);//top lelf
			m_objects[1]->m_position = glm::vec3( 0.5f,  0.5f, 0.0f);//top right
			m_objects[2]->m_position = glm::vec3(-0.5f, -0.5f, 0.0f);//lower lelft
			m_objects[3]->m_position = glm::vec3( 0.5f, -0.5f, 0.0f);//lower right*/

			//m_objects[0].m_orientation = glm::quat(glm::vec3(0.0, 0.0, 45.0));
		}

		Map::~Map()
		{
		}

		void Map::OnUpdate(float deltaTime)
		{
			float laDist = (float)sqrt(1 / 3);
			//m_objects.back()->m_orientation = glm::quat(glm::vec3(0.0f,0.0f, glm::radians(unitRotZ)));

			m_cam.Identity();
			//m_cam.Translate(glm::vec3(0, 0, -1));
			m_cam.Rotate(camRotZ, glm::vec3(0.0f, 0.0f, 1.0f));
			m_cam.Rotate(camRotY, glm::vec3(0.0f, 1.0f, 0.0f));
			m_cam.Rotate(camRotX, glm::vec3(1.0f, 0.0f, 0.0f));
			//m_cam.Translate(glm::vec3(0, 0, 1));
		}

		void Map::OnRender()
		{
			m_renderer.Clear();
			for (GameObject*& currObj : m_objects)
			{
				m_renderer.Draw(*currObj, *m_projection * m_cam.GetView());
			}
		}

		void Map::OnImGuiRender()
		{
			ImGui::Begin("Debug");
			ImGui::ColorEdit4("clear color", m_renderer.clearColor);
			ImGui::SliderFloat("light x pos", &x, -100.0f, 100.0f);
			ImGui::SliderFloat("light y pos", &y, -100.0f, 100.0f);
			ImGui::SliderFloat("light z pos", &z, -100.0f, 100.0f);
			ImGui::SliderFloat("light power", &p, -1000.0f, 1000.0f);
			ImGui::SliderFloat("cam Rot X", &camRotX, 0.0f, 360.0f);
			ImGui::SliderFloat("cam Rot Y", &camRotY, 0.0f, 360.0f);
			ImGui::SliderFloat("cam Rot Z", &camRotZ, 0.0f, 360.0f);
			ImGui::SliderFloat("unit Rot Z", &unitRotZ, 0.0f, 360.0f);
			if (ImGui::Button("set cam rot"))
			{
				camRotX = 60.0f;
				camRotY =  0.0f;
				camRotZ = 45.0f;
				unitRotZ = 0.0f;
			}
			ImGui::End();
		}
	};
}
