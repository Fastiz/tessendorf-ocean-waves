#version 450 core

#define PI 3.14159265358979323846
#define GRAVITY_CONSTANT 9.8f

layout(local_size_x = 1, local_size_y = 1) in;

layout(std430, binding = 0) readonly buffer Block {
    vec2 values[];
} h0_txt;

layout(std430, binding = 1) readonly buffer Block2 {
    vec2 values[];
} h0conj_txt;

layout(std430, binding = 2) writeonly buffer Block3 {
    vec2 values[];
} h_k_t_txt;

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
    float clamped_k = max(k, 0.00001);

    return sqrt(GRAVITY_CONSTANT * clamped_k);
}

void main(){
    ivec2 coords = ivec2(gl_GlobalInvocationID.xy);

    vec2 h0 = h0_txt.values[coords.x + coords.y*N];

    ivec2 nm = ivec2(coords.x - N / 2, coords.y - N / 2);

    vec2 h0conj = h0conj_txt.values[-nm.x + N / 2 + (-nm.y + N / 2)*N];

    vec2 K = vec2(2.0 * PI * nm.x / L, 2.0 * PI * nm.y / L);

    float coef = time * dispersion(length(K));

    vec2 h_k_t = complex_product(h0, complex_exp(coef)) + complex_product(h0conj, complex_exp(-coef));

    h_k_t_txt.values[coords.x + coords.y*N] = h_k_t;
}