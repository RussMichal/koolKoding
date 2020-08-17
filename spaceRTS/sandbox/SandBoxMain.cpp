#include <Engine.h>
#include "Framework\ObjectLoading\OBJParser.h"
#include "Framework\ObjectLoading\FBXFileParser.h"

class sandbox : public MYENGINE::Application
{
public:
	sandbox() {}
	~sandbox() {}
};

MYENGINE::GameObject* o=nullptr;
class sandBoxScene: public MYENGINE::Scene::SceneSteppingStone
{
private:
	float x = 0, y = 0, z = -6, p = 40, ux=0, uy = 0;
public:
	sandBoxScene(MYENGINE::Renderer::ShaderType sh = MYENGINE::Renderer::ShaderType::Basic, float cc0=0.0f, float cc1 = 0.0f, float cc2 = 0.0f, float cc3 = 1.0f) :SceneSteppingStone(sh, cc0, cc1, cc2, cc3)
	{
		std::vector<float> vertices =
		{	//vertices				tex coord		normals
			-1.0f, -1.0f, 0.0f,     0.0f, 0.0f,     0.2f,0.0f,-0.2f, // Left
			 1.0f, -1.0f, 0.0f,     1.0f, 0.0f,    -0.2f,0.0f,-0.2f, // Right
			 0.0f,  1.0f, 0.0f,     0.5f, 1.0f,     0.0f,0.2f,-0.2f  // Top
		};

		std::vector<unsigned int> indies =
		{
			0,1,2
		};
		//MYENGINE::OBJParser("Sample_Ship.obj");
		//MYENGINE::FBXFileParser("test.txt");
		o = new MYENGINE::GameObject(vertices, indies, "..//res//Textures//Metal.png", glm::vec4(1.0f, 0.4f, 0.1f, 1.0f));

		m_objects.push_back(new MYENGINE::GameObject(vertices, indies, "..//res//Textures//Metal.png", glm::vec4(1.0f, 0.4f, 0.1f, 1.0f)));
		m_objects.push_back(new MYENGINE::GameObject(vertices, indies, "..//res//Textures//Metal.png", glm::vec4(1.0f, 0.4f, 0.1f, 1.0f)));
		m_objects.push_back(new MYENGINE::GameObject(vertices, indies, "..//res//Textures//Metal.png", glm::vec4(1.0f, 0.4f, 0.1f, 1.0f)));
		m_objects.push_back(new MYENGINE::GameObject(vertices, indies, "..//res//Textures//Metal.png", glm::vec4(1.0f, 0.4f, 0.1f, 1.0f)));

		float scale = 40.0f;
		m_objects[0]->SetScale(glm::vec3(scale, scale, 1.0f));
		m_objects[1]->SetScale(glm::vec3(scale, scale, 1.0f));
		m_objects[2]->SetScale(glm::vec3(scale, scale, 1.0f));
		m_objects[3]->SetScale(glm::vec3(scale, scale, 1.0f));

		m_objects[1]->SetPosition(glm::vec3(50.0f, 50.0f,0.0f));
		m_objects[2]->SetPosition(glm::vec3(100.0f, 50.0f,0.0f));
		m_objects[3]->SetPosition(glm::vec3(350.0f, 50.0f,0.0f));

	}

	~sandBoxScene()
	{
	}

	void OnUpdate(float deltaTime)
	{
		m_renderer.SetConstUniforms(glm::vec3(x, y, z), p, 22.0f);

		m_objects[0]->SetPosition(glm::vec3(ux, uy, 0.0f));

	}

	void OnRender()
	{
		m_renderer.Clear();
		for (MYENGINE::GameObject*& currObj : m_objects)
		{
			m_renderer.Draw(*currObj, *m_projection * m_cam.GetView(), MYENGINE::PlayerNumber::Player1);
		}
	}

	void OnImGuiRender()
	{
		//should we include in client or wrap in engine?
		ImGui::Begin("Debug");
		ImGui::ColorEdit4("clear color", m_renderer.clearColor);
		ImGui::SliderFloat("light x pos", &x, -100.0f, 100.0f);
		ImGui::SliderFloat("light y pos", &y, -100.0f, 100.0f);
		ImGui::SliderFloat("light z pos", &z, -100.0f, 100.0f);
		ImGui::SliderFloat("light power", &p, -1000.0f, 1000.0f);
		ImGui::SliderFloat("unit x pos", &ux, 0.0f, 426.0f);
		ImGui::SliderFloat("unit y pos", &uy, 0.0f, 240.0f);

		ImGui::End();
	}
};

MYENGINE::Application* MYENGINE::CreateApp()
{
	auto box = new sandbox();
	//box->AppRegisterScene<sandBoxScene>("sandbox");
	//box->Run();
	return box;
}