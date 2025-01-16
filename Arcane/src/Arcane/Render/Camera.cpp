#include <arcpch.h>

#include "Arcane/Render/Camera.h"

namespace Arcane
{
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