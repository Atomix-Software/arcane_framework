#include <arcpch.h>

#include "Arcane/Render/Renderer.h"

#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Arcane
{
	Shared<Framebuffer> Framebuffer::Create(const FramebufferSpec& specs)
	{
		switch (Renderer::GetAPI())
		{
		case RenderAPI::API::None: ARC_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
		case RenderAPI::API::OpenGL: return CreateShared<OpenGLFramebuffer>(specs);
		}

		ARC_CORE_ASSERT(false, "Unknown API selected!");
		return nullptr;
	}
}