#pragma once

#include "Arcane/Render/Shader.h"
#include <glad/glad.h>

namespace Arcane
{
	class ARC_API OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& shaderFile);
		OpenGLShader(const std::string& name, const std::string& vertSrc, const std::string& fragSrc);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		inline virtual void SetBool(const std::string& name, bool value) const override { UploadUniformBool(name, value); }

		inline virtual void SetInt(const std::string& name, int value) const override { UploadUniformInt(name, value); }
		virtual void SetIntArray(const std::string& name, int* values, uint32_t count) const { UploadUniformIntv(name, values, count); };
		inline virtual void SetInt2(const std::string& name, const glm::ivec2& value) const override { UploadUniformInt2(name, value); }
		inline virtual void SetInt3(const std::string& name, const glm::ivec3& value) const override { UploadUniformInt3(name, value); }
		inline virtual void SetInt4(const std::string& name, const glm::ivec4& value) const override { UploadUniformInt4(name, value); }

		inline virtual void SetFloat(const std::string& name, float value) const override { UploadUniformFloat(name, value); }
		inline virtual void SetFloat2(const std::string& name, const glm::vec2& value) const override { UploadUniformFloat2(name, value); }
		inline virtual void SetFloat3(const std::string& name, const glm::vec3& value) const override { UploadUniformFloat3(name, value); }
		inline virtual void SetFloat4(const std::string& name, const glm::vec4& value) const override { UploadUniformFloat4(name, value); }

		inline virtual void SetMat3(const std::string& name, const glm::mat3& value) const override { UploadUniformMat3(name, value); }
		inline virtual void SetMat4(const std::string& name, const glm::mat4& value) const override { UploadUniformMat4(name, value); }

		virtual const std::string GetName() const override { return m_Name; }

		void UploadUniformBool(const std::string& name, bool value)	const;

		void UploadUniformInt(const std::string& name, int value) const;
		void UploadUniformIntv(const std::string& name, int* values, uint32_t count) const;
		void UploadUniformInt2(const std::string& name, const glm::ivec2& value) const;
		void UploadUniformInt3(const std::string& name, const glm::ivec3& value) const;
		void UploadUniformInt4(const std::string& name, const glm::ivec4& value) const;

		void UploadUniformFloat(const std::string& name, float value) const;
		void UploadUniformFloat2(const std::string& name, const glm::vec2& value) const;
		void UploadUniformFloat3(const std::string& name, const glm::vec3& value) const;
		void UploadUniformFloat4(const std::string& name, const glm::vec4& value) const;

		void UploadUniformMat3(const std::string& name, const glm::mat3& value) const;
		void UploadUniformMat4(const std::string& name, const glm::mat4& value) const;

		inline virtual bool operator==(Shader& other) override { return m_RendererId == static_cast<OpenGLShader&>(other).m_RendererId; }
	private:
		std::string ReadFile(const std::string& file);

		std::unordered_map<GLenum, std::string> Preprocess(const std::string& source);
		void Compile(const std::unordered_map<GLenum, std::string>& srcs);

	private:
		std::string m_Name;
		uint32_t m_RendererId;

	};
}