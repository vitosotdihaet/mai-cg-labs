#pragma once

#include <glm/vec3.hpp>
#include <glm/matrix.hpp>
#include <glm/trigonometric.hpp>

#include "common.hpp"


struct Camera {
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(0.0f);

    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 rotation = glm::vec3(0.0f);

    const float minXRotation = -PI_FLOAT * 0.49f;
    const float maxXRotation =  PI_FLOAT * 0.49f;

    const float movementSpeed = 0.1f;
    const float rotationSpeed = 0.001f;
    const float fovSpeed = 0.1f;

    float fov = 60.0f;

    const float minFov = 1.0f;
    const float maxFov = 359.0f;

    const float near = 1e-5f;
    const float far = 1e10f;

    glm::vec3 forward = glm::vec3(0.0f);
    glm::vec3 right = glm::vec3(0.0f);
    glm::vec3 up = glm::vec3(0.0f);

    glm::vec2 lastMousePosition;


    Camera() {}
    Camera(glm::vec3 position, glm::vec3 rotation);

    void update(const double aspectRatio);

    void setView() noexcept;
    void setPerspectiveProjection(const double aspectRatio) noexcept;

    void onMouseMove(const double x, const double y);
};