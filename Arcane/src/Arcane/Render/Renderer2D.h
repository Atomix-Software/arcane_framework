#pragma once

#include "RenderCMD.h"

#include "Arcane/Render/Shader.h"
#include "Arcane/Render/Texture.h"
#include "Arcane/Render/SubTexture.h"
#include "Arcane/Render/Camera.h"

namespace Arcane
{
	class ARC_API Renderer2D
	{
	public:
		struct TextureProps
		{
			glm::vec4 Color = glm::vec4(1.0f);
			float TileFactor = 1.0f;
		};

	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();
		static void Flush();

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);

		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Shared<Texture2D>& texture, const TextureProps& props = TextureProps());
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Shared<Texture2D>& texture, const TextureProps& props = TextureProps());

		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Shared<Texture2D>& texture, const TextureProps& props = TextureProps());
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Shared<Texture2D>& texture, const TextureProps& props = TextureProps());

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Shared<SubTexture2D>& subTexture, const TextureProps& props = TextureProps());
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Shared<SubTexture2D>& subTexture, const TextureProps& props = TextureProps());

		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Shared<SubTexture2D>& subTexture, const TextureProps& props = TextureProps());
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Shared<SubTexture2D>& subTexture, const TextureProps& props = TextureProps());

		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);
		static void DrawQuad(const glm::mat4& transform, const Shared<Texture2D>& texture, const TextureProps& props = TextureProps());
		static void DrawQuad(const glm::mat4& transform, const Shared<SubTexture2D>& subTexture, const TextureProps& props = TextureProps());

		static void DrawRotatedQuad(const glm::mat4& transform, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::mat4& transform, const Shared<Texture2D>& texture, const TextureProps& props = TextureProps());
		static void DrawRotatedQuad(const glm::mat4& transform, const Shared<SubTexture2D>& subTexture, const TextureProps& props = TextureProps());


		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			uint32_t GetTotalVertexCount() const { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() const { return QuadCount * 6; }
		};

		static void ResetStats();
		static Statistics GetStats();

	private:
		static void StartBatch();
		static void NextBatch();

	};
}