#pragma once

#include "RenderCMD.h"

#include "Arcane/Render/Shader.h"
#include "Arcane/Render/Texture.h"
#include "Arcane/Render/SubTexture.h"
#include "Arcane/Render/Camera.h"

namespace Arcane
{

	class ARC_API Renderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(PerspectiveCamera& camera);
		static void EndScene();

		inline static RenderAPI::API GetAPI() { return RenderAPI::GetAPI(); }

	};
}