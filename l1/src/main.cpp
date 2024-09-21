#include <SFML/Graphics.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <iostream>



sf::Vector2f directionFromInput() {
    float x = sf::Keyboard::isKeyPressed(sf::Keyboard::Right) - sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
    float y = sf::Keyboard::isKeyPressed(sf::Keyboard::Down) - sf::Keyboard::isKeyPressed(sf::Keyboard::Up);

    if (abs(x) + abs(y) == 2) {
        return sf::Vector2f(x * 0.7071, y * 0.7071); // x / 1.414213 = x * 0.7071
    }

    return sf::Vector2f(x, y);
}

float rotationFromInput() {
    return sf::Keyboard::isKeyPressed(sf::Keyboard::E) - sf::Keyboard::isKeyPressed(sf::Keyboard::Q);
}

float scaleFromInput() {
    return 2 * sf::Keyboard::isKeyPressed(sf::Keyboard::D) - 0.5 * sf::Keyboard::isKeyPressed(sf::Keyboard::A);
}


sf::Vector2f multiply(sf::Vector2f point, glm::mat3 matrix) {
    float homoPoint[] = { point.x, point.y, 1 };
    float outPoint[] = { 0, 0, 1 };

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            outPoint[i] += matrix[i][j] * homoPoint[j];
        }
    }

    return sf::Vector2f(outPoint[0], outPoint[1]);
}


sf::Clock deltaClock;

int main() {
    const unsigned int windowWidth = 1600;
    const unsigned int windowHeight = 900;

    const float movementSpeed = 1000;
    const float rotationSpeed = 10;
    const float scaleSpeed = 1;

    glm::mat3 rectangleTransformationMatrix = glm::mat3(
        1., 0., 0.,
        0., 1., 0.,
        0., 0., 1.
    );

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Lab 1: Floating rectangle", sf::Style::Close);
    
    const sf::Vector2f rectanglePoints[4] = {
        { 0, 0 },
        { 0, 150 },
        { 250, 150 },
        { 250, 0 }
    };

    sf::ConvexShape rectangle;
    rectangle.setPointCount(4);
    for (int i = 0; i < 4; ++i) {
        rectangle.setPoint(i, rectanglePoints[i]);
    }

    sf::Color currentColor(200, 30, 255);
    rectangle.setFillColor(currentColor);

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        sf::Time deltaTime = deltaClock.restart();

        // TODO: logic
        sf::Vector2f directionDelta = directionFromInput() * movementSpeed * deltaTime.asSeconds();
        rectangleTransformationMatrix[0][2] += directionDelta.x;
        rectangleTransformationMatrix[1][2] += directionDelta.y;

        float scaleDelta = scaleFromInput() * scaleSpeed * deltaTime.asSeconds();
        rectangleTransformationMatrix[0][0] *= (1. + scaleDelta);
        rectangleTransformationMatrix[1][1] *= (1. + scaleDelta);

        // for (int i = 0; i < 3; ++i) {
        //     for (int j = 0; j < 3; ++j) {
        //         std::cout << rectangleTransformationMatrix[i][j] << ' ';
        //     }
        //     std::cout << '\n';
        // }
        // std::cout << '\n';

        for (int i = 0; i < 4; ++i) {
            sf::Vector2f realCoordinates = multiply(rectanglePoints[i], rectangleTransformationMatrix);
            rectangle.setPoint(i, realCoordinates);
        }

        window.clear();
        window.draw(rectangle);
        window.display();
    }

    return 0;
}