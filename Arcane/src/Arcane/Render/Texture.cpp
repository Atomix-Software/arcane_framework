#include <arcpch.h>

#include "Arcane/Render/Texture.h"

#include "Arcane/Render/RenderAPI.h"

#include "Platform/OpenGL/OpenGLTexture.h"

namespace Arcane
{
	Shared<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (RenderAPI::GetAPI())
		{
		case RenderAPI::API::None: ARC_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
		case RenderAPI::API::OpenGL: return CreateShared<OpenGLTexture2D>(width, height);
		}

		ARC_CORE_ASSERT(false, "Unknown API selected!");
		return nullptr;
	}

	Shared<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (RenderAPI::GetAPI())
		{
		case RenderAPI::API::None: ARC_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
		case RenderAPI::API::OpenGL: return CreateShared<OpenGLTexture2D>(path);
		}

		ARC_CORE_ASSERT(false, "Unknown API selected!");
		return nullptr;
	}
}