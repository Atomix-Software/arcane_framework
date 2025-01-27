#pragma once

#include "Arcane/Core/Timestep.h"

#include "Arcane/Events/ApplicationEvent.h"
#include "Arcane/Events/MouseEvent.h"

#include "Arcane/Render/Camera.h"

namespace Arcane
{
	class ARC_API OrthoCameraController
	{
	public:
		OrthoCameraController(float aspectRatio, float zoom = 1.0f);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& event);

		void OnResize(float width, float height);

		bool InBounds(const glm::vec2& position, const glm::vec2& size);

		inline const Shared<OrthographicCamera>& GetCamera() { return m_Camera; }
		inline glm::vec3& GetPosition() { return m_Position; }

		inline const float GetRotation() const { return m_Rotation; }
		
		inline const float GetAspectRatio() const { return m_AspectRatio; }
		inline const float GetZoom() const { return m_Camera->GetZoom(); }
		inline const float GetSpeed() const { return m_Speed; }
		inline const float CanMove() const { return m_CanMove; }

		inline void SetSpeed(float speed) { m_Speed = speed; }
		inline void SetCanMove(bool canMove) { m_CanMove = canMove; }

	private:
		bool OnWindowResized(WindowResizeEvent& event);
		bool OnMouseScrolled(MouseScrolledEvent& event);

	private:
		glm::vec3 m_Position;
		float m_Rotation;
		float m_AspectRatio, m_Speed;
		bool m_CanMove;

		Shared<OrthographicCamera> m_Camera;

	};
}