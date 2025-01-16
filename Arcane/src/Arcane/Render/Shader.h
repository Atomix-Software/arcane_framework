#pragma once

#include "Arcane/Core/Base.h"

#include <string>
#include <glm/glm.hpp>

namespace Arcane
{
	class ARC_API Shader
	{
	public:
		Shader() = default;
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetBool(const std::string& name, bool value) const = 0;

		virtual void SetInt(const std::string& name, int value) const = 0;
		virtual void SetIntArray(const std::string& name, int* values, uint32_t count) const = 0;
		virtual void SetInt2(const std::string& name, const glm::ivec2& value) const = 0;
		virtual void SetInt3(const std::string& name, const glm::ivec3& value) const = 0;
		virtual void SetInt4(const std::string& name, const glm::ivec4& value) const = 0;

		virtual void SetFloat(const std::string& name, float value) const = 0;
		virtual void SetFloat2(const std::string& name, const glm::vec2& value) const = 0;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) const = 0;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) const = 0;

		virtual void SetMat3(const std::string& name, const glm::mat3& value) const = 0;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) const = 0;

		virtual const std::string GetName() const = 0;

		virtual bool operator==(Shader& other) = 0;

		static Shared<Shader> Create(const std::string& shaderFile);
		static Shared<Shader> Create(const std::string& name, const std::string& vertSrc, const std::string& fragSrc);
	};

	class ShaderLibrary
	{
	public:
		void Add(const Shared<Shader>& shader);
		void Add(const std::string& name, const Shared<Shader>& shader);

		Shared<Shader> Load(const std::string& filePath);
		Shared<Shader> Load(const std::string& name, const std::string& filePath);

		Shared<Shader> Get(const std::string& name);

		bool Exists(const std::string& name) const;

	private:
		std::unordered_map<std::string, Shared<Shader>> m_Shaders;

	};
}