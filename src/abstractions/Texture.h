#ifndef OPENGL_TEST_TEXTURE_H
#define OPENGL_TEST_TEXTURE_H

#include <string>

namespace abstractions {
    class Texture {
    private:
        unsigned int m_RendererID;
        std::string m_FilePath;
        unsigned char* m_LocalBuffer;
        int m_Width, m_Height, m_BPP;
    public:
        Texture(const std::string& path);
        ~Texture();

        void Bind(unsigned int slot = 0);
        void Unbind();

        inline int GetWidth() const { return m_Width; }
        inline int GetHeight() const { return m_Height; }
    };
}

#endif //OPENGL_TEST_TEXTURE_H
