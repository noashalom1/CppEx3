// Author: noapatito123@gmail.com
#include "Button.hpp"

Button::Button(const std::string& text, const sf::Font& font,
               const sf::Vector2f& size, const sf::Vector2f& position) {
    labelText = text; //  Save button text

    shape.setSize(size);
    shape.setPosition(position);
    shape.setFillColor(sf::Color(200, 200, 200));
    shape.setOutlineColor(sf::Color::Black);
    shape.setOutlineThickness(2);

    label.setFont(font);
    label.setString(text);
    label.setCharacterSize(16);
    label.setFillColor(sf::Color::Black);

    sf::FloatRect textBounds = label.getLocalBounds();
    label.setOrigin(textBounds.left + textBounds.width / 2,
                    textBounds.top + textBounds.height / 2);
    label.setPosition(
        position.x + size.x / 2,
        position.y + size.y / 2
    );
}

void Button::setAction(std::function<void()> func) {
    action = std::move(func);
}

void Button::draw(sf::RenderWindow& window) const {
    window.draw(shape);
    window.draw(label);
}

bool Button::contains(float x, float y) const {
    return shape.getGlobalBounds().contains(x, y);
}

void Button::execute() {
    if (action) {
        action();
    }
}

const std::string& Button::getLabel() const {
    return labelText;
}
