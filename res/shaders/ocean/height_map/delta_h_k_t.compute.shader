#version 450 core

#define PI 3.14159265358979323846
#define GRAVITY_CONSTANT 9.8f

layout(local_size_x = 1, local_size_y = 1) in;

layout(std430, binding = 0) readonly buffer Block3 {
    vec2 values[];
} h_k_t_txt;

layout(std430, binding = 1) writeonly buffer Block1 {
    vec2 values[];
} delta_h_k_t_x_txt;

layout(std430, binding = 2) writeonly buffer Block2 {
    vec2 values[];
} delta_h_k_t_y_txt;

uniform float time;
uniform int N;
uniform float L;

int alias(int a){
    if(a > N/2) a -= N;
    return a;
}

int ialias(int b){
    if(b < 0) b += N;
    return b;
}

ivec2 nm_to_xy(ivec2 nm){
    return ivec2(ialias(nm.x), ialias(nm.y));
}

ivec2 xy_to_nm(ivec2 xy){
    return ivec2(alias(xy.x), alias(xy.y));
}

vec2 nm_to_k(ivec2 nm){
    return vec2(2.0 * PI * nm.x / L, 2.0 * PI * nm.y / L);
}

vec2 complex_product(vec2 a, vec2 b){
    return vec2(a.x*b.x - a.y*b.y, a.x*b.y + a.y*b.x);
}

vec2 complex_exp(float coef){
    return vec2(cos(coef), sin(coef));
}

void main(){
    ivec2 coords = ivec2(gl_GlobalInvocationID.xy);

    uint index = coords.x + coords.y*N;

    ivec2 nm = xy_to_nm(coords);

    vec2 K = nm_to_k(nm);
    float k = length(K);

    vec2 h_k_t = h_k_t_txt.values[index];

    vec2 iKx = vec2(0.0f, -K.x);
    delta_h_k_t_x_txt.values[index] = k == 0 ? vec2(0.0f) : complex_product(iKx, h_k_t) / k;

    vec2 iKy = vec2(0.0f, -K.y);
    delta_h_k_t_y_txt.values[index] = k == 0 ? vec2(0.0f) : complex_product(iKy, h_k_t) / k;
}