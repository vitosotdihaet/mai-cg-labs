#version 330 core

uniform vec3 camera_position;
// uniform float time;

in vec3 world_position;
in vec2 screen_position;

out vec4 c;

void main() {
    c = vec4(abs(sin(world_position.xyz)), 1);
    float d = (clamp(distance(world_position, camera_position), 0, 1) - 1) / 2;
    c += vec4(d, d, d, 0);
}
