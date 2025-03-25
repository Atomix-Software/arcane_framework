#include<arcpch.h>

#include "Arcane/Core/CameraController.h"

#include "Arcane/Core/Input.h"
#include "Arcane/Core/KeyCodes.h"

namespace Arcane
{
	/* POV Camera (3D) Controller */

	POVCameraController::POVCameraController(uint32_t width, uint32_t height, float fov) :
		m_Position({ 0, 0, 0 }), m_Rotation({ 0, -90, 0 }), m_Speed(1.5f), m_CanMove(true)
	{
		m_Camera = CreateShared<PerspectiveCamera>((float)width, (float)height, fov);
		m_Camera->SetPosition(m_Position);
		m_Camera->SetRotation(m_Rotation);
	}

	void POVCameraController::OnUpdate(Timestep ts)
	{
		m_Position = m_Camera->GetPosition();

		float velocity = ts * m_Speed;

		glm::vec3 forward(0);
		forward.x = cos(glm::radians(m_Rotation.y));
		forward.z = sin(glm::radians(m_Rotation.y));
		forward   = glm::normalize(forward);

		glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0, 1, 0)));

		if (Input::IsKeyPressed(Key::W))
			m_Position += forward * velocity;
		else if (Input::IsKeyPressed(Key::S))
			m_Position -= forward * velocity;

		if (Input::IsKeyPressed(Key::A))
			m_Position -= right * velocity;
		else if (Input::IsKeyPressed(Key::D))
			m_Position += right * velocity;

		if (Input::IsKeyPressed(Key::Space)) // Jump
			m_Position.y += velocity;
		else if (Input::IsKeyPressed(Key::LeftControl)) // Crouch
			m_Position.y -= velocity;

		m_Camera->SetPosition(m_Position);
	}

	void POVCameraController::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowResizeEvent>(ARC_BIND_EVENT_FN(POVCameraController::OnWindowResized));
		dispatcher.Dispatch<MouseMovedEvent>(ARC_BIND_EVENT_FN(POVCameraController::OnMouseMoved));
	}

	void POVCameraController::OnResize(float width, float height)
	{
		m_Camera->SetSize(width, height);
	}

	bool POVCameraController::OnWindowResized(WindowResizeEvent& event)
	{
		OnResize((float)event.GetWidth(), (float)event.GetHeight());
		return false;
	}

	bool POVCameraController::OnMouseMoved(MouseMovedEvent& event)
	{
		static bool firstMouse = true;
		static float lastX = event.GetX(), lastY = event.GetY();

		float x = event.GetX();
		float y = event.GetY();

		if (firstMouse) {
			lastX = x;
			lastY = y;
			firstMouse = false;
		}

		// Calculate mouse delta
		float xOffset = x - lastX;
		float yOffset = lastY - y; // Inverted y-axis since screen coordinates start at top-left

		lastX = x;
		lastY = y;

		// Sensitivity adjustment
		float sensitivity = 0.1f;
		xOffset *= sensitivity;
		yOffset *= sensitivity;

		// Update yaw and pitch
		m_Rotation.y += xOffset; // Yaw (horizontal rotation)
		m_Rotation.x += yOffset; // Pitch (vertical rotation)

		// Constrain pitch to avoid flipping
		if (m_Rotation.x > 89.0f)
			m_Rotation.x = 89.0f;
		if (m_Rotation.x < -89.0f)
			m_Rotation.x = -89.0f;

		// Apply updated rotation to the camera
		m_Camera->SetRotation(m_Rotation);
		return false;
	}


	/* Ortho Camera (2D) Controller */

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

			if (Input::IsKeyPressed(Key::Up))
			{
				m_Position.x += -sin(glm::radians(m_Camera->GetZRotation())) * m_Speed * ts;
				m_Position.y += cos(glm::radians(m_Camera->GetZRotation())) * m_Speed * ts;
			}
			else if (Input::IsKeyPressed(Key::Down))
			{
				m_Position.x -= -sin(glm::radians(m_Camera->GetZRotation())) * m_Speed * ts;
				m_Position.y -= cos(glm::radians(m_Camera->GetZRotation())) * m_Speed * ts;
			}

			if (Input::IsKeyPressed(Key::Left))
			{
				m_Position.x -= cos(glm::radians(m_Camera->GetZRotation())) * m_Speed * ts;
				m_Position.y -= sin(glm::radians(m_Camera->GetZRotation())) * m_Speed * ts;
			}
			else if (Input::IsKeyPressed(Key::Right))
			{
				m_Position.x += cos(glm::radians(m_Camera->GetZRotation())) * m_Speed * ts;
				m_Position.y += sin(glm::radians(m_Camera->GetZRotation())) * m_Speed * ts;
			}

			float rotationSpeed = m_Speed * 20.0f;
			if (Input::IsKeyPressed(Key::RightShift))
				m_Rotation += rotationSpeed * ts;
			else if (Input::IsKeyPressed(Key::RightControl))
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
		if (m_CanMove)
		{
			m_Camera->SetZoom(m_Camera->GetZoom() - event.GetYOffset() * 0.07f);

			float zoom = m_Camera->GetZoom();
			m_Camera->SetProjection(-m_AspectRatio * zoom, m_AspectRatio * zoom, -zoom, zoom);
		}

		return false;
	}
}