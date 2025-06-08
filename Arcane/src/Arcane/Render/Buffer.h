#pragma once

#include <glm/matrix.hpp>

#include "Arcane/Core/Base.h"

namespace Arcane
{
	enum class ARC_API ShaderDataType
	{
		None = 0,
		Int, Int2, Int3, Int4,
		Float, Float2, Float3, Float4,
		Mat3, Mat4,
		Bool
	};

	static uint32_t ShaderDataSize(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::None:	 return 0;
			case ShaderDataType::Int:	 return 4;
			case ShaderDataType::Int2:	 return 4 * 2;
			case ShaderDataType::Int3:	 return 4 * 3;
			case ShaderDataType::Int4:	 return 4 * 4;
			case ShaderDataType::Float:  return 4;
			case ShaderDataType::Float2: return 4 * 2;
			case ShaderDataType::Float3: return 4 * 3;
			case ShaderDataType::Float4: return 4 * 4;
			case ShaderDataType::Mat3:	 return 4 * 3 * 3;
			case ShaderDataType::Mat4:	 return 4 * 4 * 4;
			case ShaderDataType::Bool:	 return 1;
		}

		ARC_CORE_ASSERT(false, "Unknown ShaderData type!");
		return 0;
	}

	struct ARC_API BufferElement
	{
		std::string Name;
		ShaderDataType Type;
		uint32_t Size;
		uint32_t IndexOffs;
		uint32_t Offset;
		uint32_t Stride;
		bool Normalized;
		bool Instanced;

		BufferElement() = default;
		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false, bool instanced = false, uint32_t indexOffset = 0, uint32_t offset = 0) :
			Name(name), Type(type), Size(ShaderDataSize(type)), IndexOffs(indexOffset), Stride(0), Offset(0), Normalized(normalized), Instanced(instanced) { }

		uint32_t GetComponentCount() const
		{
			switch (Type)
			{
				case ShaderDataType::None:	 break;
				case ShaderDataType::Int:	 return 1;
				case ShaderDataType::Int2:	 return 2;
				case ShaderDataType::Int3:	 return 3;
				case ShaderDataType::Int4:	 return 4;
				case ShaderDataType::Float:  return 1;
				case ShaderDataType::Float2: return 2;
				case ShaderDataType::Float3: return 3;
				case ShaderDataType::Float4: return 4;
				case ShaderDataType::Mat3:	 return 3;
				case ShaderDataType::Mat4:	 return 4;
				case ShaderDataType::Bool:	 return 1;
			}

			ARC_CORE_ASSERT(false, "Unknown Element type!");
			return 0;
		}
	};

	class ARC_API BufferLayout
	{
	public:
		BufferLayout() = default;
		BufferLayout(const std::initializer_list<BufferElement>& elements) :
			m_Elements(elements)
		{
			CalculateOffsetAndStride();
		}

		inline uint32_t GetStride() const { return m_Stride; }

		inline std::vector<BufferElement>& GetElements() { return m_Elements; }
		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }

		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

	private:
		inline void CalculateOffsetAndStride()
		{
			uint32_t offset = 0;
			m_Stride = 0;
			for (auto& elements : m_Elements)
			{
				if (elements.Offset == 0) // Only overwrite if unset
					elements.Offset = offset;

				offset = elements.Offset + elements.Size;
				m_Stride += elements.Size;
			}
		}

	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride = 0;
	};

	class ARC_API VertexBuffer
	{
	public:
		VertexBuffer() = default;
		virtual ~VertexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual BufferLayout& GetLayout() = 0;
		virtual const BufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;

		virtual void SetData(const void* data, uint32_t size, bool dynamic) = 0;
		virtual void SetSubData(const void* data, uint32_t size) = 0;

		static Shared<VertexBuffer> Create(uint32_t size);
		static Shared<VertexBuffer> Create(float* vertices, uint32_t size);
		static Shared<VertexBuffer> Create(const void* data, uint32_t size);
	};

	class ARC_API IndexBuffer
	{
	public:
		IndexBuffer() = default;
		virtual ~IndexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetCount() const = 0;

		static Shared<IndexBuffer> Create(uint32_t* indices, uint32_t size);
	};
}