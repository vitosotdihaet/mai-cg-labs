#include <SFML/System/Vector2.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>


void setShiftMatrix(glm::mat3 &matrix, sf::Vector2f shift) {
    matrix[0][2] = shift.x;
    matrix[1][2] = shift.y;
}

void setRotationMatrix(glm::mat3 &matrix, float angle) {
    matrix[0][1] = -sin(angle);
    matrix[1][0] = sin(angle);
    matrix[0][0] = cos(angle);
    matrix[1][1] = cos(angle);
}

void setScaleMatrix(glm::mat3 &matrix, float scale) {
    matrix[0][0] = scale;
    matrix[1][1] = scale;
}