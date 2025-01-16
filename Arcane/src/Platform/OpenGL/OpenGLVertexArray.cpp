#include <arcpch.h>

#include "Platform/OpenGL/OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Arcane
{
	static GLenum ShaderTypeToGLType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::None:	 return GL_NONE;
		case ShaderDataType::Int:	 return GL_INT;
		case ShaderDataType::Int2:	 return GL_INT;
		case ShaderDataType::Int3:	 return GL_INT;
		case ShaderDataType::Int4:	 return GL_INT;
		case ShaderDataType::Float:  return GL_FLOAT;
		case ShaderDataType::Float2: return GL_FLOAT;
		case ShaderDataType::Float3: return GL_FLOAT;
		case ShaderDataType::Float4: return GL_FLOAT;
		case ShaderDataType::Mat3:	 return GL_FLOAT;
		case ShaderDataType::Mat4:	 return GL_FLOAT;
		case ShaderDataType::Bool:	 return GL_BOOL;
		}

		ARC_CORE_ASSERT(false, "Unknown Data Type!");
		return GL_NONE;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		ARC_PROFILE_FUNCTION();
		glCreateVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		ARC_PROFILE_FUNCTION();
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		ARC_PROFILE_FUNCTION();
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		ARC_PROFILE_FUNCTION();
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		ARC_PROFILE_FUNCTION();
		Bind();
		vertexBuffer->Bind();

		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.GetComponentCount(), ShaderTypeToGLType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)((uint64_t) element.Offset));
			index++;
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		ARC_PROFILE_FUNCTION();
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();
		m_IndexBuffer = indexBuffer;
	}
}