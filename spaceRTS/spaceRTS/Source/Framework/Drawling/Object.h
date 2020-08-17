#pragma once

//#include "Shaders.h"

#include "DrawlingTypes\VertexBuffer.h"
#include "DrawlingTypes\VertexArray.h"
#include "DrawlingTypes\IndexBuffer.h"
#include "DrawlingTypes\Texture.h"
#include "DrawlingTypes\VertexBufferLayout.h"
#include "Vender\glm\gtc\quaternion.hpp"
#include "Core.h"
#include "Vender\glm\glm.hpp"

#include <memory.h>
#include <vector>

namespace MYENGINE
{
	template struct MYENGINE_API glm::vec<3, float, glm::qualifier::defaultp>;
	template struct MYENGINE_API glm::vec<4, float, glm::qualifier::defaultp>;
	template struct MYENGINE_API glm::qua<float, glm::qualifier::defaultp>;

	class MYENGINE_API Object
	{
	public:
		Object(const std::vector<float> vertices, const std::vector<unsigned int> indies, const std::string& texture, const glm::vec4& color, GLenum drawMode = GL_TRIANGLES);
		~Object();
		//Object(Object&& rhs) noexcept { std::cout << "object move constructor" << std::endl; }

		void Render();
		void Bind() const;
		unsigned int GetElements() const;
		unsigned int GetTextureID() const;

		glm::vec4 m_color;
		float m_shininess;
		GLenum m_drawMode;

	private:
		VertexBuffer m_vb;
		VertexArray m_va;
		IndexBuffer m_ib;
		Texture m_tex;
		unsigned int m_elements;
	};
}
