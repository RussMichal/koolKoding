#pragma once
#include "Layers\WorldLayerBase.h"
#include "Layers\Tests\CombatIMGUILayer.h"

#include "Shaders.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"

#include <memory.h>
#include <vector>

namespace MYENGINE
{
	class CombatLayer : public WorldLayerBase
	{
	public:
        CombatLayer(const Player& mainPlayer, Renderer::ShaderType sh = Renderer::ShaderType::Basic, float cc0=0.0f, float cc1 = 0.0f, float cc2 = 0.0f, float cc3 = 1.0f);
		~CombatLayer();

		void OnUpdate(const float& deltaTime) override;
		void OnRender() override;
		//void OnImGuiRender() override;
	private:
        CombatIMGUILayer* m_IMGUILayer;
	};
}