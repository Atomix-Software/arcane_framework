#pragma once

#include "Arcane/Core/Base.h"

namespace Arcane
{
	class ARC_API Texture
	{
	public:
		virtual ~Texture() = default;

		virtual void Bind(uint32_t slot = 0) const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;

		virtual unsigned char* GetData() const = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetRendererID() const = 0;

		virtual bool operator==(const Texture& other) const = 0;
	};

	class ARC_API Texture2D : public Texture
	{
	public:
		static Shared<Texture2D> Create(uint32_t width, uint32_t height);
		static Shared<Texture2D> Create(const std::string& path);

	};
}