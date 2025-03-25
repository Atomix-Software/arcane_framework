#include <arcpch.h>

#include "Platform/OpenGL/OpenGLWindow.h"

#include "Arcane/Events/ApplicationEvent.h"
#include "Arcane/Events/KeyEvent.h"
#include "Arcane/Events/MouseEvent.h"

#include "Platform/OpenGL/OpenGLContext.h"

#include <glad/glad.h>

namespace Arcane
{

	static int s_GLFWwindowCount = 0;

	static void GLFWErrorCallback(int error, const char* description)
	{
		ARC_CORE_ERROR("GLFW Error [{0}]: {1}", error, description);
	}

	OpenGLWindow::OpenGLWindow(const WindowProps& props)
	{
		ARC_PROFILE_FUNCTION();
		Init(props);
	}

	OpenGLWindow::~OpenGLWindow()
	{
		Shutdown();
	}

	void OpenGLWindow::Init(const WindowProps& props)
	{
		ARC_PROFILE_FUNCTION();
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		if (s_GLFWwindowCount == 0)
		{
			int success = glfwInit();
			ARC_CORE_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
		}

		glfwDefaultWindowHints();
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
		glfwWindowHint(GLFW_RESIZABLE, props.Resizable ? GLFW_TRUE : GLFW_FALSE);

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		++s_GLFWwindowCount;

		const GLFWvidmode* vid_mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		glfwSetWindowPos(m_Window, (vid_mode->width - props.Width) / 2, (vid_mode->height - props.Height) / 2);

		m_Context = new OpenGLContext(m_Window);
		m_Context->Init();

		SetVSync(true);
		SetCaptureMouse(props.CaptureMouse);

		glfwSetWindowUserPointer(m_Window, &m_Data);
		glfwShowWindow(m_Window);

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				data.Width = width;
				data.Height = height;

				WindowResizeEvent event(width, height);
				data.EventCallback(event);
			});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowCloseEvent event;
				data.EventCallback(event);
			});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					data.EventCallback(event);
					break;
				}

				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data.EventCallback(event);
					break;
				}

				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, 1);
					data.EventCallback(event);
					break;
				}
				}
			});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int ch)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				KeyTypedEvent event(ch);
				data.EventCallback(event);
			});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.EventCallback(event);
					break;
				}

				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.EventCallback(event);
					break;
				}
				}
			});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffs, double yOffs)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				MouseScrolledEvent event((float)xOffs, (float)yOffs);
				data.EventCallback(event);
			});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				MouseMovedEvent event((float)xPos, (float)yPos);
				data.EventCallback(event);
			});
	}

	void OpenGLWindow::Shutdown()
	{
		ARC_PROFILE_FUNCTION();

		glfwDestroyWindow(m_Window);
		--s_GLFWwindowCount;

		if (s_GLFWwindowCount == 0)
			glfwTerminate();
	}

	void OpenGLWindow::OnUpdate()
	{
		ARC_PROFILE_FUNCTION();

		glfwPollEvents();
		m_Context->SwapBuffers();
	}

	void OpenGLWindow::SetVSync(bool enabled)
	{
		ARC_PROFILE_FUNCTION();

		if (enabled) glfwSwapInterval(1);
		else glfwSwapInterval(0);

		m_Data.VSync = enabled;
	}

	void OpenGLWindow::SetCaptureMouse(bool capture)
	{
		ARC_PROFILE_FUNCTION();

		if(capture) glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		else glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		m_Data.MouseCapture = capture;
	}

	bool OpenGLWindow::IsVSync() const
	{
		return m_Data.VSync;
	}

	bool OpenGLWindow::CapturesMouse() const
	{
		return m_Data.MouseCapture;
	}

}