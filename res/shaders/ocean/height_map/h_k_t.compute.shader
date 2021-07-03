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
    return sqrt(GRAVITY_CONSTANT * k);
}

ivec2 nm_to_xy(ivec2 nm){
    return ivec2(nm.x+N/2, nm.y+N/2);
}

ivec2 xy_to_nm(ivec2 xy){
    return ivec2(xy.x-N/2, xy.y-N/2);
}

vec2 nm_to_k(ivec2 nm){
    return vec2(2.0 * PI * nm.x / L, 2.0 * PI * nm.y / L);
}

void main(){
    ivec2 coords = ivec2(gl_GlobalInvocationID.xy);

    ivec2 nm = xy_to_nm(coords);

    vec2 h0 = h0_txt.values[coords.x + coords.y*N];

    ivec2 negative_coords = nm_to_xy(-nm);
    vec2 h0conj = h0conj_txt.values[negative_coords.x + negative_coords.y*N];

    vec2 K = nm_to_k(nm);

    float coef = time * dispersion(length(K));

    vec2 h_k_t = complex_product(h0, complex_exp(coef)) + complex_product(h0conj, complex_exp(-coef));

    h_k_t_txt.values[coords.x + coords.y*N] = h_k_t;
}