#pragma once
#pragma once

namespace MYENGINE
{
	class IndexBuffer
	{
	private:
		unsigned int m_rendererId;
		unsigned int m_count;
	public:
		IndexBuffer(const unsigned int * data, unsigned int count);
		~IndexBuffer();
		//IndexBuffer(IndexBuffer&& rhs) noexcept { std::cout << "IndexBuffer move constructor" << std::endl; }

		void Bind()const;
		void Unbind()const;

		inline unsigned int GetCount() const { return m_count; }
	};
}