#version 450 core

#define PI 3.14159265358979323846
#define GRAVITY_CONSTANT 9.8f

layout(local_size_x = 1, local_size_y = 1) in;
// Real part on red channel and imaginary on green

layout(std430, binding = 0) writeonly buffer Block {
    vec2 values[];
} h0;

layout(std430, binding = 1) writeonly buffer Block2 {
    vec2 values[];
} h0conj;

// Independent uniform distributions on [0-1] on channels R and G
layout(rgba32f, binding = 2) uniform readonly image2D noise;

uniform int N;
uniform float L;
uniform float A;
uniform vec2 wind_dir;
uniform float wind_speed;

ivec2 nm_to_xy(ivec2 nm){
    return ivec2(nm.x+N/2, nm.y+N/2);
}

ivec2 xy_to_nm(ivec2 xy){
    return ivec2(xy.x-N/2, xy.y-N/2);
}

vec2 nm_to_k(ivec2 nm){
    return vec2(2.0 * PI * nm.x / L, 2.0 * PI * nm.y / L);
}

//Box muller random
vec2 gauss_random(){
    vec2 rands = imageLoad(noise, ivec2(gl_GlobalInvocationID.xy)).rg;
    float   U1 = rands.r,
            U2 = rands.g;

    float root = sqrt(-2 * log(U1));
    float Z0 = root * cos(2 * PI * U2);
    float Z1 = root * sin(2 * PI * U2);

    return vec2(Z0, Z1);
}

float philips_spectrum(float A, float L_, vec2 K, vec2 W){
    float k_lenght = length(K);
//    if(k_lenght < length(nm_to_k(ivec2(70, 0)))) return 0;
    if(k_lenght == 0) return 0;

    float kL_ = k_lenght * L_;

    float dot = dot(normalize(K), normalize(W));

    float l_min = L/1000.0f;

    return A *
        exp(-1.0f / (kL_ * kL_) + -1.0f * k_lenght * k_lenght * l_min * l_min) *
        dot * dot /
        (k_lenght * k_lenght * k_lenght * k_lenght);
}

vec2 calculate_h0(vec2 K){
    vec2 e = gauss_random();
    float L_ = wind_speed * wind_speed / GRAVITY_CONSTANT;

    float philips = philips_spectrum(A, L_, K, wind_dir);

    return sqrt(philips * 0.5f) * e;
}

void main(){
    ivec2 coords = ivec2(gl_GlobalInvocationID.xy);

    ivec2 nm = xy_to_nm(coords);

    vec2 K = nm_to_k(nm);

    vec2 h0_val = calculate_h0(K);

    h0.values[coords.x + coords.y * N] = h0_val;
    h0conj.values[coords.x + coords.y * N] = vec2(h0_val.x, -h0_val.y);
}