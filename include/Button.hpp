#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <SFML/Graphics.hpp>
#include <functional>

class Button {
private:
    sf::RectangleShape shape;
    sf::Text label;
    std::function<void()> action;
    std::string labelText;

public:
    Button(const std::string& text, const sf::Font& font,
           const sf::Vector2f& size, const sf::Vector2f& position);

    void setAction(std::function<void()> func);
    void draw(sf::RenderWindow& window) const;
    bool contains(float x, float y) const;
    void execute();
    const std::string& getLabel() const;
};

#endif
