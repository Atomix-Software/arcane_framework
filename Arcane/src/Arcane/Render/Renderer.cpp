#include <arcpch.h>

#include "Arcane/Render/Renderer.h"

#include <glad/glad.h>

namespace Arcane
{
    /* 3D Renderer */
	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData();

    void Renderer::Init()
    {
        ARC_PROFILE_FUNCTION();
        RenderCMD::Init();
    }

    void Renderer::Shutdown()
    {
        ARC_PROFILE_FUNCTION();
    }

    void Renderer::BeginScene(OrthographicCamera& camera)
    {
        ARC_PROFILE_FUNCTION();
        ARC_CORE_ASSERT(!m_SceneData->Rendering, "Must call EndScene before BeginScene!");

        m_SceneData->ShaderCount = 0;
        m_SceneData->ObjectCount = 0;

        m_SceneData->ProjectionView = camera.GetProjectionView();
        m_SceneData->Rendering = true;
    }

    void Renderer::EndScene()
    {
        ARC_PROFILE_FUNCTION();
        ARC_CORE_ASSERT(m_SceneData->Rendering, "Must call BeginScene before EndScene!");

        for (auto& shader : m_SceneData->Shaders)
        {
            shader->Bind();
            shader->SetMat4("u_ProjectionView", m_SceneData->ProjectionView);
            const auto& objects = m_SceneData->Objects[shader];
            for (const auto& [vao, data] : objects)
            {
                int textureCount = 0;
                if (data->Texture != nullptr)
                {
                    shader->SetInt("u_Texture", textureCount);
                    data->Texture->Bind(textureCount);
                    textureCount++;
                }
                else
                {
                    shader->SetFloat3("u_Color", data->Color);
                }

                shader->SetMat4("u_Model", data->Transform);

                vao->Bind();
                RenderCMD::DrawIndexed(vao);
            }
        }

        m_SceneData->Shaders.clear();
        m_SceneData->Objects.clear();
        m_SceneData->Rendering = false;
    }

    void Renderer::ResizeViewport(uint32_t width, uint32_t height)
    {
        ARC_PROFILE_FUNCTION();
        RenderCMD::SetViewport(0, 0, width, height);
    }

    void Renderer::Submit(const Shared<Shader>& shader, const Shared<VertexArray>& vao, const Shared<Texture2D>& texture, const glm::vec3 position, const glm::vec3 color, float rotation, float scale)
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
            glm::rotate(glm::mat4(1.0), glm::radians(rotation), glm::vec3(0, 0, 1)) *
            glm::scale(glm::mat4(1.0), glm::vec3(scale));

        Submit(shader, vao, texture, color, transform);
    }

    void Renderer::Submit(const Shared<Shader>& shader, const Shared<VertexArray>& vao, const Shared<Texture2D>& texture, const glm::vec3 color, const glm::mat4& transform)
    {
        ARC_PROFILE_FUNCTION();
        ARC_CORE_ASSERT(m_SceneData->Rendering, "Must call BeginScene before Submit!");

        // Add shader if not already in the list
        if (std::find(m_SceneData->Shaders.begin(), m_SceneData->Shaders.end(), shader) == m_SceneData->Shaders.end())
        {
            m_SceneData->Shaders.push_back(shader);
            m_SceneData->ShaderCount++;
        }

        RenderData* data = new RenderData();
        data->Texture = texture;
        data->Transform = transform;
        data->Color = color;

        // Associate the VAO and transform with the shader
        m_SceneData->Objects[shader].emplace_back(vao, data);
        m_SceneData->ObjectCount++;
    }

}