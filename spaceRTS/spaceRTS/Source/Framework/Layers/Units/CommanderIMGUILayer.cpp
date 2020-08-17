#include "MEpch.h"
#include "CommanderIMGUILayer.h"
#include "imgui\imgui.h"

namespace MYENGINE
{
	CommanderIMGUILayer::CommanderIMGUILayer() :ImGuiLayer("CommanderIMGUILayer")
	{

	}
	CommanderIMGUILayer::~CommanderIMGUILayer()
	{

	}
	void CommanderIMGUILayer::OnUpdateGUI(const float& deltaTime)
	{
		ImGui::Begin("Debug");
		ImGui::Text("framerate %.3f", ImGui::GetIO().Framerate);
		ImGui::ColorEdit4("clear color", clearColor);
		ImGui::SliderFloat("light x pos", &lightPosX, -100.0f, 100.0f);
		ImGui::SliderFloat("light y pos", &lightPosY, -100.0f, 100.0f);
		ImGui::SliderFloat("light z pos", &lightPosZ, -100.0f, 100.0f);
		ImGui::SliderFloat("light power", &lightPower, -1000.0f, 1000.0f);

		ImGui::SliderFloat("rot x", &unitRotX, 0.0f, 360.0f);
		ImGui::SliderFloat("rot y", &unitRotY, 0.0f, 360.0f);
		ImGui::SliderFloat("rot z", &unitRotZ, 0.0f, 360.0f);

		ImGui::SliderFloat("scale", &unitScale, 1.0f, 700.0f);

		ImGui::End();
	}
}