#ifndef OPENGL_TEST_SHADER_H
#define OPENGL_TEST_SHADER_H

#include <unordered_map>
#include "../vendor/glm/ext.hpp"

namespace abstractions {

    class Shader
    {
    private:
        std::unordered_map<std::string, int> m_UniformLocationCache;
    protected:
        unsigned int m_RendererID;
    public:
        ~Shader();

        void Bind() const;
        void Unbind() const;

        // Set uniforms
        void SetUniform1i(const std::string& name, int value);
        void SetUniform2f(const std::string &name, float v0, float v1);
        void SetUniform3f(const std::string& name, float v0, float v1, float f2);
        void SetUniform4f(const std::string& name, float v0, float v1, float f2, float f3);
        void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
        void SetUniform1f(const std::string &name, float v);
    private:
        int GetUniformLocation(const std::string& name);
    protected:
        std::string ParseShader(const std::string& filePath);
        unsigned int CompileShader(unsigned int type, const std::string &source);
    };

}

#endif //OPENGL_TEST_SHADER_H
