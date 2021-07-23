#ifndef OCEAN_WATER_SIMULATION_SSBO_H
#define OCEAN_WATER_SIMULATION_SSBO_H

#include <GL/glew.h>

namespace abstractions {
    class SSBO {
    private:
        unsigned int m_RendererID{};
        unsigned int size;
    public:
        SSBO();

        ~SSBO();

        SSBO(const void *data, unsigned int size, GLenum usage);

        void Load(const void *data, unsigned int size, GLenum usage);

        void BindToSlot(GLuint slot) const;

        static void UnBind();

        void Bind() const;

        void GetBufferData(void* data);

        unsigned int GetRendererId();

        unsigned int GetSize() const;
    };
}


#endif //OCEAN_WATER_SIMULATION_SSBO_H
