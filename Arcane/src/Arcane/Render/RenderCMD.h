#pragma once

#include "RenderAPI.h"

namespace Arcane
{

	class ARC_API RenderCMD
	{
	public:
		inline static void Init()
		{
			s_RenderAPI->Init();
		}

		inline static void Clear(bool clearDepth)
		{
			s_RenderAPI->Clear(clearDepth);
		}

		inline static void EnableDepthTesting(bool enable)
		{
			s_RenderAPI->EnableDepthTesting(enable);
		}

		inline static void EnableFaceCulling(bool enable)
		{
			s_RenderAPI->EnableFaceCulling(enable);
		}

		inline static void SetCullFace(const CullFace& face)
		{
			s_RenderAPI->SetCullFace(face);
		}

		inline static void SetClearColor(const glm::vec4& color)
		{
			s_RenderAPI->SetClearColor(color);
		}

		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_RenderAPI->SetViewport(x, y, width, height);
		}

		inline static void DrawArrays(const Shared<VertexArray>& vao, uint32_t first, uint32_t count)
		{
			s_RenderAPI->DrawArrays(vao, first, count);
		}

		inline static void DrawIndexed(const Shared<VertexArray>& vao, uint32_t indexCount = 0)
		{
			s_RenderAPI->DrawIndexed(vao, indexCount);
		}

	private:
		static RenderAPI* s_RenderAPI;

	};
}