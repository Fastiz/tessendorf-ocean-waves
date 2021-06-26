#include <random>
#include <chrono>
#include "./noise_texture.h"

namespace utils {
    abstractions::Texture generate_noise_texture(int width, int height){
        std::default_random_engine generator;
        generator.seed(std::chrono::system_clock::now().time_since_epoch().count());

        std::uniform_real_distribution<double> distribution(0.0,1.0);

        std::vector<float> buffer(width * height * 4);

        int N = width * height;
        for(int i=0; i<N; i++){
            int off = i*4;
            buffer[off] = (float) distribution(generator);
            buffer[off + 1] = (float) distribution(generator);
        }

        abstractions::Texture texture(&buffer[0], width, height);

        return texture;
    }
}