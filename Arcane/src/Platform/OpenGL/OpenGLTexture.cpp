#include <arcpch.h>

#include "Platform/OpenGL/OpenGLTexture.h"

#include <stb_image.h>

namespace Arcane
{
	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height) :
		m_Width(width), m_Height(height), m_InternalFormat(GL_RGBA8), m_DataFormat(GL_RGBA)
	{
		ARC_PROFILE_FUNCTION();
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path) :
		m_Path(path), m_InternalFormat(GL_RGBA8), m_DataFormat(GL_RGBA)
	{
		ARC_PROFILE_FUNCTION();
		int width, height, channels;
		stbi_set_flip_vertically_on_load(true); 
		stbi_uc* data = nullptr;
		{
			ARC_PROFILE_SCOPE("stbi_load - OpenGLTexture2D::OpenGLTexture2D(const std::string&)");
			data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		}
		ARC_CORE_ASSERT(data, "Failed to load image!");

		m_Width = width;
		m_Height = height;

		m_InternalFormat = channels > 3 ? GL_RGBA8 : GL_RGB8;
		m_DataFormat = channels > 3 ? GL_RGBA : GL_RGB;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		ARC_PROFILE_FUNCTION();
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		ARC_PROFILE_FUNCTION();
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		ARC_PROFILE_FUNCTION();
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}
}