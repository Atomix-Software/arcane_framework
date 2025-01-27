#include<arcpch.h>

#include "Arcane/Core/CameraController.h"

#include "Arcane/Core/Input.h"
#include "Arcane/Core/KeyCodes.h"

namespace Arcane
{
	OrthoCameraController::OrthoCameraController(float aspectRatio, float zoom) :
		m_AspectRatio(aspectRatio), m_Position(0.0), m_Rotation(0.0), m_Speed(1.5), m_CanMove(true)
	{
		m_Camera = CreateShared<OrthographicCamera>(-m_AspectRatio * zoom, m_AspectRatio * zoom, -zoom, zoom);
		m_Camera->SetZoom(zoom);
	}

	void OrthoCameraController::OnUpdate(Timestep ts)
	{
		m_Position = m_Camera->GetPosition();
		m_Rotation = m_Camera->GetZRotation();

		if (m_CanMove)
		{

			if (Input::IsKeyPressed(Key::W))
			{
				m_Position.x += -sin(glm::radians(m_Camera->GetZRotation())) * m_Speed * ts;
				m_Position.y += cos(glm::radians(m_Camera->GetZRotation())) * m_Speed * ts;
			}
			else if (Input::IsKeyPressed(Key::S))
			{
				m_Position.x -= -sin(glm::radians(m_Camera->GetZRotation())) * m_Speed * ts;
				m_Position.y -= cos(glm::radians(m_Camera->GetZRotation())) * m_Speed * ts;
			}

			if (Input::IsKeyPressed(Key::A))
			{
				m_Position.x -= cos(glm::radians(m_Camera->GetZRotation())) * m_Speed * ts;
				m_Position.y -= sin(glm::radians(m_Camera->GetZRotation())) * m_Speed * ts;
			}
			else if (Input::IsKeyPressed(Key::D))
			{
				m_Position.x += cos(glm::radians(m_Camera->GetZRotation())) * m_Speed * ts;
				m_Position.y += sin(glm::radians(m_Camera->GetZRotation())) * m_Speed * ts;
			}

			float rotationSpeed = m_Speed * 20.0f;
			if (Input::IsKeyPressed(Key::Q))
				m_Rotation += rotationSpeed * ts;
			else if (Input::IsKeyPressed(Key::E))
				m_Rotation -= rotationSpeed * ts;
		}

		m_Camera->SetPosition(m_Position);
		m_Camera->SetZRotation(m_Rotation);
	}

	void OrthoCameraController::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowResizeEvent>(ARC_BIND_EVENT_FN(OrthoCameraController::OnWindowResized));
		dispatcher.Dispatch<MouseScrolledEvent>(ARC_BIND_EVENT_FN(OrthoCameraController::OnMouseScrolled));
	}

	void OrthoCameraController::OnResize(float width, float height)
	{
		float zoom = m_Camera->GetZoom();
		m_AspectRatio = width / height;
		m_Camera->SetProjection(-m_AspectRatio * zoom, m_AspectRatio * zoom, -zoom, zoom);
	}

	bool OrthoCameraController::InBounds(const glm::vec2& position, const glm::vec2& size)
	{
		glm::vec2 halfSize = { size.x / 2.0f, size.y / 2.0f };
		if (position.x - halfSize.x >= m_Position.x + m_AspectRatio * m_Camera->GetZoom() ||
			position.x + halfSize.x <= m_Position.x - m_AspectRatio * m_Camera->GetZoom())
			return false;

		if (position.y - halfSize.y >= m_Position.y + m_Camera->GetZoom() ||
			position.y + halfSize.y <= m_Position.y - m_Camera->GetZoom())
			return false;

		return true;
	}

	bool OrthoCameraController::OnWindowResized(WindowResizeEvent& event)
	{
		OnResize((float)event.GetWidth(), (float)event.GetHeight());
		return false;
	}

	bool OrthoCameraController::OnMouseScrolled(MouseScrolledEvent& event)
	{
		m_Camera->SetZoom(m_Camera->GetZoom() - event.GetYOffset() * 0.07f);

		float zoom = m_Camera->GetZoom();
		m_Camera->SetProjection(-m_AspectRatio * zoom, m_AspectRatio * zoom, -zoom, zoom);
		return false;
	}
}