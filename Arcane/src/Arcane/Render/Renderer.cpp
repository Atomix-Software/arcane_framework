#include <arcpch.h>

#include "Arcane/Render/Renderer.h"

#include <glad/glad.h>

namespace Arcane
{
    void Renderer::Init()
    {
        ARC_PROFILE_FUNCTION();
        RenderCMD::Init();
    }

    void Renderer::Shutdown()
    {
        ARC_PROFILE_FUNCTION();

    }

    void Renderer::BeginScene(PerspectiveCamera& camera)
    {
        ARC_PROFILE_FUNCTION();

    }

    void Renderer::EndScene()
    {
        ARC_PROFILE_FUNCTION();
    }

}