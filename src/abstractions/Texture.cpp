#include "Texture.h"
#include "Renderer.h"
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace abstractions {
    Texture::Texture(const std::string &path)
            : m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0)
    {
        // OpenGL origin is on bottom left
        stbi_set_flip_vertically_on_load(1);
        m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);

        GenerateTexture();

        if(m_LocalBuffer)
            stbi_image_free(m_LocalBuffer);
    }

    Texture::Texture(int width, int height)
        : m_LocalBuffer(nullptr), m_Width(width), m_Height(height)
    {
        GenerateTexture();
    }

    Texture::Texture(float *buffer, int width, int height)
        : m_LocalBuffer(buffer), m_Width(width), m_Height(height)
    {
        GenerateTexture();
    }

    void Texture::GenerateTexture(){
        GLCall(glGenTextures(1, &m_RendererID));

        GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_Width, m_Height, 0, GL_RGBA, GL_FLOAT, m_LocalBuffer));
    }

    void Texture::DeleteTexture() {
        GLCall(glDeleteTextures(1, &m_RendererID));
    }

    void Texture::Bind(unsigned int slot) {
        GLCall(glActiveTexture(GL_TEXTURE0 + slot));
        GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
    }

    void Texture::Unbind() {
        GLCall(glBindTexture(GL_TEXTURE_2D, 0));
    }

    void Texture::BindImageTexture(unsigned int slot){
        GLCall(glBindImageTexture(slot, m_RendererID, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F));
    }

    std::vector<float> Texture::GetTextureImage(unsigned int slot) {
        GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

        std::vector<float> pixels(m_Width * m_Height * 4);
        GLCall(glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, &pixels[0]));

        return pixels;
    }
}