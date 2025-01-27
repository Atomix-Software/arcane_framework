#include <arcpch.h>

#include "Arcane/Core/Application.h"
#include "Arcane/Core/Log.h"

#include <GLFW/glfw3.h>

namespace Arcane
{

	Application* Application::s_Instance = nullptr;

	Application::Application(WindowProps winProps)
	{
		ARC_PROFILE_FUNCTION();
		ARC_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = Unique<Window>(Window::Create(winProps));
		m_Window->SetEventCallback(ARC_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();
		Renderer2D::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		ARC_PROFILE_FUNCTION();
		Renderer::Shutdown();
		Renderer2D::Shutdown();
	}

	void Application::PushLayer(Layer* layer)
	{
		ARC_PROFILE_FUNCTION();
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay)
	{
		ARC_PROFILE_FUNCTION();
		m_LayerStack.PushOverlay(overlay);
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(ARC_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(ARC_BIND_EVENT_FN(Application::OnWindowResized));

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			if (e.Handled) break;
			(*it)->OnEvent(e);
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return false;
	}

	bool Application::OnWindowResized(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}
		
		m_Minimized = false;
		RenderCMD::SetViewport(e.GetWidth(), e.GetHeight());

		return false;
	}

	void Application::Run()
	{
		ARC_PROFILE_FUNCTION();

		constexpr float fixedDT = 1.0f / 120.0f;
		Timestep accumulator(0.0f);

		auto lastTime = std::chrono::high_resolution_clock::now();

		while (m_Running)
		{

			auto currentTime = std::chrono::high_resolution_clock::now();
			std::chrono::duration<float> elapsedTime = currentTime - lastTime;
			lastTime = currentTime;

			Timestep deltaTime(elapsedTime.count());
			accumulator += deltaTime;

			if (!m_Minimized)
			{
				while (accumulator >= fixedDT)
				{
					for (auto layer : m_LayerStack)
						layer->OnUpdate(Timestep(fixedDT));

					accumulator -= fixedDT;
				}
			}

			m_ImGuiLayer->Begin();
			for (auto layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();

			//constexpr float maxFrameRate = 1.0f / 120.0f; // Cap at 120 FPS
			//if (deltaTime < maxFrameRate)
			//{
			//	std::this_thread::sleep_for(std::chrono::duration<float>(maxFrameRate - (float)deltaTime));
			//}
		}
	}

	void Application::Stop()
	{
		if (m_Running) m_Running = false;
	}

}