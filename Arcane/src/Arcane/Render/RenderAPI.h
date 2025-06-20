#pragma once

#include "Arcane/Core/Base.h"

#include "Arcane/Render/VertexArray.h"

#include <glm/glm.hpp>

namespace Arcane
{
	enum ARC_API CullFace : uint32_t
	{
		BACK = 0, FRONT,
		FRONT_AND_BACK,
	};

	class ARC_API RenderAPI
	{
	public:
		enum class API
		{
			None = 0,
			OpenGL = 1
		};

	public:
		virtual ~RenderAPI() = default;

		virtual void Init() = 0;
		virtual void Clear(bool clearDepth) = 0;
		
		virtual void EnableDepthTesting(bool enable) = 0;
		virtual void EnableFaceCulling(bool enable) = 0;

		virtual void SetCullFace(const CullFace& face) = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

		virtual void DrawArrays(const Shared<VertexArray>& vao, uint32_t first, uint32_t count) = 0;
		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vao, uint32_t count) = 0;
		virtual void DrawInstanced(const Shared<VertexArray>& vao, uint32_t amount, uint32_t indexCount) = 0;

		inline static API GetAPI() { return s_API; }
	private:
		static API s_API;
	};
}