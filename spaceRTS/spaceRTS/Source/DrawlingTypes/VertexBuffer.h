#pragma once

namespace MYENGINE
{
	class VertexBuffer
	{
	private:
		unsigned int m_rendererId;
	public:
		VertexBuffer(const void * data, unsigned int size);
		~VertexBuffer();
		//VertexBuffer(VertexBuffer&& rhs) noexcept { std::cout << "VertexBuffer move constructor" << std::endl; }

		void Bind() const;
		void Unbind() const;
	};
}