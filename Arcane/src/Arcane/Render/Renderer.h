#pragma once

#include "RenderCMD.h"

#include "Arcane/Render/Shader.h"
#include "Arcane/Render/Texture.h"
#include "Arcane/Render/SubTexture.h"
#include "Arcane/Render/Camera.h"

namespace Arcane
{
	struct ARC_API RenderData
	{
		Shared<Texture2D> Texture = nullptr;
		glm::mat4 Transform = glm::mat4(1.0f);
		glm::vec3 Color = glm::vec3(1.0f);
	};

	class ARC_API Renderer
	{
	public:
		struct SceneData
		{
			std::vector<Shared<Shader>> Shaders;
			std::unordered_map<Shared<Shader>, std::vector<std::pair<Shared<VertexArray>, RenderData*>>> Objects;

			glm::mat4 ProjectionView;

			int ShaderCount = 0;
			int ObjectCount = 0;

			bool Rendering = false;
		};

	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void ResizeViewport(uint32_t width, uint32_t height);

		static void Submit(const Shared<Shader>& shader, const Shared<VertexArray>& vao, const Shared<Texture2D>& texture, const glm::vec3 position, const glm::vec3 color = glm::vec3(1.0f), float rotation = 0.0f, float scale = 1.0f);
		static void Submit(const Shared<Shader>& shader, const Shared<VertexArray>& vao, const Shared<Texture2D>& texture, const glm::vec3 color = glm::vec3(1.0f), const glm::mat4& transform = glm::mat4(1.0));

		inline static RenderAPI::API GetAPI() { return RenderAPI::GetAPI(); }

		inline static const SceneData& GetSceneData() { return *m_SceneData; }

	private:
		static SceneData* m_SceneData;

	};

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

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Shared<Texture2D>& texture, const TextureProps& props = TextureProps());
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Shared<Texture2D>& texture, const TextureProps& props = TextureProps());

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Shared<SubTexture2D>& subTexture, const TextureProps& props = TextureProps());
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Shared<SubTexture2D>& subTexture, const TextureProps& props = TextureProps());

		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);

		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Shared<Texture2D>& texture, const TextureProps& props = TextureProps());
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Shared<Texture2D>& texture, const TextureProps& props = TextureProps());

		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Shared<SubTexture2D>& subTexture, const TextureProps& props = TextureProps());
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Shared<SubTexture2D>& subTexture, const TextureProps& props = TextureProps());


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