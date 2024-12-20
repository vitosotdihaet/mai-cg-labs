#include <glm/matrix.hpp>
#include <glm/trigonometric.hpp>



void setModelMatrix(glm::mat4 &m, const glm::vec3 &shift, const glm::vec3 &rotation, const double scale) noexcept {
    glm::mat4 t(1.0f);
    t[3] = glm::vec4(shift.x, shift.y, shift.z, 1.0f);

    const double xRotation = rotation.x;
    float xCosine = cos(xRotation);
    float xSine = sin(xRotation);
    glm::mat4 xRotationMatrix(1.0f);
    xRotationMatrix[1][1] = xCosine; xRotationMatrix[1][2] = -xSine;
    xRotationMatrix[2][1] = xSine;   xRotationMatrix[2][2] = xCosine;

    const double yRotation = rotation.y;
    float yCosine = cos(yRotation);
    float ySine = sin(yRotation);
    glm::mat4 yRotationMatrix(1.0f);
    yRotationMatrix[0][0] = yCosine; yRotationMatrix[0][2] = ySine;
    yRotationMatrix[2][0] = -ySine;  yRotationMatrix[2][2] = yCosine;

    const double zRotation = rotation.z;
    float zCosine = cos(zRotation);
    float zSine = sin(zRotation);
    glm::mat4 zRotationMatrix(1.0f);
    zRotationMatrix[0][0] = zCosine; zRotationMatrix[0][1] = -zSine;
    zRotationMatrix[1][0] = zSine;   zRotationMatrix[1][1] = zCosine;

    glm::mat4 r = xRotationMatrix * yRotationMatrix * zRotationMatrix;

    glm::mat4 s(scale);
    s[3][3] = 1.0f;

    m = t * r * s;
}


void setViewMatrix(glm::mat4 &m, const glm::vec3 &eye, const glm::vec3 &lookat, const glm::vec3 &worldUp) noexcept {
    glm::vec3 forward = glm::normalize(lookat - eye);
    glm::vec3 right = glm::normalize(glm::cross(worldUp, forward));
    glm::vec3 up = glm::cross(forward, right);


    m[0][0] = right.x;               m[0][1] = up.x;               m[0][2] = forward.x;
    m[1][0] = right.y;               m[1][1] = up.y;               m[1][2] = forward.y;
    m[2][0] = right.z;               m[2][1] = up.z;               m[2][2] = forward.z;
    m[3][0] = -glm::dot(right, eye); m[3][1] = -glm::dot(up, eye); m[3][2] = -glm::dot(forward, eye);
    m[3][3] = 1.0f;
}


void setPerspectiveProjectionMatrix(glm::mat4 &m, const float fov, const double near, const double far, const double aspectRatio) noexcept {
    float tanHalfFov = glm::tan(glm::radians(fov) * 0.5f);
    float range = far - near;

    m[0][0] = 1.0f / (aspectRatio * tanHalfFov);
    m[1][1] = 1.0f / tanHalfFov;
    m[2][2] = far / range;
    m[2][3] = 1.0f;
    m[3][2] = -(near * far) / range;
}
