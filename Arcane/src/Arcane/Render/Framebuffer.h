#pragma once

#include "Arcane/Core/Base.h"

namespace Arcane
{
	struct ARC_API FramebufferSpec
	{
		uint32_t Width, Height;
		uint32_t Samples = 1;

		bool SwapChainTarget = false;
	};

	class ARC_API Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual uint32_t GetColorAttRendererID() const = 0;

		virtual const FramebufferSpec& GetFramebufferSpecs() const = 0;

		static Shared<Framebuffer> Create(const FramebufferSpec& specs);
	};
}