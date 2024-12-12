#pragma once

#include <glm/vec3.hpp>

#include "common.hpp"



class Light {
public:
    glm::vec3 position = glm::vec3(0.0f);
    rgb color = glm::vec3(1.0f);
    float colorStrength = 0.5f;

    Light(glm::vec3 position = glm::vec3(0.0f), glm::vec3 color = glm::vec3(1.0f));
};