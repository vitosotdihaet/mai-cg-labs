#include <SFML/Graphics.hpp>

#include "model.hpp"



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
    return 2. * sf::Keyboard::isKeyPressed(sf::Keyboard::D) - 0.95 * sf::Keyboard::isKeyPressed(sf::Keyboard::A);
}

float rotationFromInput() {
    return sf::Keyboard::isKeyPressed(sf::Keyboard::E) - sf::Keyboard::isKeyPressed(sf::Keyboard::Q);
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
        // uncomment for a polygon
        // { rectangleSize.x/2.f, rectangleSize.y * 2.f },
        { 0,               rectangleSize.y }
    };

    const sf::Color currentColor(50, 50, 50);
    rectangle.setFillColor(currentColor);
    rectangle.setPointCount(rectanglePoints.size());

    for (uint64_t i = 0; i < rectanglePoints.size(); ++i) {
        rectangle.setPoint(i, rectanglePoints[i]);
    }

    Model rectangleModel(rectangle);

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Lab 1: Floating rectangle", sf::Style::Close);    

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // fps independent rendering
        sf::Time deltaTime = deltaClock.restart();

        // read parameters
        float rotationDelta = rotationFromInput() * rotationSpeed * deltaTime.asSeconds();
        float scaleDelta = scaleFromInput() * scaleSpeed * deltaTime.asSeconds();
        sf::Vector2f shiftDelta = shiftFromInput() * movementSpeed * deltaTime.asSeconds();

        rectangleModel.update(shiftDelta, rotationDelta, scaleDelta);

        window.clear();
        window.draw(rectangle);
        window.display();
    }

    return 0;
}