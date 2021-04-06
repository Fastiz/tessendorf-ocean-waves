#ifndef OPENGL_TEST_SHADER_H
#define OPENGL_TEST_SHADER_H

#include <unordered_map>
#include "../vendor/glm/ext.hpp"

namespace abstractions {
    struct ShaderProgramSource
    {
        std::string VertexSource;
        std::string FragmentSource;
    };

    class Shader
    {
    private:
        std::string m_FilePath;
        unsigned int m_RendererID;
        std::unordered_map<std::string, int> m_UniformLocationCache;
    public:
        Shader(const std::string& filepath);
        ~Shader();

        void Bind() const;
        void Unbind() const;

        // Set uniforms
        void SetUniform1i(const std::string& name, int value);
        void SetUniform3f(const std::string& name, float v0, float v1, float f2);
        void SetUniform4f(const std::string& name, float v0, float v1, float f2, float f3);
        void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
        void SetUniform1f(const std::string &name, float v);
    private:
        int GetUniformLocation(const std::string& name);

        ShaderProgramSource ParseShader(const std::string &filePath);
        unsigned int CompileShader(unsigned int type, const std::string &source);
        unsigned int CreateShader(const std::string &vertexShader, const std::string &fragmentShader);
    };

}

#endif //OPENGL_TEST_SHADER_H
