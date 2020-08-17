#pragma once
#include "Layers\IMGUILayer.h"

namespace MYENGINE
{
	class ProjectionIMGUILayer : public ImGuiLayer
	{
	public:
		ProjectionIMGUILayer();
		~ProjectionIMGUILayer();
		void OnUpdateGUI(const float& deltaTime) override;

		float clearColor[4];
		float camRotX = 0, camRotY = 0, camRotZ = 0, 
			  unitRotZ = 0;
	};
}