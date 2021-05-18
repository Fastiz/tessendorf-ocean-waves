#version 450 core

#define PI 3.14159265358979323846
#define GRAVITY_CONSTANT 9.8f

layout(local_size_x = 1, local_size_y = 1) in;

// Real part on red channel and imaginary on green
layout(rgba32f, binding = 0) uniform readonly image2D h0_txt;

layout(rgba32f, binding = 1) uniform readonly image2D h0conj_txt;

layout(rgba32f, binding = 2) uniform writeonly image2D h_k_t_txt;

uniform float time;
uniform int N;
uniform float L;

vec2 complex_product(vec2 a, vec2 b){
    return vec2(a.x*b.x - a.y*b.y, a.x*b.y + a.y*b.x);
}

vec2 complex_exp(float coef){
    return vec2(cos(coef), sin(coef));
}

float dispersion(float k){
    return sqrt(GRAVITY_CONSTANT * k);
}

void main(){
    ivec2 coords = ivec2(gl_GlobalInvocationID.xy);

    vec2 h0 = imageLoad(h0_txt, coords).rg;
    vec2 h0conj = imageLoad(h0conj_txt, coords).rg;

    ivec2 nm = ivec2(coords.x - float(N) / 2.0f, coords.y - float(N) / 2.0f);
    vec2 K = vec2(2.0 * PI * nm.x / L, 2.0 * PI * nm.y / L);

    float coef = time * dispersion(length(K));

    vec2 h_k_t = complex_product(h0, complex_exp(coef)) + complex_product(h0conj, complex_exp(-coef));

    imageStore(h_k_t_txt, coords, vec4(h_k_t, 0.0f, 1.0f));
}