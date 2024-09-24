#pragma once

#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>


class Model {
public:
    Model(sf::ConvexShape& shape);
    void update(const sf::Vector2f& shiftDelta, const float& rotationAngleDelta, const float& scaleFactorDelta);

    void toggleColors() { this->color = !this->color; };

private:
    sf::ConvexShape& shape;
    std::vector<sf::Vector2f> shapePoints;
    sf::Vector2f shapeCenter = sf::Vector2f(0, 0);
    sf::Color initialShapeColor = sf::Color::White;
    sf::Color shapeColor = initialShapeColor;

    bool color = true;

    sf::Vector2f shift = sf::Vector2f(0, 0);
    float rotationAngle = 0;
    float scaleFactor = 1;

    glm::mat3 shiftMatrix = glm::mat3(1.);
    glm::mat3 rotationMatrix = glm::mat3(1.);
    glm::mat3 scaleMatrix = glm::mat3(1.);

    glm::mat3 shiftToCenterMatrix = glm::mat3(1.);
    glm::mat3 shiftFromCenterMatrix = glm::mat3(1.);

    glm::mat3 modelMatrix = glm::mat3(1.);

    void updateShiftMatrix() noexcept;
    void updateRotationMatrix() noexcept;
    void updateScaleMatrix() noexcept;
};