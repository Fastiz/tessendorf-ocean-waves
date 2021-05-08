#include <iostream>
#include <fstream>
#include <sstream>
#include "Shader.h"
#include "Renderer.h"

namespace abstractions {
    Shader::~Shader() {
        GLCall(glDeleteProgram(m_RendererID));
    }

    void Shader::Bind() const {
        GLCall(glUseProgram(m_RendererID));
    }

    void Shader::Unbind() const {
        GLCall(glUseProgram(0));
    }

    void Shader::SetUniform1i(const std::string &name, int value) {
        GLCall(glUniform1i(GetUniformLocation(name), value));
    }

    void Shader::SetUniform3f(const std::string &name, float v0, float v1, float v2) {
        GLCall(glUniform3f(GetUniformLocation(name), v0, v1, v2));
    }

    void Shader::SetUniform4f(const std::string &name, float v0, float v1, float v2, float v3) {
        GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
    }

    void Shader::SetUniform2f(const std::string &name, float v0, float v1) {
        GLCall(glUniform2f(GetUniformLocation(name), v0, v1));
    }

    void Shader::SetUniformMat4f(const std::string &name, const glm::mat4 &matrix) {
        GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
    }

    void Shader::SetUniform1f(const std::string &name, float v){
        GLCall(glUniform1f(GetUniformLocation(name), v));
    }

    int Shader::GetUniformLocation(const std::string &name) {
        if(m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
            return m_UniformLocationCache[name];

        GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()))

        if (location == -1)
            std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;

        m_UniformLocationCache[name] = location;

        return location;
    }

    unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
    {
        GLCall(unsigned int id = glCreateShader(type));
        const char* src = source.c_str();
        GLCall(glShaderSource(id, 1, &src, nullptr));
        GLCall(glCompileShader(id));

        int result;
        GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
        if(result == GL_FALSE)
        {
            int length;
            GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
            char* message = (char*) alloca(length * sizeof(char));
            GLCall(glGetShaderInfoLog(id, length, &length, message));
            std::cout << "Failed to compile shader!" <<
                      (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
            std::cout << message << std::endl;
            GLCall(glDeleteShader(id));
            return 0;
        }

        return id;
    }

    std::string Shader::ParseShader(const std::string& filePath){
        std::ifstream stream(filePath);

        std::string line;
        std::stringstream ss;
        while(getline(stream, line))
        {
            ss << line << '\n';
        }

        return ss.str();
    }

}