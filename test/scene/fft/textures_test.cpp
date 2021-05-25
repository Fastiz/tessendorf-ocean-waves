#include <scene/fft/textures.h>
#include "textures_test.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#define H0_TEXTURE_PATH "../../res/textures/test/fft/h0.png"
#define H0CONJ_TEXTURE_PATH "../../res/textures/test/fft/h0conj.png"
#define H_K_T_TEXTURE_PATH "../../res/textures/test/fft/h_k_t.png"

namespace textures_test {
    void run(){
        h0_texture();
    }

    // TODO: automatize testing and stop saving textures into files
#define N 256
    void h0_texture(){
        auto textures = textures::generate_spectrum_textures(N, 4, 1.0f, 1.0f, 256);
        abstractions::Texture h_k_t = textures::generate_transform_texture(textures.h0, textures.h0conj, N, 256, 1.0);

        std::vector<float> h0_values = textures.h0.GetTextureImage();
        std::vector<float> h0conj_values = textures.h0conj.GetTextureImage();
        std::vector<float> h_k_t_values = h_k_t.GetTextureImage();

        std::vector<unsigned char> h0_char_data(N * N * 4);
        std::vector<unsigned char> h0conj_char_data(N * N * 4);
        std::vector<unsigned char> h_k_t_char_data(N * N * 4);

        for(int i=0; i<h0_values.size(); i++){
            h0_char_data[i] = (unsigned char)(h0_values[i] * 255);
            h0conj_char_data[i] = (unsigned char)(h0conj_values[i] * 255);
            h_k_t_char_data[i] = (unsigned char)(h_k_t_values[i] * 255);
        }


        stbi_write_png(H0_TEXTURE_PATH, N, N, 4, &h0_char_data[0], N * 4);
        stbi_write_png(H0CONJ_TEXTURE_PATH, N, N, 4, &h0conj_char_data[0], N * 4);
        stbi_write_png(H_K_T_TEXTURE_PATH, N, N, 4, &h_k_t_char_data[0], N * 4);

        textures.h0.DeleteTexture();
        textures.h0conj.DeleteTexture();
        h_k_t.DeleteTexture();
    }
}