#pragma once

#include "Arcane/Core/Base.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace Arcane
{
	class ARC_API Camera
	{
	public:
		Camera() : m_Projection(1.0), m_View(1.0), m_Position(0.0), m_Rotation(0.0), m_Zoom(1.0) {}
		virtual ~Camera() = default;

		inline const glm::mat4& GetView() const { return m_View; }
		inline const glm::mat4& GetProjection() const { return m_Projection; }

		inline const glm::mat4 GetProjectionView() const { return m_Projection * m_View; }
		inline const glm::mat4 GetViewProjection() const { return m_View * m_Projection; }

		inline const glm::vec3& GetPosition() const { return m_Position; }
		inline const glm::vec3& GetRotation() const { return m_Rotation; }
		inline const float GetZoom() const { return m_Zoom; }

		inline void SetPosition(const glm::vec3& position) { m_Position = position; UpdateMatrices(); }
		inline void SetRotation(const glm::vec3& rotation) { m_Rotation = rotation; UpdateMatrices(); }
		inline void SetZoom(float zoom) { m_Zoom = zoom; m_Zoom = std::clamp(m_Zoom, 0.075f, 2.0f); UpdateMatrices(); }

	protected:
		virtual void UpdateMatrices() = 0;

	protected:
		glm::mat4 m_Projection, m_View;
		glm::vec3 m_Position, m_Rotation;
		float m_Zoom;

	};

	class ARC_API PerspectiveCamera : public Camera
	{
	public:
	public:
		PerspectiveCamera(float width, float height, float fov);

		void SetProjection(float width, float height, float fov);

		inline void SetSize(float width, float height) { m_Width = width; m_Height = height; SetProjection(m_Width, m_Height, m_FOV); }
		inline void SetFOV(float fov) { m_FOV = fov; SetProjection(m_Width, m_Height, m_FOV); }

		inline std::tuple<float, float> GetSize() const { return { m_Width, m_Height }; }
		inline float GetFOV() const { return m_FOV; }

	protected:
		virtual void UpdateMatrices() override;

	private:
		float m_Width, m_Height;
		float m_FOV;

	};

	class ARC_API OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);

		void SetProjection(float left, float right, float bottom, float top);

		inline float GetZRotation() { return GetRotation().z; }
		inline void SetZRotation(float rotation) { SetRotation({ 0, 0, rotation }); }

	protected:
		virtual void UpdateMatrices() override;

	};
}