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

int ssbo_index(vec2 coords){
    return int(coords.x) + int(coords.y)*N;
}

void main()
{
    vec3 pos = position;
    pos.y = height_map.values[ssbo_index(pos.xy)];

    gl_Position = proj * view * model * vec4(pos, 1.0);
    Normal = normal;
    FragPos = vec3(model * vec4(pos, 1.0));

    Valley = 0.5f + pos.y*0.00001f;
}