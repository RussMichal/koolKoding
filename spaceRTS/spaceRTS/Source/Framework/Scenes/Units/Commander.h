#pragma once
#include "Scenes\Scene.h"

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
	namespace Scene
	{
		class Commander : public SceneSteppingStone
		{
		public:
			Commander(Renderer::ShaderType sh = Renderer::ShaderType::Basic, float cc0=0.0f, float cc1 = 0.0f, float cc2 = 0.0f, float cc3 = 1.0f);
			~Commander();

			void OnUpdate(float deltaTime) override;
			void OnRender() override;
			void OnImGuiRender() override;
		private:
			float x = 0, y = 0, z = -6, p = 40;
			float rx = 0, ry = 0, rz = 0;
			float px = 0, py = 0, pz = 0;
			float s = 1;
		};
	}
}