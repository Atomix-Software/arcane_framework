#pragma once

#include <Arcane/Core/Base.h>

#include <Arcane/Render/Shader.h>
#include <Arcane/Render/Texture.h>
#include <Arcane/Render/VertexArray.h>

namespace Arcane
{
	struct ARC_API Material
	{
		Shared<Shader> Shader = nullptr;
		Shared<Texture2D> Texture = nullptr;
		glm::vec3 Color = glm::vec3(1.0f);
	};

	class ARC_API Mesh
	{
	public:
		Mesh(std::vector<float>& vertices, std::vector<uint32_t>& indices, const BufferLayout& layout);
		~Mesh() = default;

		inline Shared<VertexArray>& GetVAO() { return m_VAO; }

	private:
		Shared<VertexArray> m_VAO;

	};

	class ARC_API Model
	{
	public:
		Model(const Material& material, const Shared<Mesh> mesh);
		~Model() = default;

		inline Shared<Mesh>& GetMesh() { return m_Mesh; }
		inline const Shared<Mesh>& GetMesh() const { return m_Mesh; }

		inline Material& GetMaterial() { return m_Material; }
		inline const Material& GetMaterial() const { return m_Material; }

	private:
		Shared<Mesh> m_Mesh;
		Material m_Material;

	};
}