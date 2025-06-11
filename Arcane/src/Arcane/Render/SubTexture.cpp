#include <arcpch.h>

#include "Arcane/Render/SubTexture.h"

namespace Arcane
{
	SubTexture2D::SubTexture2D(const Shared<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max) :
		m_Texture(texture)
	{
		m_TexCoords[0] = { min.x, min.y };
		m_TexCoords[1] = { max.x, min.y };
		m_TexCoords[2] = { max.x, max.y };
		m_TexCoords[3] = { min.x, max.y };
	}

	Shared<SubTexture2D> SubTexture2D::CreateFromCoords(const Shared<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& spriteSize, const glm::vec2& cellSize)
	{
		glm::vec2 min = { 
			coords.x == 0.0f ? 0.0f : (coords.x * spriteSize.x) / texture->GetWidth(), 
			coords.y == 0.0f ? 0.0f : (coords.y * spriteSize.y) / texture->GetHeight() 
		};

		glm::vec2 max = { 
			((coords.x + cellSize.x) * spriteSize.x) == 0.0f ? 0.0f :((coords.x + cellSize.x) * spriteSize.x) / texture->GetWidth(), 
			((coords.y + cellSize.y) * spriteSize.y) == 0.0f ? 0.0f :((coords.y + cellSize.y) * spriteSize.y) / texture->GetHeight() 
		};

		return CreateShared<SubTexture2D>(texture, min, max);
	}
}