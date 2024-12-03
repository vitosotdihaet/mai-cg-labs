#version 330 core

layout(location = 0) in vec3 position;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;


out vec3 world_position;
out vec3 screen_position;

void main() {
    world_position = (model * vec4(position, 1.0)).xyz;
    gl_Position = projection * view * vec4(world_position, 1.0);
    screen_position = gl_Position.xyz;
}
