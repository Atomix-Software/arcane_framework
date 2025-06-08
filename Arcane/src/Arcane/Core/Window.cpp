#include <arcpch.h>

#include "Arcane/Core/Window.h"

#include "Arcane/Render/RenderAPI.h"
#include "Platform/OpenGL/OpenGLWindow.h"

namespace Arcane
{
	Unique<Window> Window::Create(const WindowProps& props)
	{
		switch (RenderAPI::GetAPI())
		{
		case RenderAPI::API::None: ARC_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
		case RenderAPI::API::OpenGL: return CreateUnique<OpenGLWindow>(props);
		}

		ARC_CORE_ASSERT(false, "Unknown API selected!");
		return nullptr;
	}
}