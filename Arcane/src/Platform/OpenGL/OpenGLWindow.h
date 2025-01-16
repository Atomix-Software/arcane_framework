#pragma once

#include "Arcane/Core/Window.h"
#include "Arcane/Render/GraphicsContext.h"

#include <GLFW/glfw3.h>

namespace Arcane
{
	class ARC_API OpenGLWindow : public Window
	{
	public:
		OpenGLWindow(const WindowProps& props);
		virtual ~OpenGLWindow();

		virtual void OnUpdate() override;

		virtual inline unsigned int GetWidth() const override { return m_Data.Width; }
		virtual inline unsigned int GetHeight() const override { return m_Data.Height; }

		virtual inline void PushEvent(Event& event) override { m_Data.EventCallback(event); }

		// Window Attributes
		virtual inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		virtual void SetVSync(bool enabled) override;
		virtual bool IsVSync() const override;

		inline virtual void* GetNativeWindow() const override { return m_Window; }

	private:
		virtual void Init(const WindowProps& prop);
		virtual void Shutdown();

	private:
		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		GLFWwindow* m_Window;
		GraphicsContext* m_Context;

		WindowData m_Data;
	};
}