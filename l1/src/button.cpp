#include <button.hpp>


Button::Button(const std::string& text, sf::ConvexShape& shape, const sf::Font& font, const sf::Vector2f& position, const sf::Uint32& style) : position(position), style(style) {
    switch (this->style) {
        case button::style::none:
            this->textNormalColor = sf::Color(255,255,255);
            this->textHoverColor = sf::Color(255,255,255);
            this->textClickedColor = sf::Color(255,255,255);
            this->backgroundNormalColor = sf::Color(255,255,255,100);
            this->backgourndHoverColor = sf::Color(200,200,200,100);
            this->backgroundClickedColor = sf::Color(150,150,150);
            break;

        case button::style::save:
            this->textNormalColor = sf::Color(255,255,255);
            this->textHoverColor = sf::Color(255,255,255);
            this->textClickedColor = sf::Color(255,255,255);
            this->backgroundNormalColor = sf::Color(0,255,0,100);
            this->backgourndHoverColor = sf::Color(0,200,0,100);
            this->backgroundClickedColor = sf::Color(0,150,0);
            break;

        case button::style::cancel:
            this->textNormalColor = sf::Color(255,255,255);
            this->textHoverColor = sf::Color(255,255,255);
            this->textClickedColor = sf::Color(255,255,255);
            this->backgroundNormalColor = sf::Color(255,0,0,100);
            this->backgourndHoverColor = sf::Color(200,0,0,100);
            this->backgroundClickedColor = sf::Color(150,0,0);
            break;

        case button::style::clean:
            this->textNormalColor = sf::Color(255,255,255);
            this->textHoverColor = sf::Color(255,255,255);
            this->textClickedColor = sf::Color(255,255,255);
            this->backgroundNormalColor = sf::Color(0,255,255,100);
            this->backgourndHoverColor = sf::Color(0,200,200,100);
            this->backgroundClickedColor = sf::Color(0,150,150);
            break;
    }

    // set up text
    this->text.setString(text);
    this->text.setFont(font);
    this->text.setOrigin(this->text.getGlobalBounds().width/2, this->text.getGlobalBounds().height/2);
    this->text.setFillColor(textNormalColor);

    // set some defauts
    this->size = sf::Vector2f(this->text.getGlobalBounds().width * 1.5f, this->text.getGlobalBounds().height * 1.5f);

    this->shape = shape;
    this->shape.setOrigin(this->shape.getGlobalBounds().width/2, this->shape.getGlobalBounds().height/2);
    this->shape.setPosition(this->position);

    sf::Vector2f textPosition = sf::Vector2f(this->shape.getPosition().x, this->shape.getPosition().y - this->shape.getGlobalBounds().height/4);

    this->text.setPosition(textPosition);

    this->textShadow.setFont(font);
    this->textShadow = this->text;
    this->textShadow.setOrigin(this->textShadow.getGlobalBounds().width/2, this->textShadow.getGlobalBounds().height/2);
    this->textShadow.setPosition(this->text.getPosition().x + 3.f, this->text.getPosition().y + 3.f);
}


void Button::setFontSize(const unsigned int& size) {
    this->fontSize = size;
    this->text.setCharacterSize(this->fontSize);
    this->text.setOrigin(this->text.getGlobalBounds().width/2, this->text.getGlobalBounds().height/2);
    this->textShadow.setCharacterSize(this->fontSize);
    this->textShadow.setOrigin(this->textShadow.getGlobalBounds().width/2, this->textShadow.getGlobalBounds().height/2);
    this->size = sf::Vector2f(this->text.getGlobalBounds().width * 1.5f, (this->text.getGlobalBounds().height + this->text.getGlobalBounds().height) * 1.5f);
}

