#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

uniform float elapsedTime;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec3 Normal;
out vec3 FragPos;
out float Valley;

float sinusoid(float x, float z, float a, float b, float delta){
    return sin(a*x + b*z + delta);
}

void main()
{
    vec3 pos = position;
    pos.y = sinusoid(position.x, position.z, 5.0f, 5.0f, 10 * elapsedTime) / 20.0f;

    gl_Position = proj * view * model * vec4(pos, 1.0);
    Normal = normal;
    FragPos = vec3(model * vec4(pos, 1.0));

    Valley = 0.5f + pos.y * 0.5f;
}