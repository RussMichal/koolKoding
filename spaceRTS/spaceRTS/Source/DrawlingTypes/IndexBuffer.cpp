#include "MEpch.h"
#include "IndexBuffer.h"
#include "Drawling\Renderer.h"

namespace MYENGINE
{
	IndexBuffer::IndexBuffer(const unsigned int * data, unsigned int count)
	{
		GLCall(glGenBuffers(1, &m_rendererId));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_DYNAMIC_DRAW));//if unsigned int == GLUint
	}

	IndexBuffer::~IndexBuffer()
	{
		GLCall(glDeleteBuffers(1, &m_rendererId));
	}

	void IndexBuffer::Bind() const
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId));
	}

	void IndexBuffer::Unbind() const
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}
}