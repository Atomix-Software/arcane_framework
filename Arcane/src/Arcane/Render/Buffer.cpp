#include <arcpch.h>

#include "Arcane/Render/Buffer.h"
#include "Arcane/Render/Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Arcane
{
	Shared<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RenderAPI::API::None: ARC_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
		case RenderAPI::API::OpenGL: return CreateShared<OpenGLVertexBuffer>(size);
		}

		ARC_CORE_ASSERT(false, "Unknown API selected!");
		return nullptr;
	}
	Shared<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RenderAPI::API::None: ARC_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
		case RenderAPI::API::OpenGL: return CreateShared<OpenGLVertexBuffer>(vertices, size);
		}

		ARC_CORE_ASSERT(false, "Unknown API selected!");
		return nullptr;
	}

	Shared<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
			case RenderAPI::API::None: ARC_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
			case RenderAPI::API::OpenGL: return CreateShared<OpenGLIndexBuffer>(indices, count);
		}

		ARC_CORE_ASSERT(false, "Unknown API selected!");
		return nullptr;
	}
}