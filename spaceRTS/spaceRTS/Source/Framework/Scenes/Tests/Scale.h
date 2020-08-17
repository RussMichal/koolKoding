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
		class Scale : public SceneSteppingStone
		{
		public:
			Scale(Renderer::ShaderType sh = Renderer::ShaderType::Basic, float cc0 = 0.0f, float cc1 = 0.0f, float cc2 = 0.0f, float cc3 = 1.0f);
			~Scale();

			void OnUpdate(float deltaTime) override;
			void OnRender() override;
			void OnImGuiRender() override;

		private:
			float camRotX = 0, camRotY = 0, camRotZ = 0, unitRotZ=0, bullitScale =1, unitScale =10;
			bool ch = false;

		};
	}
}