#include <arcpch.h>

#include "Arcane/Render/RenderAPI.h"

#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Arcane
{
	Shared<Framebuffer> Framebuffer::Create(const FramebufferSpec& specs)
	{
		switch (RenderAPI::GetAPI())
		{
		case RenderAPI::API::None: ARC_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
		case RenderAPI::API::OpenGL: return CreateShared<OpenGLFramebuffer>(specs);
		}

		ARC_CORE_ASSERT(false, "Unknown API selected!");
		return nullptr;
	}
}