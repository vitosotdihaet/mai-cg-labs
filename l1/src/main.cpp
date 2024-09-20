#include <SFML/Graphics.hpp>

#include <math.h>


sf::Vector2f directionFromInput() {
    float x = sf::Keyboard::isKeyPressed(sf::Keyboard::Right) - sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
    float y = sf::Keyboard::isKeyPressed(sf::Keyboard::Down) - sf::Keyboard::isKeyPressed(sf::Keyboard::Up);

    if (abs(x) + abs(y) == 2) {
        return sf::Vector2f(x * 0.7071, y * 0.7071); // x / 1.414213 = x * 0.7071
    }

    return sf::Vector2f(x, y);
}


sf::Clock deltaClock;

int main() {
    const unsigned int windowWidth = 1600;
    const unsigned int windowHeight = 900;

    const float speed = 1000;

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Lab 1: Floating rectangle", sf::Style::Close);
    
    sf::RectangleShape rectangle(sf::Vector2f(250, 150));
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

        const sf::Vector2f deltaDirection = directionFromInput() * deltaTime.asSeconds() * speed;
        rectangle.setPosition(rectangle.getPosition() + deltaDirection);

        window.clear();
        window.draw(rectangle);
        window.display();
    }

    return 0;
}