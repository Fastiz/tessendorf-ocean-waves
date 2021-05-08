#ifndef OCEAN_WATER_SIMULATION_TEXTURES_H
#define OCEAN_WATER_SIMULATION_TEXTURES_H

#include "../../abstractions/Texture.h"

namespace textures {
    struct SpectrumTextures {
        abstractions::Texture h0, h0conj;
    };

    SpectrumTextures generate_spectrum_textures(int N, float A, float wind_x, float wind_y, float L);
}


#endif //OCEAN_WATER_SIMULATION_TEXTURES_H
