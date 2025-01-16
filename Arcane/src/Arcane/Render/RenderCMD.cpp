#include <arcpch.h>

#include "Arcane/Render/RenderCMD.h"

#include "Platform/OpenGL/OpenGLRenderAPI.h"

namespace Arcane
{
	RenderAPI* RenderCMD::s_RenderAPI = new OpenGLRenderAPI();
}