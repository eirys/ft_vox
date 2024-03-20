#version 450

layout(location = 0) out vec3 outForward;

layout(set = 0, binding = 1) uniform GameData {
    float time;
    vec3  forward;
    vec3  right;
    vec3  up;
} gameData;

const vec2 quadCorner[4] = {
    vec2(-1.0, -1.0),
    vec2(1.0, -1.0),
    vec2(-1.0, 1.0),
    vec2(1.0, 1.0)
};

void main() {
    vec2 pos = quadCorner[gl_VertexIndex];
    gl_Position = vec4(pos, 0.0, 1.0);
    // forward = normalize(
    //     vec4(gameData.forward, 1.0) + pos.x *
    //     vec4(gameData.right, 1.0) + pos.y *
    //     vec4(gameData.up, 1.0)).xyz;
    outForward = normalize(
        gameData.forward + pos.x *
        gameData.right + pos.y *
        gameData.up);
}