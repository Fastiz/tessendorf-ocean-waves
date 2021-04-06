#shader vertex
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

#shader fragment
#version 330 core

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

in vec3 Normal;
in vec3 FragPos;
in float Valley;

void main()
{
    float specularStrength = 0.5;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 result = (specular + ambient + diffuse) * objectColor;
    FragColor = vec4(result * Valley, 1.0);
}