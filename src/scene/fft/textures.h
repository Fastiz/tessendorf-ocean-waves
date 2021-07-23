#ifndef OCEAN_WATER_SIMULATION_TEXTURES_H
#define OCEAN_WATER_SIMULATION_TEXTURES_H

#include "../../abstractions/SSBO.h"
#include <memory>
#include "../../abstractions/Texture.h"

namespace textures {
    typedef std::shared_ptr<abstractions::SSBO> ssbo_pointer;

    std::pair<ssbo_pointer, ssbo_pointer> generate_spectrum_textures(int N, float A, float wind_x, float wind_y, float wind_speed, float L);

    ssbo_pointer generate_transform_texture(ssbo_pointer& h0, ssbo_pointer& h0conj, int N, float L, float t);

    std::pair<ssbo_pointer, ssbo_pointer> update_slope_texture(ssbo_pointer& h_k_t, int N, float L);

    ssbo_pointer update_fft_texture(ssbo_pointer& texture, int N);
}


#endif //OCEAN_WATER_SIMULATION_TEXTURES_H
