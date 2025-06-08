#include <arcpch.h>

#include "Arcane/Render/Shader.h"

#include "Arcane/Render/RenderAPI.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Arcane
{
	Shared<Shader> Shader::Create(const std::string& shaderFile)
	{
		switch (RenderAPI::GetAPI())
		{
		case RenderAPI::API::None: ARC_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
		case RenderAPI::API::OpenGL: return CreateShared<OpenGLShader>(shaderFile);
		}

		ARC_CORE_ASSERT(false, "Unknown API selected!");
		return nullptr;
	}

	Shared<Shader> Shader::Create(const std::string& name, const std::string& vertSrc, const std::string& fragSrc)
	{
		switch (RenderAPI::GetAPI())
		{
		case RenderAPI::API::None: ARC_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
		case RenderAPI::API::OpenGL: return CreateShared<OpenGLShader>(name, vertSrc, fragSrc);
		}

		ARC_CORE_ASSERT(false, "Unknown API selected!");
		return nullptr;
	}

	void ShaderLibrary::Add(const Shared<Shader>& shader)
	{
		auto& name = shader->GetName();
		ARC_CORE_ASSERT(!Exists(name), "Shader already loaded!");
		m_Shaders[name] = shader;
	}

	void ShaderLibrary::Add(const std::string& name, const Shared<Shader>& shader)
	{
		ARC_CORE_ASSERT(!Exists(name), "Shader already loaded!");
		m_Shaders[name] = shader;
	}

	Shared<Shader> ShaderLibrary::Load(const std::string& filePath)
	{
		auto shader = Shader::Create(filePath);
		Add(shader);
		return shader;
	}

	Shared<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filePath)
	{
		auto shader = Shader::Create(filePath);
		Add(name, shader);
		return shader;
	}

	Shared<Shader> ShaderLibrary::Get(const std::string& name)
	{
		ARC_CORE_ASSERT(Exists(name), "Shader not loaded!");
		return m_Shaders[name];
	}
	bool ShaderLibrary::Exists(const std::string& name) const
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}
}