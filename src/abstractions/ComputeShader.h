#ifndef OCEAN_WATER_SIMULATION_COMPUTESHADER_H
#define OCEAN_WATER_SIMULATION_COMPUTESHADER_H

#include "Shader.h"

namespace abstractions {

    class ComputeShader : public Shader {
    public:
        ComputeShader(const std::string& computeShaderPath);
    private:
        unsigned int CreateShader(const std::string& computeShader);
    };
}

#endif //OCEAN_WATER_SIMULATION_COMPUTESHADER_H
