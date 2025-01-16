#include <arcpch.h>

#include "Platform/OpenGL/OpenGLRenderAPI.h"

#include <glad/glad.h>

namespace Arcane
{
	void OpenGLRenderAPI::Init()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
	}

	void OpenGLRenderAPI::Clear(bool clearDepth)
	{
		GLenum clearMask = GL_COLOR_BUFFER_BIT;
		if (clearDepth) clearMask |= GL_DEPTH_BUFFER_BIT;

		glClear(clearMask);
	}

	void OpenGLRenderAPI::EnableDepthTesting(bool enable)
	{
		GLenum depthTest = GL_DEPTH_TEST;
		if (enable) glEnable(depthTest);
		else		glDisable(depthTest);
	}

	void OpenGLRenderAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRenderAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRenderAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vao, uint32_t indexCount)
	{
		vao->Bind();
		uint32_t count = indexCount != 0 ? indexCount : vao->GetIndexBuffer()->GetCount();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}
}