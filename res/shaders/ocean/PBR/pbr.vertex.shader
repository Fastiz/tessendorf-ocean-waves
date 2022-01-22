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

uniform float showBorder;

uniform float lambda;

out vec3 Normal;
out vec3 WorldPos;
out float IsBorder;

out vec3 Debug;

int ssbo_index(ivec2 coords){
    ivec2 normCoords = coords % N;
    return normCoords.x + normCoords.y * N;
}

vec2 get_displacement(ivec2 coords){
    int index = ssbo_index(coords.xy);

    float x = height_map_x_slope.values[index];
    float y = height_map_y_slope.values[index];

    return vec2(x, y);
}

float get_height(ivec2 coords){
    return height_map.values[ssbo_index(coords.xy)];
}

vec3 get_normal(ivec2 coords){
    vec3 N;

    // read neightbor heights using an arbitrary small offset
    ivec3 off = ivec3(1.0, 1.0, 0.0);
    float hL = get_height(coords - off.xz);
    float hR = get_height(coords + off.xz);
    float hD = get_height(coords - off.zy);
    float hU = get_height(coords + off.zy);

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

    pos.y = get_height(coords);
    Normal = get_normal(coords);

    pos.xz *= L / N;
    pos.xz += lambda * get_displacement(coords);

    vec4 modelCoords = model * vec4(pos, 1.0);

    gl_Position = proj * view * modelCoords;

    WorldPos = vec3(modelCoords);

    IsBorder = (showBorder > 0.5) && (coords.x % N == 0 || coords.y % N == 0) ? 1.0 : 0.0;
}