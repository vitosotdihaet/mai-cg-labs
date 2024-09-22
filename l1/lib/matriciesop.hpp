#include <SFML/System/Vector2.hpp>

#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include <iostream>


sf::Vector2f multiplyHomogenous(sf::Vector2f point, glm::mat3 matrix) {
    glm::vec3 homogeneousPoint(point.x, point.y, 1.);
    glm::vec3 resultPoint = homogeneousPoint * matrix;
    return sf::Vector2f(resultPoint[0], resultPoint[1]);
}

void printMatrix(glm::mat3 matrix) {
    std::cout << glm::to_string(matrix) << '\n';
}