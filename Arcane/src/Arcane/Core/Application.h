#pragma once

#include "Arcane/Core/Base.h"
#include "Arcane/Core/Window.h"
#include "Arcane/Core/LayerStack.h"
#include "Arcane/Core/Timestep.h"

#include "Arcane/Events/ApplicationEvent.h"

#include "Arcane/Render/Renderer.h"

#include "Arcane/ImGui/ImGuiLayer.h"

namespace Arcane
{

	class ARC_API Application
	{
	public:
		Application(WindowProps winProps = WindowProps());
		~Application();

		void Run();
		void Stop();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline void PushEvent(const Event& event) { m_Window->PushEvent((Event&)event); }

		inline const Unique<Window>& GetWindow() const { return m_Window; }

		inline ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

		inline static Application& Get() { return *s_Instance; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

		Unique<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;

	private:
		static Application* s_Instance;

	};

	// Defined application side
	Application* CreateApplication();
}