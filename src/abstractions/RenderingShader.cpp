#include <fstream>
#include "RenderingShader.h"
#include <sstream>
#include "Renderer.h"

namespace abstractions {
    RenderingShader::RenderingShader(const std::string &vertexShaderPath, const std::string &fragmentShaderPath) {
        std::string ver = ParseShader(vertexShaderPath),
            frag = ParseShader(fragmentShaderPath);

        m_RendererID = CreateShader(ver, frag);
    }

    unsigned int RenderingShader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
    {
        GLCall(unsigned int program = glCreateProgram());
        unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
        unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

        GLCall(glAttachShader(program, vs));
        GLCall(glAttachShader(program, fs));
        GLCall(glLinkProgram(program));
        GLCall(glValidateProgram(program));

        GLCall(glDeleteShader(vs));
        GLCall(glDeleteShader(fs));

        return program;
    }

}