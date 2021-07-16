#include "textures.h"
#include "../../abstractions/ComputeShader.h"
#include "../../utils/noise_texture.h"
#include "../../abstractions/Computer.h"
#include "../../abstractions/Renderer.h"
#include <glfft.hpp>
#include <glfft_gl_interface.hpp>
#include <GL/glew.h>
#include <iostream>

#define H0_COMPUTE_SHADER "../../res/shaders/ocean/height_map/h0.compute.shader"
#define H0CONJ_COMPUTE_SHADER "../../res/shaders/ocean/height_map/h0conj.compute.shader"
#define TRANSFORM_COMPUTE_SHADER "../../res/shaders/ocean/height_map/h_k_t.compute.shader"

namespace textures {

    std::pair<ssbo_pointer, ssbo_pointer> generate_spectrum_textures(int N, float A, float wind_x, float wind_y, float wind_speed, float L){
        abstractions::ComputeShader h0_shader(H0_COMPUTE_SHADER);

        std::unique_ptr<abstractions::SSBO> h0(new abstractions::SSBO(nullptr, 2*4*N*N, GL_DYNAMIC_COPY));
        std::unique_ptr<abstractions::SSBO> h0conj(new abstractions::SSBO(nullptr, 2*4*N*N, GL_DYNAMIC_COPY));

        abstractions::Texture noise_texture = utils::generate_noise_texture(N, N);

        (*h0).BindToSlot(0);
        (*h0conj).BindToSlot(1);

        noise_texture.BindImageTexture(2);

        h0_shader.Bind();
        h0_shader.SetUniform1f("A", A);
        h0_shader.SetUniform2f("wind_dir", wind_x, wind_y);
        h0_shader.SetUniform1f("wind_speed", wind_speed);
        h0_shader.SetUniform1f("L", L);
        h0_shader.SetUniform1i("N", N);

        abstractions::Computer computer;

        computer.DispatchCompute(h0_shader, N, N, 1);
        abstractions::Computer::MemoryBarrier();

        return std::make_pair(std::move(h0), std::move(h0conj));
    }

    ssbo_pointer generate_transform_texture(ssbo_pointer& h0, ssbo_pointer& h0conj, int N, float L, float t){
        abstractions::ComputeShader transform_shader(TRANSFORM_COMPUTE_SHADER);

        ssbo_pointer out(new abstractions::SSBO(nullptr, 2*4*N*N, GL_DYNAMIC_COPY));

        (*h0).BindToSlot(0);
        (*h0conj).BindToSlot(1);
        (*out).BindToSlot(2);

        transform_shader.Bind();
        transform_shader.SetUniform1f("time", t);
        transform_shader.SetUniform1i("N", N);
        transform_shader.SetUniform1f("L", L);

        abstractions::Computer computer;
        computer.DispatchCompute(transform_shader, N, N, 1);
        abstractions::Computer::MemoryBarrier();

        return out;
    }

    std::shared_ptr<abstractions::SSBO> update_fft_texture(ssbo_pointer& h_k_t, int N){
        GLFFT::FFTOptions options;
        options.type.fp16 = false;
        options.type.output_fp16 = false;
        options.type.input_fp16 = false;
        options.type.normalize = true;
        GLFFT::GLContext context;

        GLFFT::FFT fft(&context, N, N, GLFFT::ComplexToReal, GLFFT::Inverse, GLFFT::SSBO, GLFFT::SSBO, std::make_shared<GLFFT::ProgramCache>(), options);

        GLuint output_texture, input_texture;

        std::shared_ptr<abstractions::SSBO> out(new abstractions::SSBO(nullptr, 4*N*N, GL_DYNAMIC_COPY));

        input_texture = (*h_k_t).GetRendererId();
        output_texture = (*out).GetRendererId();

        // Adapt raw GL types to types which GLContext uses internally.
        GLFFT::GLBuffer adaptor_output(output_texture);
        GLFFT::GLBuffer adaptor_input(input_texture);

        GLCall(
                {
                    // Do the FFT
                    GLFFT::CommandBuffer *cmd = context.request_command_buffer();
                    fft.process(cmd, &adaptor_output, &adaptor_input);
                    context.submit_command_buffer(cmd);
                }
                )


        GLCall(glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT));

        return out;
    }
}