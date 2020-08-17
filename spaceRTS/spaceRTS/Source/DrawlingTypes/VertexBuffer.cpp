#include "MEpch.h"

#include "VertexBuffer.h"
#include "Drawling\Renderer.h"

namespace MYENGINE
{
	VertexBuffer::VertexBuffer(const void * data, unsigned int size)
	{
		float* xx = (float*)data;
		for (int i = 0; i < 16; i++)
		{
			float x = *(xx + i);
			int s=4;
		}
		GLCall(glGenBuffers(1, &m_rendererId));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_rendererId));
		GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW));
	}

	VertexBuffer::~VertexBuffer()
	{
		GLCall(glDeleteBuffers(1, &m_rendererId));
	}

	void VertexBuffer::Bind() const
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_rendererId));
	}

	void VertexBuffer::Unbind() const
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}
}