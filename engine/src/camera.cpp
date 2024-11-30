#include "camera.hpp"


Camera::Camera(glm::vec3 position, glm::vec3 rotation) : position(position), rotation(rotation) {}


void Camera::update(const double aspectRatio) {
    this->rotation.y = glm::mod(this->rotation.y, 2.0f * PI_FLOAT);

    if (this->rotation.x < this->minXRotation) {
        this->rotation.x = this->minXRotation;
    } else if (this->rotation.x > this->maxXRotation) {
        this->rotation.x = this->maxXRotation;
    }

    if (this->fov < this->minFov) {
        this->fov = this->minFov;
    } else if (this->fov > this->maxFov) {
        this->fov = this->maxFov;
    }

    this->forward = glm::normalize(glm::vec3(
        -glm::cos(this->rotation.x) * glm::sin(this->rotation.y),
        -glm::sin(this->rotation.x),
        glm::cos(this->rotation.x) * glm::cos(this->rotation.y)
    ));
    this->right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), this->forward));
    this->up = glm::normalize(glm::cross(this->forward, this->right));

    this->setView();
    this->setPerspectiveProjection(aspectRatio);
}

void Camera::setView() noexcept {
    this->view[0][0] = this->right.x;                          this->view[0][1] = this->up.x;                          this->view[0][2] = this->forward.x;
    this->view[1][0] = this->right.y;                          this->view[1][1] = this->up.y;                          this->view[1][2] = this->forward.y;
    this->view[2][0] = this->right.z;                          this->view[2][1] = this->up.z;                          this->view[2][2] = this->forward.z;
    this->view[3][0] = -glm::dot(this->right, this->position); this->view[3][1] = -glm::dot(this->up, this->position); this->view[3][2] = -glm::dot(this->forward, this->position);
    this->view[3][3] = 1.0f;
}

void Camera::setPerspectiveProjection(const double aspectRatio) noexcept {
    float tanHalfFov = glm::tan(glm::radians(this->fov) * 0.5f);
    float range = this->far - this->near;

    this->projection[0][0] = 1.0f / (aspectRatio * tanHalfFov);
    this->projection[1][1] = 1.0f / tanHalfFov;
    this->projection[2][2] = this->far / range;
    this->projection[2][3] = 1.0f;
    this->projection[3][2] = -(this->near * this->far) / range;
}

void Camera::onMouseMove(const double x, const double y) {
    this->rotation.y -= (x - this->lastMousePosition.x) * rotationSpeed;
    this->rotation.x += (y - this->lastMousePosition.y) * rotationSpeed;
    this->lastMousePosition.x = x;
    this->lastMousePosition.y = y;
}