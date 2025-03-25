#include <arcpch.h>

#include "Arcane/Render/Camera.h"

namespace Arcane
{
	/* Perspective Camera (3D) */

	PerspectiveCamera::PerspectiveCamera(float width, float height, float fov) :
		Camera(), m_Width(width), m_Height(height), m_FOV(fov)
	{
		SetProjection(width, height, fov);
		UpdateMatrices();
	}

	void PerspectiveCamera::SetProjection(float width, float height, float fov)
	{
		float aspectRatio = width / height;
		m_Projection = glm::perspective(glm::radians(fov), aspectRatio, 0.01f, 1000.0f);
	}

	void PerspectiveCamera::UpdateMatrices()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(m_Rotation.y)) * cos(glm::radians(m_Rotation.x));
		front.y = sin(glm::radians(m_Rotation.x));
		front.z = sin(glm::radians(m_Rotation.y)) * cos(glm::radians(m_Rotation.x));
		front = glm::normalize(front);

		glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0, 1, 0)));
		glm::vec3 up = glm::normalize(glm::cross(right, front));

		m_View = glm::lookAt(m_Position, m_Position + front, up);
	}

	/* Orthographic Camera (2D) */

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top) :
		Camera()
	{
		SetProjection(left, right, bottom, top);
		UpdateMatrices();
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		m_Projection = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
	}

	void OrthographicCamera::UpdateMatrices()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.z), glm::vec3(0, 0, 1)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));

		m_View = glm::inverse(transform);
	}
}