#pragma once
#include "Layers\IMGUILayer.h"

namespace MYENGINE
{
	class CommanderIMGUILayer : public ImGuiLayer
	{
	public:
		CommanderIMGUILayer();
		~CommanderIMGUILayer();
		void OnUpdateGUI(const float& deltaTime) override;

		float clearColor[4];
		float lightPosX = 0.0f, lightPosY = 0.0f, lightPosZ = -6.0f, lightPower = 40.0f,
			  unitRotX  = 0.0f, unitRotY  = 0.0f, unitRotZ  =  0.0f, unitScale  = 100.0f;
	};
}