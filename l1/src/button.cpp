#include <button.hpp>


Button::Button(const std::string& text, sf::ConvexShape& shape, const sf::Font& font, const sf::Vector2f& position) : position(position) {
    // default colors
    this->setColors();

    // set up the shape
    this->shape = shape;
    this->shape.setOrigin(this->shape.getGlobalBounds().width/2, this->shape.getGlobalBounds().height/2);
    this->shape.setPosition(this->position);

    // set up the text
    this->text.setString(text);
    this->text.setFont(font);
    this->text.setOrigin(this->text.getGlobalBounds().width/2, this->text.getGlobalBounds().height/2);
    this->text.setPosition(this->shape.getPosition());
    this->text.setFillColor(this->textColor);

    this->textShadow = this->text;
    this->textShadow.setPosition(this->text.getPosition() - sf::Vector2f(3, 3));
    this->textShadow.setFillColor(this->textShadowColor);
}


void Button::setFontSize(const unsigned int& fontSize) {
    this->fontSize = fontSize;

    this->text.setCharacterSize(this->fontSize);
    this->text.setOrigin(this->text.getGlobalBounds().width/2, this->text.getGlobalBounds().height/2);

    this->textShadow = this->text;
    this->textShadow.setPosition(this->text.getPosition() - sf::Vector2f(3, 3));
    this->textShadow.setFillColor(this->textShadowColor);
}

void Button::setColors(
    const sf::Color& textColor,
    const sf::Color& textShadowColor,
    const sf::Color& backgroundNormalColor,
    const sf::Color& backgroundHoverColor,
    const sf::Color& backgroundClickedColor
) {
    this->textColor = textColor;
    this->textShadowColor = textShadowColor;
    this->backgroundNormalColor = backgroundNormalColor;
    this->backgroundHoverColor = backgroundHoverColor;
    this->backgroundClickedColor = backgroundClickedColor;
}

void Button::update(const sf::Event& event, const sf::RenderWindow& window) {
    // perform updates for settings from user
    this->shape.setOrigin(this->shape.getGlobalBounds().width/2, this->shape.getGlobalBounds().height/2);

    this->shape.setPosition(this->position);
    this->text.setOrigin(this->text.getGlobalBounds().width/2, this->text.getGlobalBounds().height/2);
    this->text.setPosition(this->shape.getPosition());
    this->text.setFillColor(this->textColor);

    this->textShadow = this->text;
    this->textShadow.setPosition(this->text.getPosition() + sf::Vector2f(3, -3));
    this->textShadow.setFillColor(this->textShadowColor);

    // perform updates for user mouse interactions
    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

    bool mouseInButton = mousePosition.x >= this->shape.getPosition().x - this->shape.getGlobalBounds().width/2
                      && mousePosition.x <= this->shape.getPosition().x + this->shape.getGlobalBounds().width/2
                      && mousePosition.y >= this->shape.getPosition().y - this->shape.getGlobalBounds().height/2
                      && mousePosition.y <= this->shape.getPosition().y + this->shape.getGlobalBounds().height/2;

    if (mouseInButton) {
        if (event.type == sf::Event::MouseMoved) {
            this->state = button::state::hovered;
        }

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            this->state = button::state::clicked;
        }

        if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
            if (mouseInButton) {
                this->state = button::state::hovered;
                if (this->callback) this->callback();
            }
        }
    } else {
        this->state = button::state::normal;
    }

    switch (this->state) {
        case button::state::normal:
            this->shape.setFillColor(this->backgroundNormalColor);
            this->text.setFillColor(this->textColor);
            break;

        case button::state::hovered:
            this->shape.setFillColor(this->backgroundHoverColor);
            break;

        case button::state::clicked:
            this->shape.setFillColor(this->backgroundClickedColor);
            break;
        
        default:
            break;
    }
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(this->shape, states);
    target.draw(this->textShadow, states);
    target.draw(this->text, states);
}

