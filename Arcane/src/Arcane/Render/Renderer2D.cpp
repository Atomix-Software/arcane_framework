#include <arcpch.h>

#include "Arcane/Render/Renderer2D.h"

namespace Arcane
{
    struct QuadVertex
    {
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec2 TexCoord;
        float TexIndex;
        float TilingFactor;
    };

    struct RenderData2D
    {
        static const uint32_t Max_Quads = 20000;
        static const uint32_t Max_Vertices = Max_Quads * 4;
        static const uint32_t Max_Indices = Max_Quads * 6;
        static const uint32_t Max_TextureSlots = 32;

        Shared<VertexArray> QuadVertexArray;
        Shared<VertexBuffer> QuadVertexBuffer;

        Shared<Shader> TextureShader;
        Shared<Texture2D> WhiteTexture;

        uint32_t QuadIndexCount = 0;
        QuadVertex* QuadBufferBase = nullptr;
        QuadVertex* QuadBufferPtr = nullptr;

        std::array<Shared<Texture2D>, Max_TextureSlots> TextureSlots;
        uint32_t TextureSlotIndex = 1;

        glm::vec4 QuadVertexPositions[4];

        Renderer2D::Statistics Stats;
    };

    static RenderData2D s_Data;

    void Renderer2D::Init()
    {
        ARC_PROFILE_FUNCTION();

        s_Data.QuadVertexArray = VertexArray::Create();

        s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.Max_Vertices * sizeof(QuadVertex));
        s_Data.QuadVertexBuffer->SetLayout({
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float4, "a_Color" },
            { ShaderDataType::Float2, "a_TexCoord" },
            { ShaderDataType::Float,  "a_TexIndex" },
            { ShaderDataType::Float,  "a_TilingFactor" }
            });
        s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

        s_Data.QuadBufferBase = new QuadVertex[s_Data.Max_Vertices];

        uint32_t* quadIndices = new uint32_t[s_Data.Max_Indices];

        uint32_t offset = 0;
        for (uint32_t i = 0; i < s_Data.Max_Indices; i += 6)
        {
            quadIndices[i] = offset;
            quadIndices[i + 1] = offset + 1;
            quadIndices[i + 2] = offset + 2;

            quadIndices[i + 3] = offset + 2;
            quadIndices[i + 4] = offset + 3;
            quadIndices[i + 5] = offset;

            offset += 4;
        }

        Shared<IndexBuffer> ebo = IndexBuffer::Create(quadIndices, s_Data.Max_Indices);
        s_Data.QuadVertexArray->SetIndexBuffer(ebo);
        delete[] quadIndices;

        s_Data.WhiteTexture = Texture2D::Create(1, 1);
        uint32_t whiteTextureData = 0xffffffff;
        s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

        uint32_t samplers[RenderData2D::Max_TextureSlots];
        for (int i = 0; i < RenderData2D::Max_TextureSlots; ++i)
            samplers[i] = i;

        s_Data.TextureShader = Shader::Create("assets/shaders/Batch.glsl");
        s_Data.TextureShader->Bind();
        s_Data.TextureShader->SetIntArray("u_Textures", (int*)samplers, RenderData2D::Max_TextureSlots);

        s_Data.TextureSlots[0] = s_Data.WhiteTexture;

        s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
        s_Data.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
        s_Data.QuadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
        s_Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
    }

    void Renderer2D::Shutdown()
    {
        delete[] s_Data.QuadBufferBase;
    }

    void Renderer2D::BeginScene(OrthographicCamera& camera)
    {
        s_Data.TextureShader->Bind();
        s_Data.TextureShader->SetMat4("u_ProjectionView", camera.GetProjectionView());

        RenderCMD::EnableDepthTesting(false);
        StartBatch();
    }

    void Renderer2D::EndScene()
    {
        Flush();
        RenderCMD::EnableDepthTesting(true);
    }

    void Renderer2D::StartBatch()
    {
        s_Data.QuadIndexCount = 0;
        s_Data.QuadBufferPtr = s_Data.QuadBufferBase;

        s_Data.TextureSlotIndex = 1;
    }

    void Renderer2D::Flush()
    {
        if (s_Data.QuadIndexCount)
        {
            uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadBufferPtr - (uint8_t*)s_Data.QuadBufferBase);
            s_Data.QuadVertexBuffer->SetData(s_Data.QuadBufferBase, dataSize);

            for (uint32_t i = 0; i < s_Data.TextureSlotIndex; ++i)
                s_Data.TextureSlots[i]->Bind(i);

            s_Data.TextureShader->Bind();
            RenderCMD::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
            s_Data.Stats.DrawCalls++;
        }
    }

    void Renderer2D::NextBatch()
    {
        Flush();
        StartBatch();
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
    {
        DrawQuad({ position.x, position.y, 0.0f }, size, color);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
    {
        glm::round(position);

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
            glm::scale(glm::mat4(1.0), { size.x, size.y, 1.0f });

        DrawQuad(transform, color);
    }

    void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
    {
        DrawRotatedQuad(transform, color);
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Shared<Texture2D>& texture, const TextureProps& props)
    {
        DrawQuad({ position.x, position.y, 0.0f }, size, texture, props);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Shared<Texture2D>& texture, const TextureProps& props)
    {
        glm::round(position);

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
            glm::scale(glm::mat4(1.0), { size.x, size.y, 1.0f });

        DrawQuad(transform, texture, props);
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Shared<SubTexture2D>& subTexture, const TextureProps& props)
    {
        DrawQuad({ position.x, position.y, 0.0f }, size, subTexture, props);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Shared<SubTexture2D>& subTexture, const TextureProps& props)
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
            glm::scale(glm::mat4(1.0), { size.x, size.y, 1.0f });

        DrawRotatedQuad(transform, subTexture, props);
    }

    void Renderer2D::DrawQuad(const glm::mat4& transform, const Shared<Texture2D>& texture, const TextureProps& props)
    {
        DrawRotatedQuad(transform, texture, props);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
    {
        DrawRotatedQuad({ position.x, position.y, 0 }, size, rotation, color);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
    {
        DrawRotatedQuad(position, size, rotation, s_Data.WhiteTexture, { color });
    }

    void Renderer2D::DrawRotatedQuad(const glm::mat4& transform, const glm::vec4& color)
    {
        constexpr size_t quadVertexCount = 4;
        constexpr glm::vec2 texCoords[] = { { 0, 0 }, { 1, 0 }, { 1, 1 }, { 0, 1 } };
        constexpr float textureIndex = 0.0f;
        constexpr float tilingFactor = 1.0f;

        if (s_Data.QuadIndexCount >= RenderData2D::Max_Indices)
            NextBatch();

        for (size_t i = 0; i < quadVertexCount; ++i)
        {
            s_Data.QuadBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
            s_Data.QuadBufferPtr->Color = color;
            s_Data.QuadBufferPtr->TexCoord = texCoords[i];
            s_Data.QuadBufferPtr->TexIndex = textureIndex;
            s_Data.QuadBufferPtr->TilingFactor = tilingFactor;
            s_Data.QuadBufferPtr++;
        }

        s_Data.QuadIndexCount += 6;
        s_Data.Stats.QuadCount++;
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Shared<Texture2D>& texture, const TextureProps& props)
    {
        DrawRotatedQuad({ position.x, position.y, 0 }, size, rotation, texture, props);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Shared<Texture2D>& texture, const TextureProps& props)
    {
        glm::round(position);

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
            glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0, 0, 1 }) *
            glm::scale(glm::mat4(1.0), { size.x, size.y, 1.0f });

        DrawRotatedQuad(transform, texture, props);
    }

    void Renderer2D::DrawRotatedQuad(const glm::mat4& transform, const Shared<Texture2D>& texture, const TextureProps& props)
    {
        constexpr size_t quadVertexCount = 4;
        constexpr glm::vec2 texCoords[] = { { 0, 0 }, { 1, 0 }, { 1, 1 }, { 0, 1 } };

        if (s_Data.QuadIndexCount >= RenderData2D::Max_Indices)
            NextBatch();

        float textureIndex = 0.0f;
        for (uint32_t i = 1; i < s_Data.TextureSlotIndex; ++i)
        {
            if (*s_Data.TextureSlots[i] == *texture)
            {
                textureIndex = (float)i;
                break;
            }
        }

        if (textureIndex == 0.0f)
        {
            if (s_Data.TextureSlotIndex >= RenderData2D::Max_TextureSlots)
                NextBatch();

            textureIndex = (float)s_Data.TextureSlotIndex;
            s_Data.TextureSlots[(uint32_t)s_Data.TextureSlotIndex] = texture;
            s_Data.TextureSlotIndex++;
        }

        for (size_t i = 0; i < quadVertexCount; ++i)
        {
            s_Data.QuadBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
            s_Data.QuadBufferPtr->Color = props.Color;
            s_Data.QuadBufferPtr->TexCoord = texCoords[i];
            s_Data.QuadBufferPtr->TexIndex = textureIndex;
            s_Data.QuadBufferPtr->TilingFactor = props.TileFactor;
            s_Data.QuadBufferPtr++;
        }

        s_Data.QuadIndexCount += 6;
        s_Data.Stats.QuadCount++;
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Shared<SubTexture2D>& subTexture, const TextureProps& props)
    {
        DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, subTexture, props);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Shared<SubTexture2D>& subTexture, const TextureProps& props)
    {
        glm::round(position);

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
            glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0, 0, 1 }) *
            glm::scale(glm::mat4(1.0), { size.x, size.y, 1.0f });

        DrawRotatedQuad(transform, subTexture, props);
    }



    void Renderer2D::DrawRotatedQuad(const glm::mat4& transform, const Shared<SubTexture2D>& subTexture, const TextureProps& props)
    {
        constexpr size_t quadVertexCount = 4;
        Shared<Texture2D> texture = subTexture->GetTexture();

        if (s_Data.QuadIndexCount >= RenderData2D::Max_Indices)
            NextBatch();

        float textureIndex = 0.0f;
        for (uint32_t i = 1; i < s_Data.TextureSlotIndex; ++i)
        {
            if (*s_Data.TextureSlots[i] == *texture)
            {
                textureIndex = (float)i;
                break;
            }
        }

        if (textureIndex == 0.0f)
        {
            if (s_Data.TextureSlotIndex >= RenderData2D::Max_TextureSlots)
                NextBatch();

            textureIndex = (float)s_Data.TextureSlotIndex;
            s_Data.TextureSlots[(uint32_t)s_Data.TextureSlotIndex] = texture;
            s_Data.TextureSlotIndex++;
        }

        for (size_t i = 0; i < quadVertexCount; ++i)
        {
            s_Data.QuadBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
            s_Data.QuadBufferPtr->Color = props.Color;
            s_Data.QuadBufferPtr->TexCoord = subTexture->GetTexCoords()[i];
            s_Data.QuadBufferPtr->TexIndex = textureIndex;
            s_Data.QuadBufferPtr->TilingFactor = props.TileFactor;
            s_Data.QuadBufferPtr++;
        }

        s_Data.QuadIndexCount += 6;
        s_Data.Stats.QuadCount++;
    }

    void Renderer2D::ResetStats()
    {
        memset(&s_Data.Stats, 0, sizeof(Statistics));
    }

    Renderer2D::Statistics Renderer2D::GetStats()
    {
        return s_Data.Stats;
    }
}