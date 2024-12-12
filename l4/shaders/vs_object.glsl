#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 uv;
layout(location = 2) in vec3 normal;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;


out vec3 world_position;
out vec3 screen_position;
out vec3 f_normal;

void main() {
    world_position = (model * vec4(position, 1.0)).xyz;

    f_normal = (model * vec4(normal, 1.0f)).xyz;

    gl_Position = projection * view * vec4(world_position, 1.0);
    screen_position = gl_Position.xyz;
}
