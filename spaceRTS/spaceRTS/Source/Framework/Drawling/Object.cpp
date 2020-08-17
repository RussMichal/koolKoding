#include "MEpch.h"
#include "Object.h"

#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "VertexBufferLayout.h"

namespace MYENGINE
{
	Object::Object(const std::vector<float> vertices, const std::vector<unsigned int> indies, const std::string& texture, const glm::vec4& m_color, GLenum drawMode)
		: m_vb(VertexBuffer(&vertices[0], vertices.size() * sizeof(float)))
		, m_ib(IndexBuffer(&indies[0], indies.size()))
		, m_tex(Texture(texture))
		, m_va(VertexArray())
		, m_color(m_color)
		, m_shininess(0.5f)
		, m_elements(indies.size())
		, m_drawMode(drawMode)
	{
		VertexBufferLayout vbl;
		vbl.Push<float>(3);
		vbl.Push<float>(2);
		vbl.Push<float>(3);

		m_va.AddBuffer(m_vb, vbl);
		//SetUniforms();
	}

	Object::~Object()
	{
		m_vb.Unbind();
		m_ib.Unbind();
		m_tex.Unbind();
		m_va.Unbind();
	}

	void Object::Bind() const
	{
		m_va.Bind();
		m_ib.Bind();
		m_tex.Bind();
	}

	unsigned int Object::GetElements() const
	{
		return m_elements;
	}

	unsigned int Object::GetTextureID() const
	{
		return m_tex.GetTextureUnitID();
	}

	void Object::Render()
	{

	}


}