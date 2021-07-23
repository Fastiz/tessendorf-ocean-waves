#ifndef OCEAN_WATER_SIMULATION_TEST_UTILS_H
#define OCEAN_WATER_SIMULATION_TEST_UTILS_H

#include <iostream>
#include <stb_image_write.h>
#include <scene/fft/textures.h>

#define IS_TRUE(x) { if (!(x)) std::cout << __FUNCTION__ << " failed on line " << __LINE__ << std::endl; }

namespace test_utils {
    enum image_type {
        BNW, COMPLEX
    };

    static std::vector<float> normalize(std::vector<float> values){
        float max = std::numeric_limits<float>::lowest(), min = std::numeric_limits<float>::max();

        for(float value : values){
            if(value > max) max = value;
            if(value < min) min = value;
        }

        std::vector<float> result(values.size());
        for(int i=0; i<values.size(); i++){
            result[i] = (values[i] - min) / (max - min);
        }

        return result;
    }

    static void save_char_data(std::vector<unsigned char> char_data, std::basic_string<char> path, int N){
        stbi_write_png(path.c_str(), N, N, 4, &char_data[0], N * 4);
    }

    static void print_bnw(std::vector<float> values, int N, std::basic_string<char> path){
        std::vector<unsigned char> char_data(N*N*4);

        for(int i=0; i<N*N; i++) {
            float value = values[i] * 255;

            char_data[i * 4] = (unsigned char) (value);
            char_data[i * 4 + 1] = (unsigned char) (value);
            char_data[i * 4 + 2] = (unsigned char) (value);
            char_data[i * 4 + 3] = 255;
        }

        save_char_data(char_data, path, N);
    }

    static void print_complex(std::vector<float> values, int N, std::basic_string<char> path){
        std::vector<unsigned char> char_data(N*N*4);

        for(int i=0; i<N*N; i++) {
            char_data[i * 4] = (unsigned char) (values[i * 2] * 255);
            char_data[i * 4 + 1] = (unsigned char) (values[i * 2 + 1] * 255);
            char_data[i * 4 + 2] = 0;
            char_data[i * 4 + 3] = 255;
        }

        save_char_data(char_data, path, N);
    }

    static void print_image(textures::ssbo_pointer ssbo, int N, std::basic_string<char> path, bool normalize, image_type type){
        std::vector<float> values(ssbo->GetSize() / 4);

        ssbo->GetBufferData(&values[0]);

        if(normalize) values = test_utils::normalize(values);

        if(type == image_type::BNW) print_bnw(values, N, path);
        else print_complex(values, N, path);
    }

}

#endif //OCEAN_WATER_SIMULATION_TEST_UTILS_H
