#pragma once

#include "Arcane/Render/RenderAPI.h"

namespace Arcane
{
	class ARC_API OpenGLRenderAPI : public RenderAPI
	{
	public:
		virtual ~OpenGLRenderAPI() override {}

		virtual void Init() override;
		virtual void Clear(bool clearDepth) override;

		virtual void EnableDepthTesting(bool enable) override;
		virtual void EnableFaceCulling(bool enable) override;

		virtual void SetCullFace(const CullFace& face) override;
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

		virtual void DrawArrays(const Shared<VertexArray>& vao, uint32_t first, uint32_t count) override;
		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vao, uint32_t count) override;
	};
}