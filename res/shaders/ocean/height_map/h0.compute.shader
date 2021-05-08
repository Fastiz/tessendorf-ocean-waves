#version 450 core

#define PI 3.14159265358979323846
#define GRAVITY_CONSTANT 9.8f

layout(local_size_x = 1, local_size_y = 1) in;

// Real part on red channel and imaginary on green
layout(rgba32f, binding = 0) uniform writeonly image2D h0;

// Independent uniform distributions on [0-1] on channels R and G
layout(rgba32f, binding = 1) uniform readonly image2D noise;

uniform int N;
uniform float L;
uniform float A;
uniform vec2 wind;

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
    float K_norm = length(K);

    float exponential = exp(-1.0f / pow(K_norm * L_, 2));
    float dotProduct = pow(
            abs(
                dot(normalize(K), normalize(W))
            ),
            2
        );

    float aux = -pow(K_norm, 2) * pow(L / 2000.0f, 2);
    float multFactor = exp(aux);

    return clamp(A * exponential * dotProduct * multFactor / pow(K_norm, 4), -4000, 4000);
}

vec2 calculate_h0(vec2 K){
    vec2 e = gauss_random();
    float L_ = pow(length(wind), 2) / GRAVITY_CONSTANT;

    float philips = philips_spectrum(A, L_, K, wind);

    return sqrt(philips) * e / sqrt(2);
}

void main(){
    ivec2 coords = ivec2(gl_GlobalInvocationID.xy);

    ivec2 nm = ivec2(coords.x - float(N) / 2.0f, coords.y - float(N) / 2.0f);
    vec2 K = vec2(2.0 * PI * nm.x / L, 2.0 * PI * nm.y / L);

    vec2 h0_val = calculate_h0(K);

    imageStore(h0, coords, vec4(h0_val, 0.0f, 1.0f));
}