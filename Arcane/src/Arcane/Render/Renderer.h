#pragma once

#include "RenderCMD.h"

#include "Arcane/Render/Model.h"
#include "Arcane/Render/Shader.h"
#include "Arcane/Render/Texture.h"
#include "Arcane/Render/SubTexture.h"
#include "Arcane/Render/Camera.h"

#include <glm/glm.hpp>

namespace Arcane
{

	struct Transform
	{
		glm::vec3 Position;
		glm::vec3 Rotation;
		glm::vec3 Scale;

		Transform() :
			Position(0.0), Rotation({0, 0, 0}), Scale(1.0) { }

		Transform(glm::vec3& position, glm::vec3 rotation, glm::vec3 scale) :
			Position(position), Rotation(rotation), Scale(scale) { }
	};

	class ARC_API Renderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(PerspectiveCamera& camera);
		static void EndScene();

		static void DrawCube(Material& material, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);
		static void DrawCube(Material& material, glm::mat4& transform);

		static void DrawPlane(Material& material, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);
		static void DrawPlane(Material& material, glm::mat4& transform);

		static void AddShader(Shared<Shader> shader);

		inline static RenderAPI::API GetAPI() { return RenderAPI::GetAPI(); }

	private:
		static Model* MakePlane(Material& material);
		static Model* MakeCube(Material& material);

	};
}