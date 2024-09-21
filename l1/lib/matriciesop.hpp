#include <SFML/System/Vector2.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>


const glm::mat3 defaultTransformationMatrix = glm::mat3(1.f);


void setShiftMatrix(glm::mat3 &matrix, const sf::Vector2f &shift) {
    matrix[0][2] = shift.x;
    matrix[1][2] = shift.y;
}

void setRotationMatrix(glm::mat3 &matrix, const float &angle, const sf::Vector2f &rotationPoint) {
    setShiftMatrix(matrix, sf::Vector2f(0, 0));

    matrix[0][0] = cos(angle);
    matrix[1][1] = cos(angle);
    matrix[0][1] = -sin(angle);
    matrix[1][0] = sin(angle);

    glm::mat3 preRotation = defaultTransformationMatrix;
    setShiftMatrix(preRotation, -rotationPoint);

    glm::mat3 postRotation = defaultTransformationMatrix;
    setShiftMatrix(postRotation, rotationPoint);

    matrix = preRotation * matrix * postRotation;
}

void setScaleMatrix(glm::mat3 &matrix, const float &scale) {
    matrix[0][0] = scale;
    matrix[1][1] = scale;
}