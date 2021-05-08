#ifndef OPENGL_TEST_TEXTURE_H
#define OPENGL_TEST_TEXTURE_H

#include <string>
#include <vector>

namespace abstractions {
    class Texture {
    protected:
        unsigned int m_RendererID{};

        std::string m_FilePath;
        void* m_LocalBuffer{};
        int m_Width{}, m_Height{}, m_BPP{};
    public:
        Texture(int width, int height);
        explicit Texture(const std::string& path);
        explicit Texture(float *buffer, int width, int height);

        void DeleteTexture();
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
