// Author: noapatito123@gmail.com
#pragma once

#include <SFML/Graphics.hpp> 
#include <functional>       

class Button
{
private:
    sf::RectangleShape shape;     // Visual shape of the button
    sf::Text label;               // Text displayed on the button
    std::function<void()> action; // Action to perform when clicked
    std::string labelText;        // Text content of the label

public:
    // Constructor to initialize button with text, font, size, and position
    Button(const std::string &text, const sf::Font &font,
           const sf::Vector2f &size, const sf::Vector2f &position);

    void setAction(std::function<void()> func); // Set the action to execute
    void draw(sf::RenderWindow &window) const;  // Draw the button on the window
    bool contains(float x, float y) const;      // Check if a point is inside the button
    void execute();                             // Execute the assigned action
    const std::string &getLabel() const;        // Get the label text
};
