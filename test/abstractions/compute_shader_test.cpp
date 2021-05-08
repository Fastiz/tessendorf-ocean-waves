#include "compute_shader_test.h"
#include <abstractions/Texture.h>
#include <abstractions/ComputeShader.h>
#include <abstractions/Computer.h>
#include <abstractions/Renderer.h>
#include "../test_utils.h"

#define EPSILON .00001

using namespace abstractions;

namespace compute_shader_test {
    struct RGB32F {
        float R, G, B, A;
    };

    std::vector<RGB32F> parse(std::vector<float> values){
        int N = values.size() / 4;

        std::vector<RGB32F> result(N);
        for(int i=0; i<N; i++){
            int off = i*4;
            result[i] = {values[off], values[off+1], values[off+2], values[off+3]};
        }

        return result;
    }

    bool compare_colors(RGB32F a, RGB32F b){
        return  std::abs(a.R - b.R) < EPSILON &&
                std::abs(a.G - b.G) < EPSILON &&
                std::abs(a.B - b.B) < EPSILON &&
                std::abs(a.A - b.A) < EPSILON;
    }

    void run(){
        compute_triangle_regions();
    }

    void compute_triangle_regions() {
        int width = 10,
            height = 10;

        Texture output_texture(width, height);

        ComputeShader shader(
                "../../res/shaders/test/abstractions/compute_shader_test/compute_triangle_regions.shader"
                );

        output_texture.BindImageTexture(0);

        Computer computer;

        computer.DispatchCompute(shader, width, height, 1);

        GLCall(glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT));

        std::vector<float> values = output_texture.GetTextureImage();
        std::vector<RGB32F> pixels = parse(values);

        RGB32F  red = {1.0f, 0.0f, 0.0f, 1.0f},
                blue = {0.0f, 0.0f, 1.0f, 1.0f};

        int errors = 0;
        for(int i=0; i<pixels.size(); i++){
            int row = i / width;
            int col = i % width;

            bool flag;
            if(row > col) {
                flag = compare_colors(pixels[i], red);
            }else {
                flag = compare_colors(pixels[i], blue);
            }

            if(!flag) errors++;
        }

        IS_TRUE(errors == 0)

        output_texture.DeleteTexture();
    }
}