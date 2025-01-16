#include <arcpch.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include <glm/gtc/type_ptr.hpp>

namespace Arcane
{
    static GLenum ShaderTypeFromString(const std::string& type)
    {
        if (type == "vertex") return GL_VERTEX_SHADER;
        if (type == "fragment" || type == "pixel") return GL_FRAGMENT_SHADER;

        ARC_CORE_ASSERT(false, "Unknown Shader type!");
        return GL_NONE;
    }

    OpenGLShader::OpenGLShader(const std::string& shaderFile)
    {
        ARC_PROFILE_FUNCTION();
        std::string src = ReadFile(shaderFile);
        auto sources = Preprocess(src);
        Compile(sources);

        auto lastSlash = shaderFile.find_last_of("/\\");
        lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
        auto lastDot = shaderFile.rfind('.');
        auto count = lastDot == std::string::npos ? shaderFile.size() - lastSlash : lastDot - lastSlash;
        m_Name = shaderFile.substr(lastSlash, count);
    }

    OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertSrc, const std::string& fragSrc) :
        m_Name(name)
	{
        ARC_PROFILE_FUNCTION();
        std::unordered_map<GLenum, std::string> sources;
        sources[GL_VERTEX_SHADER] = vertSrc;
        sources[GL_FRAGMENT_SHADER] = fragSrc;

        Compile(sources);
	}

	OpenGLShader::~OpenGLShader()
	{
        ARC_PROFILE_FUNCTION();
        glDeleteProgram(m_RendererId);
	}

    std::string OpenGLShader::ReadFile(const std::string& file)
    {
        ARC_PROFILE_FUNCTION();
        std::ifstream in(file, std::ios::in | std::ios::binary);
        ARC_CORE_ASSERT(in, "Failed to load shader file {0}", file);

        std::string result;
        in.seekg(0, std::ios::end);
        result.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&result[0], result.size());
        in.close();

        return result;
    }

    std::unordered_map<GLenum, std::string> OpenGLShader::Preprocess(const std::string& source)
    {
        ARC_PROFILE_FUNCTION();
        std::unordered_map<GLenum, std::string> sources;

        const char* typeToken = "#type";
        size_t typeTokenLength = strlen(typeToken);
        size_t pos = source.find(typeToken, 0);
        while (pos != std::string::npos)
        {
            size_t eol = source.find_first_of("\r\n", pos);
            ARC_CORE_ASSERT(eol != std::string::npos, "Syntax Error!");
            size_t begin = pos + typeTokenLength + 1;
            std::string type = source.substr(begin, eol - begin);
            ARC_CORE_ASSERT(type == "vertex" || type == "fragment" || type == "pixel", "Invalid Shader Type Specification!");

            size_t nextLinePos = source.find_first_not_of("\r\n", eol);
            pos = source.find(typeToken, nextLinePos);
            sources[ShaderTypeFromString(type)] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
        }

        return sources;
    }

    void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& srcs)
    {
        ARC_PROFILE_FUNCTION();
        GLuint program = glCreateProgram();
        ARC_CORE_ASSERT(srcs.size() <= 2, "We only support Vertex and Fragment shaders for now!");
        std::array<GLuint, 2> glShaderIds;

        int idIndex = 0;
        for (auto& [type, src] : srcs)
        {
            GLuint shader = glCreateShader(type); 
            const GLchar* source = (const GLchar*)src.c_str();
            glShaderSource(shader, 1, &source, NULL);
            glCompileShader(shader);

            GLint success = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (success == GL_FALSE)
            {
                char* infoLog = (char*)malloc(5120);
                GLint maxLength = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
                glGetShaderInfoLog(shader, maxLength, &maxLength, infoLog);

                ARC_CORE_ERROR("Shader Compiler Error: \n{0}", infoLog);
                glDeleteShader(shader);

                free(infoLog);
                break;
            }

            glAttachShader(program, shader);
            glShaderIds[idIndex++] = shader;
        }

        m_RendererId = program;
        glLinkProgram(m_RendererId);
        GLint success = 0;
        glGetProgramiv(m_RendererId, GL_LINK_STATUS, &success);
        if (success == GL_FALSE)
        {
            char* infoLog = (char*)malloc(5120);
            GLint maxLength = 0;
            glGetProgramiv(m_RendererId, GL_INFO_LOG_LENGTH, &maxLength);
            glGetProgramInfoLog(m_RendererId, maxLength, &maxLength, &infoLog[0]);

            ARC_CORE_ERROR("Shader Linking Error: \n{0}", infoLog);
            glDeleteProgram(m_RendererId);

            for (auto& shader : glShaderIds)
            {
                glDetachShader(m_RendererId, shader);
                glDeleteShader(shader);
            }

            free(infoLog);
            return;
        }

        for (auto& shader : glShaderIds)
        {
            glDetachShader(m_RendererId, shader);
            glDeleteShader(shader);
        }
    }

    void OpenGLShader::Bind() const
    {
        ARC_PROFILE_FUNCTION();
        glUseProgram(m_RendererId);
    }

    void OpenGLShader::Unbind() const
    {
        ARC_PROFILE_FUNCTION();
        glUseProgram(0);
    }

    void OpenGLShader::UploadUniformBool(const std::string& name, bool value) const
    {
        UploadUniformInt(name, value ? GL_TRUE : GL_FALSE);
    }

    void OpenGLShader::UploadUniformInt(const std::string& name, int value) const
    {
        ARC_PROFILE_FUNCTION();
        GLint location = glGetUniformLocation(m_RendererId, name.c_str());
        glUniform1i(location, value);
    }

    void OpenGLShader::UploadUniformIntv(const std::string& name, int* values, uint32_t count) const
    {
        ARC_PROFILE_FUNCTION();
        GLint location = glGetUniformLocation(m_RendererId, name.c_str());
        glUniform1iv(location, count, values);
    }

    void OpenGLShader::UploadUniformInt2(const std::string& name, const glm::ivec2& value) const
    {
        ARC_PROFILE_FUNCTION();
        GLint location = glGetUniformLocation(m_RendererId, name.c_str());
        glUniform2i(location, value.x, value.y);
    }

    void OpenGLShader::UploadUniformInt3(const std::string& name, const glm::ivec3& value) const
    {
        ARC_PROFILE_FUNCTION();
        GLint location = glGetUniformLocation(m_RendererId, name.c_str());
        glUniform3i(location, value.x, value.y, value.z);
    }

    void OpenGLShader::UploadUniformInt4(const std::string& name, const glm::ivec4& value) const
    {
        ARC_PROFILE_FUNCTION();
        GLint location = glGetUniformLocation(m_RendererId, name.c_str());
        glUniform4i(location, value.x, value.y, value.z, value.w);
    }

    void OpenGLShader::UploadUniformFloat(const std::string& name, float value) const
    {
        ARC_PROFILE_FUNCTION();
        GLint location = glGetUniformLocation(m_RendererId, name.c_str());
        glUniform1f(location, value);
    }

    void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value) const
    {
        ARC_PROFILE_FUNCTION();
        GLint location = glGetUniformLocation(m_RendererId, name.c_str());
        glUniform2f(location, value.x, value.y);
    }

    void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& value) const
    {
        ARC_PROFILE_FUNCTION();
        GLint location = glGetUniformLocation(m_RendererId, name.c_str());
        glUniform3f(location, value.x, value.y, value.z);
    }

    void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value) const
    {
        ARC_PROFILE_FUNCTION();
        GLint location = glGetUniformLocation(m_RendererId, name.c_str());
        glUniform4f(location, value.x, value.y, value.z, value.w);
    }

    void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& value) const
    {
        ARC_PROFILE_FUNCTION();
        GLint location = glGetUniformLocation(m_RendererId, name.c_str());
        glUniformMatrix3fv(location, 1, false, glm::value_ptr(value));
    }

    void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& value) const
    {
        ARC_PROFILE_FUNCTION();
        GLint location = glGetUniformLocation(m_RendererId, name.c_str());
        glUniformMatrix4fv(location, 1, false, glm::value_ptr(value));
    }
}