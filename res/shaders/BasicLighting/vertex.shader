#version 450 core

layout(location = 0) in vec3 position;

layout(std430, binding = 0) readonly buffer Block {
    float values[];
} height_map;

layout(std430, binding = 1) readonly buffer Block1 {
    float values[];
} height_map_x_slope;

layout(std430, binding = 2) readonly buffer Block2 {
    float values[];
} height_map_y_slope;

uniform int N;
uniform float L;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec3 Normal;
out vec3 FragPos;

int ssbo_index(ivec2 coords){
    return coords.x % N + (coords.y % N)*N;
}

vec3 get_normal(ivec2 coords){
    return vec3(
        -height_map_x_slope.values[ssbo_index(coords)],
        1.0,
        -height_map_y_slope.values[ssbo_index(coords)]
    );
}

float get_height(ivec2 coords){
    return height_map.values[ssbo_index(coords.xy)];
}

void main()
{
    vec3 pos = position;
    ivec2 coords = ivec2(pos.xz);

    pos.y = get_height(coords);
    Normal = get_normal(coords);

    gl_Position = proj * view * model * vec4(pos, 1.0);

    FragPos = vec3(model * vec4(pos, 1.0));
}