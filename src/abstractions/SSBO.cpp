#include "SSBO.h"
#include "Renderer.h"

abstractions::SSBO::SSBO() {
    GLCall(glGenBuffers(1, &m_RendererID));
}

abstractions::SSBO::~SSBO() {
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

SSBO::SSBO(const void *data, unsigned int size, GLenum usage) {
    GLCall(glGenBuffers(1, &m_RendererID));

    Load(data, size, usage);
}

void abstractions::SSBO::Load(const void *data, unsigned int size, GLenum usage) {
    GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID));
    GLCall(glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, usage));
    this->size = size;
}

void abstractions::SSBO::BindToSlot(GLuint slot) const {
    GLCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, slot, m_RendererID));
}

void abstractions::SSBO::Bind() const{
    GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID));
}

void abstractions::SSBO::UnBind() {
    GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));
}

void SSBO::GetBufferData(void *data) {
    GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID));
    GLCall(glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, size, data));
}

unsigned int SSBO::GetRendererId() {
    return m_RendererID;
}


