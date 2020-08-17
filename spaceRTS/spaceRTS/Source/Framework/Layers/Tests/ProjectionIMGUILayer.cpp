#include "MEpch.h"
#include "Layers/Tests/ProjectionIMGUILayer.h"
#include "imgui\imgui.h"

namespace MYENGINE
{
	ProjectionIMGUILayer::ProjectionIMGUILayer() :ImGuiLayer("ProjectionIMGUILayer")
	{

	}
	ProjectionIMGUILayer::~ProjectionIMGUILayer()
	{

	}
	void ProjectionIMGUILayer::OnUpdateGUI(const float& deltaTime)
	{
		ImGui::Begin("Debug");
		ImGui::Text("framerate %.3f", ImGui::GetIO().Framerate);
		ImGui::ColorEdit4("clear color", clearColor);
		ImGui::SliderFloat("cam Rot X", &camRotX, 0.0f, 360.0f);
		ImGui::SliderFloat("cam Rot Y", &camRotY, 0.0f, 360.0f);
		ImGui::SliderFloat("cam Rot Z", &camRotZ, 0.0f, 360.0f);
		ImGui::SliderFloat("unit Rot Z", &unitRotZ, 100, 201);
		if (ImGui::Button("set cam rot"))
		{
			camRotX = 60.0f;
			camRotY = 0.0f;
			camRotZ = 45.0f;
			unitRotZ = 0.0f;
		}
		ImGui::End();
	}
}