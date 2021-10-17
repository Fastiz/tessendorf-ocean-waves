#ifndef OCEAN_WATER_SIMULATION_NOISE_TEXTURE_H
#define OCEAN_WATER_SIMULATION_NOISE_TEXTURE_H

#include "../abstractions/Texture.h"

namespace utils {
    std::unique_ptr<abstractions::Texture> generate_noise_texture(int width, int height);
}

#endif //OCEAN_WATER_SIMULATION_NOISE_TEXTURE_H
