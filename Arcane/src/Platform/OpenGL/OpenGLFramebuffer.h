#pragma once

#include "Arcane/Render/Framebuffer.h"

namespace Arcane
{
	class ARC_API OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpec& specs);
		virtual ~OpenGLFramebuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void Resize(uint32_t width, uint32_t height) override;

		inline virtual uint32_t GetColorAttRendererID() const override { return m_ColorAttachment; }

		inline virtual const FramebufferSpec& GetFramebufferSpecs() const override { return m_Specification; }

		void Invalidate();

	private:
		uint32_t m_RendererID = 0;
		uint32_t m_ColorAttachment = 0, m_DepthAttachment = 0;

		FramebufferSpec m_Specification;

	};
}