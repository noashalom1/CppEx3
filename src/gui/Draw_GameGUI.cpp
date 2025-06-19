#include "GameGUI.hpp"
#include "Player.hpp"
#include "exceptions.hpp"
#include "Governor.hpp"
#include "Spy.hpp"
#include "Baron.hpp"
#include "General.hpp"
#include "Judge.hpp"
#include "Merchant.hpp"
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <ctime>
#include <SFML/Graphics.hpp>

using namespace coup;
using namespace sf;

void GameGUI::drawPlayerList()
{
    float startY = 500;
    for (const Player *p : game.get_players())
    {
        sf::Text info;
        info.setFont(font);
        info.setCharacterSize(16);

        std::string label = p->get_name() + " - " + p->role();
        if (p->is_eliminated())
        {
            info.setFillColor(sf::Color(150, 150, 150));
        }

        else if (p->is_sanctioned() && p->get_name() == game.get_last_arrested_name())
        {
            info.setFillColor(sf::Color(255, 255, 0));
            label += " [BOTH]";
        }
        else if (p->is_sanctioned())
        {
            info.setFillColor(sf::Color(128, 0, 0));
            label += " [SANCTIONED]";
        }
        else if (p->get_name() == game.get_last_arrested_name())
        {
            info.setFillColor(sf::Color(255, 0, 0));
            label += " [DISABLE TO ARREST]";
        }
        else
        {
            info.setFillColor(sf::Color::White);
        }

        info.setString(label);
        info.setPosition(700, startY);
        window.draw(info);
        startY += 25;
    }
}