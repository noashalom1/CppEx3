// Author: noapatito123@gmail.com
#pragma once

#include <SFML/Graphics.hpp> 
#include <string>            

// A simple text input box for user input
class TextBox final
{
private:
    sf::RectangleShape box; // Visual rectangle of the textbox
    sf::Text text; // Displayed text
    std::string input; // Internal input buffer
    bool isSelected = false; // Indicates if textbox is active

public:
    TextBox(const sf::Font &font, sf::Vector2f size, sf::Vector2f position); // Constructor

    void draw(sf::RenderWindow &window) const; // Draw the textbox on screen
    void handleEvent(const sf::Event &event); // Handle keyboard events

    void setSelected(bool selected); // Set focus state
    std::string getText() const; // Get the current input text
    void clear(); // Clear the input
};
