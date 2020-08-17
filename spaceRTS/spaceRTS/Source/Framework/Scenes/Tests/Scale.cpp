#include "MEpch.h"

#include "Scenes\Tests\Scale.h"
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
	namespace Scene
	{
		Scale::Scale(Renderer::ShaderType sh, float cc0, float cc1, float cc2, float cc3) :SceneSteppingStone(sh, cc0, cc1, cc2, cc3)
		{
			//unit to built 10 to 1. projection scale 

			float bullitScale = 1;
			//float unitScale = bullitScale*5;

			float projScale = m_windowHeight /(1080/350);

			std::vector<float> verticesBullit =
			{	//vertices				tex coord		normals
				-bullitScale, -bullitScale, 0.0f,     0.0f, 0.0f,     0.2f,0.0f,-0.2f, // Left
				 bullitScale, -bullitScale, 0.0f,     1.0f, 0.0f,    -0.2f,0.0f,-0.2f, // Right
				 0.0f,  bullitScale, 0.0f,     0.5f, 1.0f,     0.0f,0.2f,-0.2f  // Top
			};
			std::vector<float> verticesUnit =
			{	//vertices				tex coord		normals
				-unitScale, -unitScale, 0.0f,     0.0f, 0.0f,     0.2f,0.0f,-0.2f, // Left
				 unitScale, -unitScale, 0.0f,     1.0f, 0.0f,    -0.2f,0.0f,-0.2f, // Right
				 0.0f,  unitScale, 0.0f,     0.5f, 1.0f,     0.0f,0.2f,-0.2f  // Top
			};
			std::vector<unsigned int> indies =
			{
				0,1,2
			};
			//m_projection = std::make_unique<glm::mat4>(glm::ortho<float>(-projScale*16/9, projScale*16/9, -projScale, projScale, -200.0f, 200.0f));
			//m_cam.LookAt(glm::vec3(laDist, laDist, laDist), glm::vec3(0,0,0), glm::vec3(0.0f,1.0f,0.0f));
			//m_cam.Rotate(45.0,glm::vec3(0.0f, 1.0f, 0.0f));

			m_objects.push_back(new GameObject(verticesBullit, indies, "..//res//Textures//Metal.png", glm::vec4(0.1f, 0.4f, 1.0f, 1.0f)));
			m_objects.push_back(new GameObject(verticesBullit, indies, "..//res//Textures//Metal.png", glm::vec4(0.1f, 1.0f, 0.4f, 1.0f)));
			m_objects.push_back(new GameObject(verticesBullit, indies, "..//res//Textures//Metal.png", glm::vec4(1.0f, 0.1f, 0.4f, 1.0f)));

			m_objects[0]->SetPosition(glm::vec3( 0.0f,  15.0f, 0.0f));
			m_objects[1]->SetPosition(glm::vec3( 0.0f, -15.0f, 0.0f));

			m_objects[2]->SetPosition(glm::vec3(200.0f, -100.0f, 0.0f));
			m_objects[2]->SetScale(glm::vec3(40.0f, 40.0f, 1.0f));


			//m_objects[0].m_orientation = glm::quat(glm::vec3(0.0, 0.0, 45.0));
		}

		Scale::~Scale()
		{
		}

		void Scale::OnUpdate(float deltaTime)
		{
			m_objects.back()->SetOrientation(glm::quat(glm::vec3(0.0f,0.0f, glm::radians(unitRotZ))));

			m_cam.Identity();
			//m_cam.Translate(glm::vec3(0, 0, -1));
			m_cam.Rotate(camRotZ, glm::vec3(0.0f, 0.0f, 1.0f));
			m_cam.Rotate(camRotY, glm::vec3(0.0f, 1.0f, 0.0f));
			m_cam.Rotate(camRotX, glm::vec3(1.0f, 0.0f, 0.0f));
			//m_cam.Translate(glm::vec3(0, 0, 1));
		}

		void Scale::OnRender()
		{
			m_renderer.Clear();
			for (GameObject*& currObj : m_objects)
			{
				m_renderer.Draw(*currObj, *m_projection * m_cam.GetView());
			}
		}

		void Scale::OnImGuiRender()
		{
			ImGui::Begin("Debug");
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
			ImGui::SliderFloat("scale unit", &unitScale, 20.0f, 30.0f);
			ImGui::SliderFloat("scale bullit", &bullitScale, 1.0f, 3.0f);

			m_objects[0]->SetScale(glm::vec3(bullitScale, bullitScale,1));
			m_objects[1]->SetScale(glm::vec3(unitScale, unitScale, 1));

			ImGui::Checkbox("selected", &ch);
			if (ch)
			{
				m_objects[1]->m_isSelected = true;
			}
			else
			{
				m_objects[1]->m_isSelected = false;
			}
			ImGui::End();
		}
	};
}