void Button::setStyle(const sf::Uint32& style) {
    this->style = style;

    switch (this->style) {
        case button::style::none:
            this->textNormalColor = sf::Color(255,255,255);
            this->textHoverColor = sf::Color(255,255,255);
            this->textClickedColor = sf::Color(255,255,255);
            this->backgroundNormalColor = sf::Color(255,255,255,100);
            this->backgourndHoverColor = sf::Color(200,200,200,100);
            this->backgroundClickedColor = sf::Color(150,150,150);
            break;

        case button::style::save:
            this->textNormalColor = sf::Color(255,255,255);
            this->textHoverColor = sf::Color(255,255,255);
            this->textClickedColor = sf::Color(255,255,255);
            this->backgroundNormalColor = sf::Color(0,255,0,100);
            this->backgourndHoverColor = sf::Color(0,200,0,100);
            this->backgroundClickedColor = sf::Color(0,150,0);
            break;

        case button::style::cancel:
            this->textNormalColor = sf::Color(255,255,255);
            this->textHoverColor = sf::Color(255,255,255);
            this->textClickedColor = sf::Color(255,255,255);
            this->backgroundNormalColor = sf::Color(255,0,0,100);
            this->backgourndHoverColor = sf::Color(200,0,0,100);
            this->backgroundClickedColor = sf::Color(150,0,0);
            break;

        case button::style::clean:
            this->textNormalColor = sf::Color(255,255,255);
            this->textHoverColor = sf::Color(255,255,255);
            this->textClickedColor = sf::Color(255,255,255);
            this->backgroundNormalColor = sf::Color(0,255,255,100);
            this->backgourndHoverColor = sf::Color(0,200,200,100);
            this->backgroundClickedColor = sf::Color(0,150,150);
            break;
    }
}

void Button::update(const sf::Event& event, const sf::RenderWindow& window) {
    //perform updates for settings from user
    this->size = sf::Vector2f(this->text.getGlobalBounds().width * 1.5f, this->text.getGlobalBounds().height * 1.75f);
    // this->shape.setOrigin(this->shape.getGlobalBounds().width/2, this->shape.getGlobalBounds().height/2);
    this->shape.setPosition(this->position);
    this->text.setOrigin(this->text.getGlobalBounds().width/2, this->text.getGlobalBounds().height/2);

    sf::Vector2f textPosition = sf::Vector2f(this->shape.getPosition().x, this->shape.getPosition().y - this->shape.getGlobalBounds().height/4);
    
    this->text.setPosition(textPosition);
    this->text.setFillColor(this->textNormalColor);
    this->textShadow.setOrigin(this->textShadow.getGlobalBounds().width/2, this->textShadow.getGlobalBounds().height/2);
    this->textShadow.setPosition(this->text.getPosition().x + 3.f, this->text.getPosition().y + 3.f);
    this->textShadow.setFillColor(sf::Color(0,0,0));

    //perform updates for user mouse interactions
    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

    bool mouseInButton =    mousePosition.x >= this->shape.getPosition().x - this->shape.getGlobalBounds().width/2
                         && mousePosition.x <= this->shape.getPosition().x + this->shape.getGlobalBounds().width/2
                         && mousePosition.y >= this->shape.getPosition().y - this->shape.getGlobalBounds().height/2
                         && mousePosition.y <= this->shape.getPosition().y + this->shape.getGlobalBounds().height/2;

    if (event.type == sf::Event::MouseMoved) {
        if (mouseInButton) {
            this->state = button::state::hovered;
        } else {
            this->state = button::state::normal;
        }
    }

    if (event.type == sf::Event::MouseButtonPressed) {
        switch(event.mouseButton.button) {
            case sf::Mouse::Left:
                if (mouseInButton) {
                    this->state = button::state::clicked;
                } else {
                    this->state = button::state::normal;
                }
                break;

            default:
                break;
        }
    }

    if (event.type == sf::Event::MouseButtonReleased) {
        switch(event.mouseButton.button) {
            case sf::Mouse::Left:
                if (mouseInButton) {
                    this->state = button::state::hovered;
                    if (callback) {
                        callback();
                    }
                } else {
                    this->state = button::state::normal;
                }
                break;

            default:
                break;
        }
    }

    switch (this->state) {
        case button::state::normal:
            this->shape.setFillColor(this->backgroundNormalColor);
            this->text.setFillColor(this->textNormalColor);
            break;

        case button::state::hovered:
            this->shape.setFillColor(this->backgourndHoverColor);
            this->text.setFillColor(this->textHoverColor);
            break;

        case button::state::clicked:
            this->shape.setFillColor(this->backgroundClickedColor);
            this->text.setFillColor(this->textClickedColor);
            break;
    }
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(shape, states);
    target.draw(textShadow, states);
    target.draw(text, states);
}

