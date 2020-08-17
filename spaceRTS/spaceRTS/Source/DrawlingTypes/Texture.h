#pragma once
#include "Framework\GLCall.h"

namespace MYENGINE
{
	class Texture
	{
	private:
		unsigned int m_renderID;
		std::string m_filePath;
		unsigned char* m_localBuffer;
		int m_width;
		int m_height;
		int m_BPP;
		int m_numSamples;
		mutable unsigned int m_textureUnitID;

	public:
		Texture(const std::string& path);
		~Texture();
		//Texture(Texture&& rhs) noexcept { std::cout << "Texture move constructor" << std::endl; }

		void Bind(unsigned int slot = 0) const;
		void Unbind() const;

		inline int GetWidth() const { return m_width; }
		inline int Getheight() const { return m_height; }
		inline unsigned int GetTextureUnitID() const { return m_textureUnitID; }
	};
}