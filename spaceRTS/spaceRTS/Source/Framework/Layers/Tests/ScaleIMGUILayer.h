#pragma once
#include "Layers\IMGUILayer.h"

namespace MYENGINE
{
	class ScaleIMGUILayer : public ImGuiLayer
	{
	public:
		ScaleIMGUILayer();
		~ScaleIMGUILayer();
		void OnUpdateGUI(const float& deltaTime) override;

		float clearColor[4];
		float camRotX = 0,  camRotY = 0,     camRotZ = 0, 
			  unitRotZ = 0, bullitScale = 1, unitScale = 10;
	};
}