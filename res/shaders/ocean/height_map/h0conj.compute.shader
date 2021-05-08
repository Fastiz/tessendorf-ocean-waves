#version 450 core

#define CONJUGATE(a) vec2(a.x, -a.y)

layout(local_size_x = 1, local_size_y = 1) in;

// Real part on red channel and imaginary on green
layout(rgba32f, binding = 0) uniform readonly image2D h0;

layout(rgba32f, binding = 1) uniform writeonly image2D h0conj;

uniform int N;

void main(){
    ivec2 coords = ivec2(gl_GlobalInvocationID.xy);

    vec2 h0_val = imageLoad(h0, ivec2(N) - coords).rg;

    vec2 h0conj_val = vec2(h0_val.r, -h0_val.g);

    imageStore(h0conj, coords, vec4(h0conj_val, 0.0f, 1.0f));
}