#include <SFML/Graphics.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "matriciesop.hpp"

#include <iostream>



sf::Vector2f shiftFromInput() {
    float x = sf::Keyboard::isKeyPressed(sf::Keyboard::Right) - sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
    float y = sf::Keyboard::isKeyPressed(sf::Keyboard::Down) - sf::Keyboard::isKeyPressed(sf::Keyboard::Up);

    if (abs(x) + abs(y) == 2) {
        // x/sqrt(2) = x/1.414213 = x*0.7071
        return sf::Vector2f(x * 0.7071, y * 0.7071);
    }

    return sf::Vector2f(x, y);
}

float scaleFromInput() {
    return 2. * sf::Keyboard::isKeyPressed(sf::Keyboard::D) - 0.9 * sf::Keyboard::isKeyPressed(sf::Keyboard::A);
}

float rotationFromInput() {
    return sf::Keyboard::isKeyPressed(sf::Keyboard::E) - sf::Keyboard::isKeyPressed(sf::Keyboard::Q);
}


sf::Vector2f multiply(sf::Vector2f point, glm::mat3 matrix) {
    float homogeneousPoint[] = { point.x, point.y, 1 };
    float resultPoint[] = { 0, 0, 1 };

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            resultPoint[i] += matrix[i][j] * homogeneousPoint[j];
        }
    }

    return sf::Vector2f(resultPoint[0], resultPoint[1]);
}


void applyTransformaitonMatrix(const glm::mat3 &matrix, const std::vector<sf::Vector2f> points, sf::ConvexShape &rectangle) {
    for (int i = 0; i < points.size(); ++i) {
        sf::Vector2f realCoordinates = multiply(points[i], matrix);
        rectangle.setPoint(i, realCoordinates);
    }
}


const unsigned int windowWidth = 1280;
const unsigned int windowHeight = 720;

const float movementSpeed = 1000;
const float rotationSpeed = 5;
const float scaleSpeed = 2;

sf::Clock deltaClock;


int main() {
    sf::ConvexShape rectangle;
    sf::Vector2f rectangleSize(250, 150);

    const std::vector<sf::Vector2f> rectanglePoints = {
        { 0,               0               },
        { rectangleSize.x, 0               },
        { rectangleSize.x, rectangleSize.y },
        { 0,               rectangleSize.y }
    };

    rectangle.setPointCount(4);

    sf::Color currentColor(200, 30, 255);
    rectangle.setFillColor(currentColor);


    float currentRotation = 0;
    glm::mat3 rotationMatrix = defaultTransformationMatrix;

    float currentScale = 1;
    glm::mat3 scaleMatrix = defaultTransformationMatrix;

    sf::Vector2f currentShift(0, 0);
    glm::mat3 shiftMatrix = defaultTransformationMatrix;


    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Lab 1: Floating rectangle", sf::Style::Close);    

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        sf::Time deltaTime = deltaClock.restart();

        // read rotation
        float rotationDelta = rotationFromInput() * rotationSpeed * deltaTime.asSeconds();
        currentRotation += rotationDelta;
        setRotationMatrix(rotationMatrix, currentRotation, rectangleSize/2.f);

        // read scaling
        float scaleDelta = scaleFromInput() * scaleSpeed * deltaTime.asSeconds();
        currentScale += scaleDelta;
        setScaleMatrix(scaleMatrix, currentScale);

        // read shift
        sf::Vector2f shiftDelta = shiftFromInput() * movementSpeed * deltaTime.asSeconds();
        currentShift += shiftDelta;
        setShiftMatrix(shiftMatrix, currentShift);

        glm::mat3 transformationMatrix = rotationMatrix * scaleMatrix * shiftMatrix;

        // for (int i = 0; i < 3; ++i) {
        //     for (int j = 0; j < 3; ++j) {
        //         std::cout << transformationMatrix[i][j] << ' ';
        //     }
        //     std::cout << '\n';
        // }
        // std::cout << '\n';

 
        applyTransformaitonMatrix(transformationMatrix, rectanglePoints, rectangle);

        window.clear();
        window.draw(rectangle);
        window.display();
    }

    return 0;
}