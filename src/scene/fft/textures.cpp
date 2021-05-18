#include "textures.h"
#include "../../abstractions/ComputeShader.h"
#include "../../utils/noise_texture.h"
#include "../../abstractions/Computer.h"

#define H0_COMPUTE_SHADER "../../res/shaders/ocean/height_map/h0.compute.shader"
#define H0CONJ_COMPUTE_SHADER "../../res/shaders/ocean/height_map/h0conj.compute.shader"
#define TRANSFORM_COMPUTE_SHADER "../../res/shaders/ocean/height_map/h_k_t.compute.shader"

namespace textures {
    SpectrumTextures generate_spectrum_textures(int N, float A, float wind_x, float wind_y, float L){
        abstractions::ComputeShader h0_shader(H0_COMPUTE_SHADER);
        abstractions::ComputeShader h0conj_shader(H0CONJ_COMPUTE_SHADER);

        abstractions::Texture h0(N, N);
        abstractions::Texture h0conj(N, N);

        abstractions::Texture noise_texture = utils::generate_noise_texture(N, N);

        h0.BindImageTexture(0);
        noise_texture.BindImageTexture(1);

        h0_shader.Bind();
        h0_shader.SetUniform1f("A", A);
        h0_shader.SetUniform2f("wind", wind_x, wind_y);
        h0_shader.SetUniform1f("L", L);
        h0_shader.SetUniform1i("N", N);

        abstractions::Computer computer;

        computer.DispatchCompute(h0_shader, N, N, 1);
        abstractions::Computer::MemoryBarrier();

        h0conj_shader.Bind();
        h0conj_shader.SetUniform1i("N", N);

        h0conj.BindImageTexture(1);
        computer.DispatchCompute(h0conj_shader, N, N, 1);
        abstractions::Computer::MemoryBarrier();

        return {h0, h0conj};
    }

    abstractions::Texture generate_transform_texture(abstractions::Texture& h0, abstractions::Texture& h0conj, int N, float L, float t){
        abstractions::ComputeShader transform_shader(TRANSFORM_COMPUTE_SHADER);

        abstractions::Texture out_texture(N, N);

        h0.BindImageTexture(0);
        h0conj.BindImageTexture(1);
        out_texture.BindImageTexture(2);

        transform_shader.Bind();
        transform_shader.SetUniform1f("time", t);
        transform_shader.SetUniform1i("N", N);
        transform_shader.SetUniform1f("L", L);

        abstractions::Computer computer;
        computer.DispatchCompute(transform_shader, N, N, 1);
        abstractions::Computer::MemoryBarrier();

        return out_texture;
    }
}