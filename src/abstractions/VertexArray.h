#ifndef OPENGL_TEST_VERTEXARRAY_H
#define OPENGL_TEST_VERTEXARRAY_H

#include "VertexBuffer.h"


namespace abstractions {
    class VertexBufferLayout;

    class VertexArray
    {
    private:
        unsigned int m_RendererId;
    public:
        VertexArray();
        ~VertexArray();

        void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

        void Bind() const;
        void Unbind() const;
    };
}


#endif //OPENGL_TEST_VERTEXARRAY_H
