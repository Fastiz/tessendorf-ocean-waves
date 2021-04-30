#ifndef OCEAN_WATER_SIMULATION_RENDERERINGSHADER_H
#define OCEAN_WATER_SIMULATION_RENDERERINGSHADER_H

#include <string>
#include "Shader.h"

namespace abstractions {

    class RenderingShader : public Shader {
    public:
        RenderingShader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
    private:
        unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
    };
}

#endif //OCEAN_WATER_SIMULATION_RENDERERINGSHADER_H
