#include <arcpch.h>

#include "Arcane/Render/Renderer.h"

#include <glad/glad.h>
#include <glm/ext.hpp>

namespace Arcane
{
    struct RenderData3D
    {
        std::map<Shared<Shader>, std::vector<std::pair<Shared<Model>, glm::mat4>>> ShaderModelMap;
        std::vector<Shared<Shader>> Shaders;
    };

    static RenderData3D s_Data;

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

        for (auto shader : s_Data.Shaders)
        {
            shader->Bind();
            shader->SetMat4("u_ProjectionView", camera.GetProjectionView());
        }
    }

    void Renderer::EndScene()
    {
        ARC_PROFILE_FUNCTION();

        for (auto& shader : s_Data.Shaders)
        {
            if (s_Data.ShaderModelMap.empty() || !s_Data.ShaderModelMap.contains(shader))
                continue;

            shader->Bind();
            for (auto& [model, transform] : s_Data.ShaderModelMap[shader])
            {
                shader->SetFloat3("u_Color", model->GetMaterial().Color);

                bool textured = model->GetMaterial().Texture != nullptr;
                shader->SetBool("u_Textured", textured);
                shader->SetInt("u_Texture", 0);

                if (textured)
                    model->GetMaterial().Texture->Bind();

                shader->SetMat4("u_Model", transform);
                RenderCMD::DrawIndexed(model->GetMesh()->GetVAO());
            }
            s_Data.ShaderModelMap[shader].clear();
        }

    }

    void Renderer::DrawCube(Material& material, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0), position) *
            glm::rotate(glm::mat4(1.0), glm::radians(rotation.x), { 1, 0, 0 }) *
            glm::rotate(glm::mat4(1.0), glm::radians(rotation.y), { 0, 1, 0 }) *
            glm::rotate(glm::mat4(1.0), glm::radians(rotation.z), { 0, 0, 1 }) *
            glm::scale(glm::mat4(1.0), scale);

        DrawCube(material, transform);
    }

    void Renderer::DrawCube(Material& material, glm::mat4& transform)
    {
        Shared<Model> cube = Shared<Model>(MakeCube(material));
        s_Data.ShaderModelMap[material.Shader].push_back(std::pair<Shared<Model>, glm::mat4>(cube, transform));
    }

    void Renderer::DrawPlane(Material& material, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0), position) *
            glm::rotate(glm::mat4(1.0), glm::radians(rotation.x), { 1, 0, 0 }) *
            glm::rotate(glm::mat4(1.0), glm::radians(rotation.y), { 0, 1, 0 }) *
            glm::rotate(glm::mat4(1.0), glm::radians(rotation.z), { 0, 0, 1 }) *
            glm::scale(glm::mat4(1.0), scale);

        DrawPlane(material, transform);
    }

    void Renderer::DrawPlane(Material& material, glm::mat4& transform)
    {
        Shared<Model> plane = Shared<Model>(MakePlane(material));
        s_Data.ShaderModelMap[material.Shader].push_back(std::pair<Shared<Model>, glm::mat4>(plane, transform));
    }

    void Renderer::AddShader(Shared<Shader> shader)
    {
        s_Data.Shaders.push_back(shader);
    }

    Model* Renderer::MakePlane(Material& material)
    {
        std::vector<float> vertices = {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
             0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
             0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
            -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,

             0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
            -0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
             0.5f,  0.5f, 0.0f, 0.0f, 1.0f
        };

        std::vector<uint32_t> indices = {
            0, 1, 2,
            0, 2, 3,
            4, 5, 6,
            4, 6, 7
        };

        BufferLayout layout = {
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float2, "a_TexCoord" }
        };

        Shared<Mesh> mesh = CreateShared<Mesh>(vertices, indices, layout);
        return new Model(material, mesh);
    }

    Model* Renderer::MakeCube(Material& material)
    {
        std::vector<float> vertices = {
               -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,
                0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
                0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
               -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,

                0.5f, -0.5f,  0.0f, 0.0f, 0.0f,
                0.5f, -0.5f, -1.0f, 1.0f, 0.0f,
                0.5f,  0.5f, -1.0f, 1.0f, 1.0f,
                0.5f,  0.5f,  0.0f, 0.0f, 1.0f,

                0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
               -0.5f, -0.5f, -1.0f, 1.0f, 0.0f,
               -0.5f,  0.5f, -1.0f, 1.0f, 1.0f,
                0.5f,  0.5f, -1.0f, 0.0f, 1.0f,

               -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
               -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
               -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
               -0.5f,  0.5f, -1.0f, 0.0f, 1.0f,

               -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,
                0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
                0.5f,  0.5f, -1.0f, 1.0f, 1.0f,
               -0.5f,  0.5f, -1.0f, 0.0f, 1.0f,

               -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
                0.5f, -0.5f, -1.0f, 1.0f, 0.0f,
                0.5f, -0.5f,  0.0f, 1.0f, 1.0f,
               -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
        };

        std::vector<uint32_t> indices = {
            0, 1, 2,
            0, 2, 3,

            4, 5, 6,
            4, 6, 7,

            8, 9, 10,
            8, 10, 11,

            12, 13, 14,
            12, 14, 15,

            16, 17, 18,
            16, 18, 19,

            20, 21, 22,
            20, 22, 23
        };

        BufferLayout layout = {
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float2, "a_TexCoord" }
        };

        Shared<Mesh> mesh = CreateShared<Mesh>(vertices, indices, layout);
        return new Model(material, mesh);
    }

}