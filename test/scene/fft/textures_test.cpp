#include <scene/fft/textures.h>
#include "textures_test.h"
#include "../../test_utils.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#include <utils/noise_texture.h>

#define NOISE_TEXTURE_PATH "../../res/textures/test/fft/noise.png"
#define H0_TEXTURE_PATH "../../res/textures/test/fft/h0.png"
#define H0CONJ_TEXTURE_PATH "../../res/textures/test/fft/h0conj.png"
#define H_K_T_TEXTURE_PATH "../../res/textures/test/fft/h_k_t.png"
#define SLOPE_SPECTRUM_X_TEXTURE_PATH "../../res/textures/test/fft/delta_h_k_t_x.png"
#define SLOPE_SPECTRUM_Y_TEXTURE_PATH "../../res/textures/test/fft/delta_h_k_t_y.png"
#define HEIGHT_MAP_TEXTURE_PATH "../../res/textures/test/fft/height_map.png"
#define SLOPE_X_TEXTURE_PATH "../../res/textures/test/fft/slope_x.png"
#define SLOPE_Y_TEXTURE_PATH "../../res/textures/test/fft/slope_y.png"

namespace textures_test {
    void run(){
        h0_texture();
    }

#define N 512
#define L 1000
    void h0_texture(){
        auto noise_texture = utils::generate_noise_texture(N, N);
        auto textures = textures::generate_spectrum_textures(N, 1, 1.0f, 0.0f, 30.0f, L);
        textures::ssbo_pointer h_k_t = textures::generate_transform_texture(std::get<0>(textures), std::get<1>(textures), N, L, 1.0);
        auto slope_spectrum = textures::update_slope_texture(h_k_t, N, L);

        textures::ssbo_pointer height_map = textures::update_fft_texture(h_k_t, N);
        textures::ssbo_pointer slope_x = textures::update_fft_texture(get<0>(slope_spectrum), N);
        textures::ssbo_pointer slope_y = textures::update_fft_texture(get<1>(slope_spectrum), N);

        test_utils::print_image(std::get<0>(textures), N, H0_TEXTURE_PATH, false, test_utils::image_type::COMPLEX);
        test_utils::print_image(std::get<1>(textures), N, H0CONJ_TEXTURE_PATH, false, test_utils::image_type::COMPLEX);
        test_utils::print_image(h_k_t, N, H_K_T_TEXTURE_PATH, false, test_utils::image_type::COMPLEX);
        test_utils::print_image(get<0>(slope_spectrum), N, SLOPE_SPECTRUM_X_TEXTURE_PATH, false, test_utils::image_type::COMPLEX);
        test_utils::print_image(get<1>(slope_spectrum), N, SLOPE_SPECTRUM_Y_TEXTURE_PATH, false, test_utils::image_type::COMPLEX);

        test_utils::print_image(height_map, N, HEIGHT_MAP_TEXTURE_PATH, true, test_utils::image_type::BNW);
        test_utils::print_image(slope_x, N, SLOPE_X_TEXTURE_PATH, false, test_utils::image_type::BNW);
        test_utils::print_image(slope_y, N, SLOPE_Y_TEXTURE_PATH, false, test_utils::image_type::BNW);

        std::vector<float> noise = noise_texture->GetTextureImage(0);
        std::vector<unsigned char> noise_char_data(N * N * 4);

        for(int i=0; i<N*N; i++){
            noise_char_data[i*4] = (unsigned char)(noise[i*4] * 255);
            noise_char_data[i*4+1] = (unsigned char)(noise[i*4+1] * 255);
            noise_char_data[i*4+2] = 0;
            noise_char_data[i*4+3] = 255;
        }

        stbi_write_png(NOISE_TEXTURE_PATH, N, N, 4, &noise_char_data[0], N * 4);
    }
}