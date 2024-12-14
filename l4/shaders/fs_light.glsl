#version 330 core

uniform vec3 camera_position;
uniform vec3 light_position;
uniform vec3 light_color;
uniform vec3 model_color;

in vec3 world_position;
in vec3 screen_position;
in vec3 f_normal;

out vec4 c;

void main() {
    const float light_constant_term = 1.0;
    const float light_linear_term = 0.1;
    const float light_quadratic_term = 0.03;

    vec3 view_direction = normalize(camera_position - world_position);
    vec3 light_direction = normalize(light_position - world_position);
    float light_distance = length(light_position - world_position);

    // ambient
    const float ambient_strength = 0.1;
    vec3 ambient_light = light_color * ambient_strength;

    // diffuse
    vec3 normal = normalize(f_normal.xyz);
    float diffuse_strength = max(0.0, dot(light_direction, normal));
    vec3 diffuse_light = diffuse_strength * light_color;

    // specular
    vec3 reflect_direction = reflect(-light_direction, normal);
    float specular_strength = pow(max(0.0, dot(view_direction, reflect_direction)), 32.0);
    vec3 specular_light = specular_strength * light_color;


    float attenuation = 1.0f / (light_constant_term + light_linear_term * light_distance + light_quadratic_term * light_distance * light_distance);


    vec3 lighting = (ambient_light + diffuse_light + specular_light) * attenuation;

    c = vec4(model_color * lighting, 1.0);
}
