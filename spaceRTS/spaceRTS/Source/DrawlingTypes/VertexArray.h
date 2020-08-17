#pragma once

#include "VertexBuffer.h"

namespace MYENGINE
{
	class VertexBufferLayout;
	class VertexArray
	{
	private:
		unsigned int m_rendererId;
	public:
		VertexArray();
		~VertexArray();
		//VertexArray(VertexArray&& rhs) noexcept { std::cout << "VertexArray move constructor" << std::endl; }

		void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
		void SetUpBuffer();
		void Bind() const;
		void Unbind() const;
	};
}