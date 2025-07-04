#pragma once

#include "Arcane/Render/Buffer.h"

namespace Arcane
{
	class ARC_API OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(uint32_t size);
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		OpenGLVertexBuffer(const void* data, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetData(const void* data, uint32_t size, bool dynamic = false);
		virtual void SetSubData(const void* data, uint32_t size);

		inline virtual BufferLayout& GetLayout() override { return m_Layout; }
		inline virtual const BufferLayout& GetLayout() const override { return m_Layout; }
		inline virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }

	private:
		uint32_t m_RendererId;
		BufferLayout m_Layout;
	};

	class ARC_API OpenGLSSBuffer : public ShaderStorageBuffer
	{
	public:
		OpenGLSSBuffer(uint32_t size, uint32_t binding);
		OpenGLSSBuffer(uint32_t size, const void* data, uint32_t binding);
		virtual ~OpenGLSSBuffer();

		virtual void Bind(uint32_t binding = 0) const override;
		virtual void Unbind() const override;

		virtual void  SetData(const void* data, uint32_t size, uint32_t offset = 0) override;
		virtual void* MapBuffer(uint32_t access) override;
		virtual void  UnmapBuffer() override;

	private:
		uint32_t m_RendererID = 0;
	};

	class ARC_API OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual uint32_t GetCount() const override { return m_Count; }

	private:
		uint32_t m_RendererId;
		uint32_t m_Count;

	};
}