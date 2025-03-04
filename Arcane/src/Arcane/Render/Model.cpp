#include <arcpch.h>

#include "Model.h"

namespace Arcane
{
	Mesh::Mesh(std::vector<float>& vertices, std::vector<uint32_t>& indices, const BufferLayout& layout)
	{
		m_VAO = VertexArray::Create();
		Shared<VertexBuffer> vbo = VertexBuffer::Create(vertices.data(), (uint32_t)vertices.size() * sizeof(float));
		vbo->SetLayout(layout);
		m_VAO->AddVertexBuffer(vbo);

		Shared<IndexBuffer> ebo = IndexBuffer::Create(indices.data(), (uint32_t)indices.size());
		m_VAO->SetIndexBuffer(ebo);
	}

	Model::Model(const Material& material, const Shared<Mesh> mesh) :
		m_Material(material), m_Mesh(mesh) { }
}