#ifndef OPENGL_TEST_RENDERER_H
#define OPENGL_TEST_RENDERER_H

#include <GL/glew.h>
#include <csignal>
#include "VertexArray.h"
#include "Shader.h"
#include "IndexBuffer.h"
#include <csignal>

#ifndef SIGTRAP
#define SIGTRAP 5
#endif

#define ASSERT(x) if (!(x)) raise(SIGTRAP);
#define GLCall(x) GLClearError(); \
    x;                            \
    ASSERT(GLLogCall(#x, __FILE__, __LINE__));

using namespace abstractions;


namespace abstractions {
    void GLClearError();

    bool GLLogCall(const char* function, const char* file, int line);

    class Renderer {
    public:
        void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
        void DrawArrays(const VertexArray &va, const Shader &shader, int first, int count) const;
        void Clear() const;
    };
}

#endif //OPENGL_TEST_RENDERER_H
