#pragma once

#include "Arcane/Render/Texture.h"

#include <glm/glm.hpp>

namespace Arcane
{
	class ARC_API SubTexture2D
	{
	public:
		SubTexture2D(const Shared<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max);

		inline const Shared<Texture2D> GetTexture() const { return m_Texture; }
		inline const glm::vec2* GetTexCoords() const { return m_TexCoords; }

		static Shared<SubTexture2D> CreateFromCoords(const Shared<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& spriteSize, const glm::vec2& cellSize = { 1, 1 });

	private:
		Shared<Texture2D> m_Texture;
		glm::vec2 m_TexCoords[4];

	};
}