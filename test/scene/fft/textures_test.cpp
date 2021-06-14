#include <scene/fft/textures.h>
#include "textures_test.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#define H0_TEXTURE_PATH "../../res/textures/test/fft/h0.png"
#define H0CONJ_TEXTURE_PATH "../../res/textures/test/fft/h0conj.png"
#define H_K_T_TEXTURE_PATH "../../res/textures/test/fft/h_k_t.png"
#define HEIGHT_MAP_TEXTURE_PATH "../../res/textures/test/fft/height_map.png"

namespace textures_test {
    void run(){
        h0_texture();
    }

    // TODO: automatize testing and stop saving textures into files
#define N 256
    void h0_texture(){
        auto textures = textures::generate_spectrum_textures(N, 4, 1.0f, 1.0f, 256);
        textures::ssbo_pointer h_k_t = textures::generate_transform_texture(std::get<0>(textures), std::get<1>(textures), N, 256, 1.0);
//        std::shared_ptr<abstractions::Texture> height_map = textures::update_fft_texture(h_k_t, N);

        std::vector<float> h0_values(N*N*2);
        get<0>(textures)->GetBufferData(&h0_values[0]);

        std::vector<float> h0conj_values(N*N*2);
        get<1>(textures)->GetBufferData(&h0conj_values[0]);

        std::vector<float> h_k_t_values(N*N*2);
        h_k_t->GetBufferData(&h_k_t_values[0]);

//        std::vector<float> height_map_values = height_map->GetTextureImage(0);

        std::vector<unsigned char> h0_char_data(N * N * 4);
        std::vector<unsigned char> h0conj_char_data(N * N * 4);
        std::vector<unsigned char> h_k_t_char_data(N * N * 4);
//        std::vector<unsigned char> height_map_char_data(N * N * 4);

        for(int i=0; i<N*N; i++){
            h0_char_data[i*4] = (unsigned char)(h0_values[i*2] * 255);
            h0_char_data[i*4+1] = (unsigned char)(h0_values[i*2+1] * 255);
            h0_char_data[i*4+2] = 0;
            h0_char_data[i*4+3] = 255;

            h0conj_char_data[i*4] = (unsigned char)(h0conj_values[i*2] * 255);
            h0conj_char_data[i*4+1] = (unsigned char)(h0conj_values[i*2+1] * 255);
            h0conj_char_data[i*4+2] = 0;
            h0conj_char_data[i*4+3] = 255;

            h_k_t_char_data[i*4] = (unsigned char)(h_k_t_values[i*2] * 255);
            h_k_t_char_data[i*4+1] = (unsigned char)(h_k_t_values[i*2+1] * 255);
            h_k_t_char_data[i*4+2] = 0;
            h_k_t_char_data[i*4+3] = 255;

//            height_map_char_data[i*4] = (unsigned char)(height_map_values[i] * 255);
//            height_map_char_data[i*4+1] = (unsigned char)(height_map_values[i] * 255);
//            height_map_char_data[i*4+2] = (unsigned char)(height_map_values[i] * 255);
//            height_map_char_data[i*4+3] = 255;
        }

        stbi_write_png(H0_TEXTURE_PATH, N, N, 4, &h0_char_data[0], N * 4);
        stbi_write_png(H0CONJ_TEXTURE_PATH, N, N, 4, &h0conj_char_data[0], N * 4);
        stbi_write_png(H_K_T_TEXTURE_PATH, N, N, 4, &h_k_t_char_data[0], N * 4);
//        stbi_write_png(HEIGHT_MAP_TEXTURE_PATH, N, N, 4, &height_map_char_data[0], N * 4);
    }
}