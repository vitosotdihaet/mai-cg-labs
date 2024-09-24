#include <SFML/Graphics.hpp>

#include "model.hpp"
#include "button.hpp"


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
    return sf::Keyboard::isKeyPressed(sf::Keyboard::D) - sf::Keyboard::isKeyPressed(sf::Keyboard::A);
}

float rotationFromInput() {
    return sf::Keyboard::isKeyPressed(sf::Keyboard::E) - sf::Keyboard::isKeyPressed(sf::Keyboard::Q);
}


const sf::Vector2i windowSize(1280, 720);

const float movementSpeed = 1000;
const float rotationSpeed = 5;
const float scaleSpeed = 2;

sf::Clock deltaClock;


const sf::Vector2f rectangleSize(250, 150);
const std::vector<sf::Vector2f> rectanglePoints = {
    { 0,               0               },
    { rectangleSize.x, 0               },
    { rectangleSize.x, rectangleSize.y },
    // uncomment for a polygon
    // { rectangleSize.x/2.f, rectangleSize.y * 2.f },
    { 0,               rectangleSize.y }
};


const sf::Vector2f buttonPosition(1200, 55);
const sf::Vector2f buttonSize(150, 100);
const std::vector<sf::Vector2f> buttonShapePoints = {
    { 0,            0            },
    { buttonSize.x, 0            },
    { buttonSize.x, buttonSize.y },
    { 0,            buttonSize.y }
};



int main() {
    // set up the rectangle model
    sf::ConvexShape rectangle(buttonShapePoints.size());

    // set initial rectangle color
    const sf::Color currentColor(50, 50, 50);
    rectangle.setFillColor(currentColor);

    // set initial rectangle points
    for (uint64_t i = 0; i < rectanglePoints.size(); ++i) {
        rectangle.setPoint(i, rectanglePoints[i]);
    }

    // create a model from rectangle
    Model rectangleModel(rectangle);

    // set up the color toggle button
    sf::ConvexShape buttonShape(buttonShapePoints.size());
    for (uint64_t i = 0; i < buttonShapePoints.size(); ++i) {
        buttonShape.setPoint(i, buttonShapePoints[i]);
    }

    sf::Font buttonFont;
    buttonFont.loadFromFile("./resources/fonts/LiberationMono-Bold.ttf");

    Button button("toggle\ncolors", buttonShape, buttonFont, buttonPosition);
    button.setColors(
        sf::Color(255, 255, 255),
        sf::Color(75, 75, 75),
        sf::Color(10, 10, 10, 200),
        sf::Color(50, 50, 50, 200),
        sf::Color(75, 75, 75, 200)
    );

    button.setCallback([&rectangleModel]() {
        rectangleModel.toggleColors();
    });


    sf::RenderWindow window(sf::VideoMode(windowSize.x, windowSize.y), "Lab 1: Floating rectangle", sf::Style::Close);
    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            button.update(event, window);
        }

        // fps independent rendering
        sf::Time deltaTime = deltaClock.restart();

        float dt = deltaTime.asSeconds();


        // read parameters
        float rotationDelta = rotationFromInput() * rotationSpeed * dt;
        float scaleDelta = scaleFromInput() * scaleSpeed * dt;
        sf::Vector2f shiftDelta = shiftFromInput() * movementSpeed * dt;

        rectangleModel.update(shiftDelta, rotationDelta, scaleDelta);


        window.clear();
        window.draw(rectangle);
        window.draw(button);
        window.display();
    }

    return 0;
}