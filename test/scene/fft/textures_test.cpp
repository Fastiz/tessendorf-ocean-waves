#include <scene/fft/textures.h>
#include "textures_test.h"

#include <vendor/std_image/stb_image_write.h>

#define H0_TEXTURE_PATH "../../res/textures/test/fft/h0.png"
#define H0CONJ_TEXTURE_PATH "../../res/textures/test/fft/h0conj.png"

namespace textures_test {
    void run(){
        h0_texture();
    }

    // TODO: automatize testing and stop saving textures into files
    void h0_texture(){
        auto textures = textures::generate_spectrum_textures(256, 4, 1.0f, 1.0f, 256);

        std::vector<float> h0_values = textures.h0.GetTextureImage();
        std::vector<float> h0conj_values = textures.h0conj.GetTextureImage();

        std::vector<unsigned char> h0_char_data(256 * 256 * 4);
        std::vector<unsigned char> h0conj_char_data(256 * 256 * 4);

        for(int i=0; i<h0_values.size(); i++){
            h0_char_data[i] = (unsigned char)(h0_values[i] * 255);
            h0conj_char_data[i] = (unsigned char)(h0conj_values[i] * 255);
        }

        stbi_write_png(H0_TEXTURE_PATH, 256, 256, 4, &h0_char_data[0], 256 * 4);
        stbi_write_png(H0CONJ_TEXTURE_PATH, 256, 256, 4, &h0conj_char_data[0], 256 * 4);

        textures.h0.DeleteTexture();
        textures.h0conj.DeleteTexture();
    }
}