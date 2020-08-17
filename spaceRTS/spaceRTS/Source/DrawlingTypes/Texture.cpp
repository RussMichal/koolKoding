#include "MEpch.h"

#include "Texture.h"
#include "stb_image/stb_image.h"
#include <glew-2.1.0/include/GL/glew.h>

namespace MYENGINE
{
	Texture::Texture(const std::string & path)
		: m_renderID(0)
		, m_filePath(path)
		, m_localBuffer(nullptr)
		, m_width(0)
		, m_height(0)
		, m_BPP(0)
		, m_numSamples(0)

	{
		stbi_set_flip_vertically_on_load(1);
		m_localBuffer = stbi_load(("..//res//assets//"+path).c_str(), &m_width, &m_height, &m_BPP, 4);

		GLCall(glGenTextures(1, &m_renderID));
		GLCall(glGetIntegerv(GL_SAMPLES, &m_numSamples));
		m_numSamples = 0;

		if (m_numSamples > 1)
		{
			GLCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_renderID));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
			GLCall(glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_numSamples, GL_RGB, m_width, m_height, GL_TRUE));
		}
		else
		{
			GLCall(glBindTexture(GL_TEXTURE_2D, m_renderID));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
			GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer));
		}

		Unbind();

		if (m_localBuffer)
		{
			stbi_image_free(m_localBuffer);
		}
	}

	Texture::~Texture()
	{
		//if (m_localBuffer)
		//{
		//	stbi_image_free(m_localBuffer);
		//}

		GLCall(glDeleteTextures(1, &m_renderID));
	}

	void Texture::Bind(unsigned int slot) const
	{

		m_textureUnitID = slot;
		GLCall(glActiveTexture(GL_TEXTURE0 + slot));
		if (m_numSamples > 1)
		{
			GLCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_renderID));
		}
		else
		{
			GLCall(glBindTexture(GL_TEXTURE_2D, m_renderID));
		}
	}

	void Texture::Unbind() const
	{
		if (m_numSamples > 1)
		{
			GLCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0));
		}
		else
		{
			GLCall(glBindTexture(GL_TEXTURE_2D, 0));
		}
	}
}