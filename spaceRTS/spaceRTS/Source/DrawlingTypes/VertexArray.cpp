#include "MEpch.h"

#include "VertexArray.h"
#include "Drawling\Renderer.h"
#include "VertexBufferLayOut.h"

namespace MYENGINE
{
	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &m_rendererId);
	}

	VertexArray::~VertexArray()
	{
		GLCall(glDeleteVertexArrays(1, &m_rendererId));
	}

	void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
	{
		vb.Bind();
		Bind();

		const std::vector<VertexBufferElement>& elements = layout.GetElements();

		unsigned int offset = 0;
		for (unsigned int i = 0; i < elements.size(); i++)
		{
			const VertexBufferElement& element = elements[i];

			//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
			//glEnableVertexAttribArray(0);
			GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset));
			GLCall(glEnableVertexAttribArray(i));

			offset += element.count * element.GetSizeOfType(element.type);

		}
		vb.Unbind();
	}


	void VertexArray::Bind() const
	{
		GLCall(glBindVertexArray(m_rendererId));
	}

	void VertexArray::Unbind() const
	{
		GLCall(glBindVertexArray(0));
	}
}