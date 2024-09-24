#pragma once
#include <iostream>
#include <functional>

#include <SFML/Graphics.hpp>


namespace button {
    namespace style {
        enum {
            none = 0,
            save = 1,
            cancel = 2,
            clean = 3,
        };
    };

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
        Button(const std::string& text, sf::ConvexShape& shape, const sf::Font& font, const sf::Vector2f& position, const sf::Uint32& style);
        ~Button() {};

        void setCallback(const std::function<void()>& callback) { this->callback = callback; };

        void setBackgroundNoramlColor(const sf::Color& color) { this->backgroundNormalColor = color; };
        void setBackgroundHoverColor(const sf::Color& color) { this->backgourndHoverColor = color; };
        void setBackgroundClickedColor(const sf::Color& color) { this->backgroundClickedColor = color; };

        void setTextNormalColor(const sf::Color& color) { this->textNormalColor = color; };
        void setTextHoverColor(const sf::Color& color) { this->textHoverColor = color; };
        void setTextClickedColor(const sf::Color& color) { this->textClickedColor = color; };

        void setPosition(const sf::Vector2f& position) { this->position = position; };
        void setFontSize(const unsigned int& size);

        void setText(const std::string& text) {
            this->text.setString(text);
            this->textShadow = this->text;
        };

        void setStyle(const sf::Uint32& style);
        void setFont(sf::Font& font) {
            this->text.setFont(font);
            this->textShadow = this->text;
        };

        sf::Vector2f getPosition() const { return position; };
        sf::Vector2f getDimensions() const { return sf::Vector2f(shape.getGlobalBounds().width, shape.getGlobalBounds().height); };
        sf::Uint32 getState() const { return state; };

        void update(const sf::Event& event, const sf::RenderWindow& window);

    private:
        std::function<void()> callback;

        sf::Color backgroundNormalColor;
        sf::Color backgourndHoverColor;
        sf::Color backgroundClickedColor;

        sf::Color textNormalColor;
        sf::Color textHoverColor;
        sf::Color textClickedColor;

        sf::ConvexShape shape;
        sf::Vector2f size;
        sf::Vector2f position;

        sf::Uint32 style;
        sf::Uint32 state = button::state::normal;

        sf::Font font;
        unsigned int fontSize;

        sf::Text text;
        sf::Text textShadow;


        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};