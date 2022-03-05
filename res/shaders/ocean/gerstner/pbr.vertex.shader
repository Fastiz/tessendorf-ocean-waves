#version 450 core

layout(location = 0) in vec3 position;

struct Wave {
    vec2 waveVector;
    float amplitude;
    float frequency;
    float phase;
};

layout(std430, binding = 0) readonly buffer Block {
    Wave values[];
} waves;
uniform int number_of_waves;
uniform float current_time;

uniform int N;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform float showBorder;

out vec3 Normal;
out vec3 WorldPos;
out float IsBorder;

int ssbo_index(ivec2 coords){
    ivec2 normCoords = coords % N;
    return normCoords.x + normCoords.y * N;
}

vec2 get_horizontal_displacement(vec2 x0, float time){
    vec2 result = x0;
    for(int i=0; i<number_of_waves; i++){
        Wave wave = waves.values[i];
        if(wave.waveVector.x == 0 && wave.waveVector.y == 0) continue;
        result -= normalize(wave.waveVector) * wave.amplitude *
            sin( dot( wave.waveVector, x0 ) - wave.frequency * time + wave.phase );
    }
    return result;
}

float get_vertical_displacement(vec2 x0, float time){
    float height = 0;
    for(int i=0; i<number_of_waves; i++){
        Wave wave = waves.values[i];
        height += wave.amplitude * cos( dot( wave.waveVector, x0 ) - wave.frequency * time + wave.phase );
    }
    return height;
}

vec3 get_normal(ivec2 coords, float time){
    vec3 N;

    // read neightbor heights using an arbitrary small offset
    ivec3 off = ivec3(1.0, 1.0, 0.0);
    float hL = get_vertical_displacement(coords - off.xz, time);
    float hR = get_vertical_displacement(coords + off.xz, time);
    float hD = get_vertical_displacement(coords - off.zy, time);
    float hU = get_vertical_displacement(coords + off.zy, time);

    // deduce terrain normal
    N.x = hL - hR;
    N.z = hD - hU;
    N.y = 2.0;
    N = normalize(N);

    return N;
}

void main()
{
    vec3 pos = position;
    ivec2 coords = ivec2(pos.xz);

    Normal = get_normal(coords, current_time);

    pos.xz = get_horizontal_displacement(pos.xz, current_time);
    pos.y = get_vertical_displacement(pos.xz, current_time);

    vec4 modelCoords = model * vec4(pos, 1.0);

    gl_Position = proj * view * modelCoords;

    WorldPos = vec3(modelCoords);

    IsBorder = (showBorder > 0.5) && (coords.x % N == 0 || coords.y % N == 0) ? 1.0 : 0.0;
}