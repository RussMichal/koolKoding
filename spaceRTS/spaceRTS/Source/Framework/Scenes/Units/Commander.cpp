#include "MEpch.h"

#include "Scenes\Units\Commander.h"
#include "Drawling\GameObject.h"
#include "VertexBufferLayout.h"
#include "Scenes\Scene.h"
#include "ObjectLoading\FBXFileParser.h"
#include "ObjectLoading\OBJParser.h"
#include "ObjectLoading\AssetLoader.h"

#include "imgui\imgui.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\quaternion.hpp"
//#include <glm/gtx/quaternion.hpp>

namespace MYENGINE
{
	namespace Scene
	{
		Commander::Commander(Renderer::ShaderType sh, float cc0, float cc1, float cc2, float cc3 ):SceneSteppingStone(sh,cc0,cc1,cc2,cc3)
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
			//OBJParser o = OBJParser("Commander\\baseMod.obj");
			//FBXFileParser("test.txt");
			float projscale = 1.0f;
			m_projection = std::make_unique<glm::mat4>(glm::ortho<float>(-projscale, projscale, -projscale, projscale, -2000.0f, 2000.0f));
			//m_objects.push_back(new GameObject(o.vertexData, o.vertexIndices, "..//res//OBJModels//Textures//sh3.jpg", glm::vec4(0.1f, 0.4f, 1.0f, 1.0f)));
			m_objects.push_back(new GameObject(al.Assets[Asset::OGCommander]));
			//m_objects[0]->SetOrientation(glm::quat(glm::vec3(0.0,0.0,45.0)));
			m_objects[0]->SetScale(glm::vec3(110,110,110));
			m_objects[0]->type = ObjectType::Commander;
			glEnable(GL_DEPTH_TEST);
		}
		
		Commander::~Commander()
		{
		}

		void Commander::OnUpdate(float deltaTime)
		{
			m_renderer.SetConstUniforms(glm::vec3(x, y, z), p, 2.2f);

			m_cam.Identity();
			m_cam.Rotate(rz, glm::vec3(0.0f, 0.0f, 1.0f));
			m_cam.Rotate(ry, glm::vec3(0.0f, 1.0f, 0.0f));
			m_cam.Rotate(rx, glm::vec3(1.0f, 0.0f, 0.0f));
			m_objects[0]->SetScale(glm::vec3(s, s, s));
			//m_objects[0]->SetPosition(glm::vec3(px,py,pz));

		}

		void Commander::OnRender()
		{
			m_renderer.Clear();
			for (GameObject*& currObj : m_objects)
			{
				m_renderer.Draw(*currObj, *m_projection * m_cam.GetView());
			}
		}

		void Commander::OnImGuiRender()
		{
			ImGui::Begin("Debug");
			ImGui::ColorEdit4("clear color", m_renderer.clearColor);
			ImGui::SliderFloat("light x pos", &x, -100.0f, 100.0f);
			ImGui::SliderFloat("light y pos", &y, -100.0f, 100.0f);
			ImGui::SliderFloat("light z pos", &z, -100.0f, 100.0f);            
			ImGui::SliderFloat("light power", &p, -1000.0f, 1000.0f);

			ImGui::SliderFloat("rot x", &rx, 0.0f, 360.0f);
			ImGui::SliderFloat("rot y", &ry, 0.0f, 360.0f);
			ImGui::SliderFloat("rot z", &rz, 0.0f, 360.0f);

			ImGui::SliderFloat("pos x", &px, 0.0f, 360.0f);
			ImGui::SliderFloat("pos y", &py, 0.0f, 360.0f);
			ImGui::SliderFloat("pos z", &pz, -500.0f, 500.0f);
			ImGui::SliderFloat("scale", &s, 1.0f, 700.0f);

			ImGui::End();
		}
	};
}