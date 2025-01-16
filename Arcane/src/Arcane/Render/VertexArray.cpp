#include <arcpch.h>

#include "Arcane/Render/VertexArray.h"

#include "Arcane/Render/Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Arcane
{
	Shared<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RenderAPI::API::None: ARC_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
		case RenderAPI::API::OpenGL: return CreateShared<OpenGLVertexArray>();
		}

		ARC_CORE_ASSERT(false, "Unknown API selected!");
		return nullptr;
	}
}