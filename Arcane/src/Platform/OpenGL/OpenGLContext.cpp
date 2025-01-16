#include <arcpch.h>

#include "Platform/OpenGL/OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Arcane
{
	static bool s_GLADInitialized = false;

	OpenGLContext::OpenGLContext(GLFWwindow* winHandle) :
		m_WinHandle(winHandle)
	{
		ARC_CORE_ASSERT(winHandle, "Window Handle is null!");
	}

	void OpenGLContext::Init()
	{
		ARC_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_WinHandle);
		if (!s_GLADInitialized)
		{
			int success = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
			ARC_CORE_ASSERT(success, "Could not initialize OpenGL!");

			s_GLADInitialized = true;
		}
	}

	void OpenGLContext::SwapBuffers()
	{
		ARC_PROFILE_FUNCTION();
		glfwSwapBuffers(m_WinHandle);
	}
}