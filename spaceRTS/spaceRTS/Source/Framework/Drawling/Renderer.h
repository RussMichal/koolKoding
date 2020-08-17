#pragma once
// GLEW
#define GLEW_STATIC

#include "DrawlingTypes\Shaders.h"
#include "Vender\glm\glm.hpp"
#include "Framework\Drawling\GameObject.h"
#include "Core.h"

namespace MYENGINE
{
	class Renderer
	{
	public:
		enum MYENGINE_API ShaderType
		{
			Basic
		};
		Renderer(const ShaderType shader= ShaderType::Basic, float cc1 = 0.0f, float cc2 = 0.0f, float cc3 = 0.0f, float cc4 = 1.0f);
		void MYENGINE_API Draw(const GameObject& obj, const glm::mat4& projectionView, const PlayerNumber& mainPlayer);
		void MYENGINE_API Draw(const PrimitiveObject& obj, const glm::mat4& projectionView, const PlayerNumber& mainPlayer);
		void MYENGINE_API SetConstUniforms(glm::vec3 lightPos, float lightPower, float gamma);
		void SetBlend();
		void SetDrawBuffers(const unsigned int buffers) const;
		void MYENGINE_API Clear() const;

		Shaders GetShader(ShaderType shaderID);
		void SetShader(ShaderType shaderID);

		float clearColor[4];
	private:
        enum ModelMode
        {
            All,
            None,
            NoScale
        };
		void SetObjectUniforms(const Object& obj);
		void SetObjectMVP(const PrimitiveObject& obj, const glm::mat4& projectionView, const ModelMode& modelUsage = ModelMode::All);

		std::unique_ptr<Shaders> m_shader;
		glm::mat4 GetNormalMatrix(glm::mat4 mv);
	};
}