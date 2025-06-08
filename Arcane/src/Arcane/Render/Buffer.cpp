#include <arcpch.h>

#include "Arcane/Render/Buffer.h"
#include "Arcane/Render/RenderAPI.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Arcane
{
	Shared<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (RenderAPI::GetAPI())
		{
		case RenderAPI::API::None: ARC_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
		case RenderAPI::API::OpenGL: return CreateShared<OpenGLVertexBuffer>(size);
		}

		ARC_CORE_ASSERT(false, "Unknown API selected!");
		return nullptr;
	}
	Shared<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (RenderAPI::GetAPI())
		{
		case RenderAPI::API::None: ARC_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
		case RenderAPI::API::OpenGL: return CreateShared<OpenGLVertexBuffer>(vertices, size);
		}

		ARC_CORE_ASSERT(false, "Unknown API selected!");
		return nullptr;
	}

	Shared<VertexBuffer> VertexBuffer::Create(const void* data, uint32_t size)
	{
		switch (RenderAPI::GetAPI())
		{
		case RenderAPI::API::None: ARC_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
		case RenderAPI::API::OpenGL: return CreateShared<OpenGLVertexBuffer>(data, size);
		}

		ARC_CORE_ASSERT(false, "Unknown API selected!");
		return nullptr;
	}

	Shared<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (RenderAPI::GetAPI())
		{
			case RenderAPI::API::None: ARC_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
			case RenderAPI::API::OpenGL: return CreateShared<OpenGLIndexBuffer>(indices, count);
		}

		ARC_CORE_ASSERT(false, "Unknown API selected!");
		return nullptr;
	}
}