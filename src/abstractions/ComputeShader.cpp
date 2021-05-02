#include "ComputeShader.h"
#include "Renderer.h"

namespace abstractions {

    ComputeShader::ComputeShader(const std::string &computeShaderPath) {
        std::string shader = ParseShader(computeShaderPath);

        m_RendererID = CreateShader(shader);
    }

    unsigned int ComputeShader::CreateShader(const std::string& computeShader)
    {
        GLCall(unsigned int program = glCreateProgram());
        unsigned int shader = CompileShader(GL_COMPUTE_SHADER, computeShader);

        GLCall(glAttachShader(program, shader));
        GLCall(glLinkProgram(program));
        GLCall(glValidateProgram(program));

        GLCall(glDeleteShader(shader));

        return program;
    }
}