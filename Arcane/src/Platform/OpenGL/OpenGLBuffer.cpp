#include <arcpch.h>

#include <glad/glad.h>

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Arcane
{
#pragma region Vertex Buffer

	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
	{
		ARC_PROFILE_FUNCTION();
		glCreateBuffers(1, &m_RendererId);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
	{
		ARC_PROFILE_FUNCTION();
		glCreateBuffers(1, &m_RendererId);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(const void* data, uint32_t size)
	{
		ARC_PROFILE_FUNCTION();
		glCreateBuffers(1, &m_RendererId);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		ARC_PROFILE_FUNCTION();
		glDeleteBuffers(1, &m_RendererId);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		ARC_PROFILE_FUNCTION();
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
	}

	void OpenGLVertexBuffer::Unbind() const
	{
		ARC_PROFILE_FUNCTION();
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLVertexBuffer::SetData(const void* data, uint32_t size, bool dynamic)
	{
		ARC_PROFILE_FUNCTION();
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
		glBufferData(GL_ARRAY_BUFFER, size, data, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
	}

	void OpenGLVertexBuffer::SetSubData(const void* data, uint32_t size)
	{
		ARC_PROFILE_FUNCTION();
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

#pragma endregion

#pragma region Shader Storage Buffer

	OpenGLSSBuffer::OpenGLSSBuffer(uint32_t size, uint32_t binding)
	{
		ARC_PROFILE_FUNCTION();
		glGenBuffers(1, &m_RendererID);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID);
		glBufferData(GL_SHADER_STORAGE_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, m_RendererID);
	}

	OpenGLSSBuffer::OpenGLSSBuffer(uint32_t size, const void* data, uint32_t binding)
	{
		ARC_PROFILE_FUNCTION();
		glGenBuffers(1, &m_RendererID);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID);
		glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, m_RendererID);
	}

	OpenGLSSBuffer::~OpenGLSSBuffer()
	{
		ARC_PROFILE_FUNCTION();
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLSSBuffer::Bind(uint32_t binding) const
	{
		ARC_PROFILE_FUNCTION();
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, m_RendererID);
	}

	void OpenGLSSBuffer::Unbind() const
	{
		ARC_PROFILE_FUNCTION();
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}

	void OpenGLSSBuffer::SetData(const void* data, uint32_t size, uint32_t offset)
	{
		ARC_PROFILE_FUNCTION();
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID);
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, size, data);
	}

	void* OpenGLSSBuffer::MapBuffer(uint32_t access)
	{
		ARC_PROFILE_FUNCTION();
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID);
		return glMapBuffer(GL_SHADER_STORAGE_BUFFER, access);
	}

	void OpenGLSSBuffer::UnmapBuffer()
	{
		ARC_PROFILE_FUNCTION();
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	}

#pragma endregion

#pragma region Index Buffer

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count) :
		m_Count(count)
	{
		ARC_PROFILE_FUNCTION();
		glCreateBuffers(1, &m_RendererId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		ARC_PROFILE_FUNCTION();
		glDeleteBuffers(1, &m_RendererId);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		ARC_PROFILE_FUNCTION();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId);
	}

	void OpenGLIndexBuffer::Unbind() const
	{
		ARC_PROFILE_FUNCTION();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

#pragma endregion
}