// Author: noapatito123@gmail.com
#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class TextBox final
{
private:
    sf::RectangleShape box;
    sf::Text text;
    std::string input;
    bool isSelected = false;

public:
    TextBox(const sf::Font &font, sf::Vector2f size, sf::Vector2f position);

    void draw(sf::RenderWindow &window) const;
    void handleEvent(const sf::Event &event);

    void setSelected(bool selected);
    std::string getText() const;
    void clear();
};