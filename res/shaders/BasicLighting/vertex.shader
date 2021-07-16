#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

layout(std430, binding = 0) readonly buffer Block {
    float values[];
} height_map;

uniform int N;
uniform float L;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec3 Normal;
out vec3 FragPos;
out float Valley;

int ssbo_index(ivec2 coords){
    return coords.x % N + (coords.y % N)*N;
}

float height(ivec2 coords){
    return height_map.values[ssbo_index(coords.xy)];
}

float dy(ivec2 coords, int D){
    float h = height(coords);

    float aux = (h - height(ivec2(coords.x, coords.y + D))) +
    (h - height(ivec2(coords.x, coords.y - D))) +
    (h - height(ivec2(coords.x + D, coords.y))) +
    (h - height(ivec2(coords.x - D, coords.y)));

    return aux / 4.0f;
}

void main()
{
    vec3 pos = position;
    ivec2 coords = ivec2(pos.xy);
    pos.y = height(coords);

    gl_Position = proj * view * model * vec4(pos, 1.0);
    Normal = normal;
    FragPos = vec3(model * vec4(pos, 1.0));

    float shadow = 1;
    for(int i=0; i<10; i++){
        float val = dy(coords, i);
        shadow += val * val * val;
    }

    Valley = 0.5f + shadow * 0.002f;
}