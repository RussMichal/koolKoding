#include "MEpch.h"
#include "Layers/Tests/ScaleIMGUILayer.h"
#include "imgui\imgui.h"

namespace MYENGINE
{
	ScaleIMGUILayer::ScaleIMGUILayer() :ImGuiLayer("ScaleIMGUILayer")
	{

	}
	ScaleIMGUILayer::~ScaleIMGUILayer()
	{

	}
	void ScaleIMGUILayer::OnUpdateGUI(const float& deltaTime)
	{
		ImGui::Begin("Debug");
		ImGui::Text("framerate %.3f",ImGui::GetIO().Framerate);
		ImGui::SliderFloat("cam Rot X", &camRotX, 0.0f, 360.0f);
		ImGui::SliderFloat("cam Rot Y", &camRotY, 0.0f, 360.0f);
		ImGui::SliderFloat("cam Rot Z", &camRotZ, 0.0f, 360.0f);
		ImGui::SliderFloat("unit Rot Z", &unitRotZ, 0.0f, 360.0f);
		if (ImGui::Button("set cam rot"))
		{
			camRotX = 60.0f;
			camRotY = 0.0f;
			camRotZ = 45.0f;
			unitRotZ = 0.0f;
		}
		ImGui::SliderFloat("scale unit", &unitScale, 20.0f, 30.0f);
		ImGui::SliderFloat("scale bullit", &bullitScale, 1.0f, 3.0f);

		ImGui::End();
	}
}