#version 330 core

in vec3 f_position;
out vec4 c;

void main() {
    c = vec4(abs(f_position.xy), 0.5, 1);
}
