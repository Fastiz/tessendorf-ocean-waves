#ifndef OPENGL_TEST_TEXTURE_H
#define OPENGL_TEST_TEXTURE_H

#include <string>
#include <vector>

namespace abstractions {
    class Texture {
    protected:
        unsigned int m_RendererID;

        std::string m_FilePath;
        unsigned char* m_LocalBuffer;
        int m_Width, m_Height, m_BPP;
    public:
        Texture(int width, int height);
        Texture(const std::string& path);
        ~Texture();

        void Bind(unsigned int slot = 0);
        void Unbind();

        void BindImageTexture(unsigned int slot=0);
        std::vector<float> GetTextureImage(unsigned int slot=0);

        inline int GetWidth() const { return m_Width; }
        inline int GetHeight() const { return m_Height; }

    protected:
        void GenerateTexture();
    };
}

#endif //OPENGL_TEST_TEXTURE_H
