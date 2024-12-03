#version 330 core

uniform vec3 camera_position;
// uniform float time;

in vec3 world_position;
in vec3 screen_position;

out vec4 c;

void main() {
    c = vec4(0.5, 0.5, 0.5, 1);
}
