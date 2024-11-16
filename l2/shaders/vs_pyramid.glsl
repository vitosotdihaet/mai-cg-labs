#version 330 core

layout(location = 0) in vec3 position;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;


out vec3 f_position;

void main() {
    f_position = (model * vec4(position, 1.0)).xyz;
    gl_Position = projection * view * vec4(f_position, 1.0);
}
