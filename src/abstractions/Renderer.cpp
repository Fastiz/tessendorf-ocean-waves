#include "Renderer.h"

#include <iostream>

namespace abstractions {
    void GLClearError()
    {
        while(glGetError() != GL_NO_ERROR);
    }

    bool GLLogCall(const char* function, const char* file, int line)
    {
        bool flag = true;
        while(GLenum error = glGetError())
        {
            std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file << ": " << line << std::endl;
            flag = false;
        }
        return flag;
    }

    void Renderer::Draw(const VertexArray &va, const IndexBuffer& ib, const Shader &shader) const
    {
        shader.Bind();
        va.Bind();
        ib.Bind();

        GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
    }

    void Renderer::Clear() const {
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    }

    void Renderer::DrawArrays(const VertexArray &va, const Shader &shader, int first, int count) const {
        shader.Bind();
        va.Bind();

        GLCall(glDrawArrays(GL_TRIANGLES, first, count));
    }

}

