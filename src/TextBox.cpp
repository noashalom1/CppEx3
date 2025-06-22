// Author: noapatito123@gmail.com
#include "TextBox.hpp"

TextBox::TextBox(const sf::Font& font, sf::Vector2f size, sf::Vector2f position) {
    box.setSize(size);
    box.setPosition(position);
    box.setFillColor(sf::Color::White);
    box.setOutlineThickness(2);
    box.setOutlineColor(sf::Color::Black);

    text.setFont(font);
    text.setCharacterSize(18);
    text.setFillColor(sf::Color::Black);
    text.setPosition(position.x + 5, position.y + 5);
}

void TextBox::draw(sf::RenderWindow& window) const {
    window.draw(box);
    window.draw(text);
}

void TextBox::handleEvent(const sf::Event& event) {
    if (!isSelected) return;

    if (event.type == sf::Event::TextEntered) {
        if (event.text.unicode == 8 && !input.empty()) {
            input.pop_back();
        } else if (event.text.unicode < 128 && event.text.unicode >= 32) {
            input += static_cast<char>(event.text.unicode);
        }
        text.setString(input);
    }
}

void TextBox::setSelected(bool selected) {
    isSelected = selected;
    box.setOutlineColor(selected ? sf::Color::Blue : sf::Color::Black);
}

std::string TextBox::getText() const {
    return input;
}

void TextBox::clear() {
    input.clear();
    text.setString("");
}
