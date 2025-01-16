#pragma once

#include "Arcane/Render/GraphicsContext.h"

namespace Arcane
{
	class ARC_API OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* winHandle);
		virtual ~OpenGLContext() override {}

		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		GLFWwindow* m_WinHandle;
	};
}