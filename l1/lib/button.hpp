#pragma once
#include <functional>

#include <SFML/Graphics.hpp>


namespace button {
    namespace state {
        enum {
            normal = 0,
            hovered = 1,
            clicked = 2
        };
    };
}

class Button : public sf::Drawable {
    public:
        Button(const std::string& text, sf::ConvexShape& shape, const sf::Font& font, const sf::Vector2f& position);
        ~Button() {};

        void setCallback(const std::function<void()>& callback) { this->callback = callback; };

        void setPosition(const sf::Vector2f& position) { this->position = position; };
        void setFontSize(const unsigned int& size);

        void setText(const std::string& text) { this->text.setString(text); };

        void setColors(
            const sf::Color& textColor = sf::Color(255, 255, 255),
            const sf::Color& textShadowColor = sf::Color(0, 0, 0),
            const sf::Color& backgroundNormalColor = sf::Color(0, 0, 0),
            const sf::Color& backgroundHoverColor = sf::Color(25, 25, 25),
            const sf::Color& backgroundClickedColor = sf::Color(100, 100, 100)
        );

        void setFont(sf::Font& font) { this->text.setFont(font); };

        sf::Vector2f getPosition() const { return position; };
        sf::Vector2f getDimensions() const { return sf::Vector2f(shape.getGlobalBounds().width, shape.getGlobalBounds().height); };
        sf::Uint32 getState() const { return state; };

        void update(const sf::Event& event, const sf::RenderWindow& window);

    private:
        std::function<void()> callback;

        sf::Color backgroundNormalColor;
        sf::Color backgroundHoverColor;
        sf::Color backgroundClickedColor;

        sf::Color textColor;
        sf::Color textShadowColor;

        sf::ConvexShape shape;
        sf::Vector2f position;

        sf::Uint32 state = button::state::normal;

        sf::Font font;
        unsigned int fontSize = 14;

        sf::Text text;
        sf::Text textShadow;

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};