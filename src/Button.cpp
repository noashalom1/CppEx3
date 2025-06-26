// Author: noapatito123@gmail.com
#include "Button.hpp"

/**
 * @brief Constructs a Button with the given text, font, size, and position.
 *
 * Sets up the shape, label, colors, and centers the text within the button.
 *
 * @param text The label text to display on the button.
 * @param font The font to use for the text.
 * @param size The size of the button.
 * @param position The position of the button on the window.
 */
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

/**
 * @brief Sets the function to execute when the button is clicked.
 *
 * @param func A callable object to assign as the action.
 */
void Button::setAction(std::function<void()> func) {
    action = std::move(func);
}

/**
 * @brief Draws the button shape and label onto the window.
 *
 * @param window The SFML render window to draw onto.
 */
void Button::draw(sf::RenderWindow& window) const {
    window.draw(shape);
    window.draw(label);
}

/**
 * @brief Checks if a point (x, y) is within the button's bounds.
 *
 * @param x The x-coordinate of the point.
 * @param y The y-coordinate of the point.
 * @return true if the point is inside the button, false otherwise.
 */
bool Button::contains(float x, float y) const {
    return shape.getGlobalBounds().contains(x, y);
}

/**
 * @brief Executes the assigned action if it exists.
 */
void Button::execute() {
    if (action) {
        action();
    }
}

/**
 * @brief Returns the label text of the button.
 *
 * @return The label text as a const reference.
 */
const std::string& Button::getLabel() const {
    return labelText;
}
