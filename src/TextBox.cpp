// Author: noapatito123@gmail.com
#include "TextBox.hpp"
#include <memory>

/**
 * @brief Constructs a TextBox with given font, size, and position.
 *
 * Initializes the rectangle and the text field with default styles.
 *
 * @param font The font used for rendering text.
 * @param size The size of the text box.
 * @param position The position of the text box on the screen.
 */
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

/**
 * @brief Draws the text box and its text onto the window.
 *
 * @param window The SFML render window where the text box should be drawn.
 */
void TextBox::draw(sf::RenderWindow& window) const {
    window.draw(box);
    window.draw(text);
}

/**
 * @brief Handles keyboard input events for typing into the text box.
 *
 * Supports basic ASCII input and backspace when the box is selected.
 *
 * @param event The SFML event containing user input.
 */
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

/**
 * @brief Sets the selection state of the text box.
 *
 * When selected, the outline color changes and input is accepted.
 *
 * @param selected True to select the box; false to deselect.
 */
void TextBox::setSelected(bool selected) {
    isSelected = selected;
    box.setOutlineColor(selected ? sf::Color::Blue : sf::Color::Black);
}

/**
 * @brief Returns the current text entered in the text box.
 *
 * @return The input string.
 */
std::string TextBox::getText() const {
    return input;
}

/**
 * @brief Clears the text content of the text box.
 */
void TextBox::clear() {
    input.clear();
    text.setString("");
}
