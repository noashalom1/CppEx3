// Author: noapatito123@gmail.com
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
#include <memory>
using namespace coup;
using namespace sf;

/**
 * @brief Draws the list of players in the game window.
 *
 * This method displays all active players with their names and roles.
 * It also changes the text color and adds status labels based on their game state:
 * - Gray for eliminated players
 * - Dark red for sanctioned players
 * - Red for players who were last arrested
 * - Yellow for players who are both sanctioned and last arrested
 */
void GameGUI::drawPlayerList()
{
    float startY = 500; // Starting Y-coordinate for the first player's label
    for (const std::shared_ptr<Player>& player : game.get_all_players()) // Iterate over all players in the game
    {
        sf::Text info;
        info.setFont(font);  // Use the main game font
        info.setCharacterSize(16); // Set readable text size
        // Construct the label with player name and role
        std::string label = player->get_name() + " - " + player->role();
        if (player->is_eliminated()) // Gray color for eliminated players
        {
            info.setFillColor(sf::Color(150, 150, 150));
        }

        else if (player->is_sanctioned() && player->get_name() == game.get_last_arrested_name())
        {
            // Yellow color for players who are both sanctioned and last arrested
            info.setFillColor(sf::Color(255, 255, 0));
            label += " [BOTH]";
        }
        else if (player->is_sanctioned()) // Dark red for sanctioned players
        {
            info.setFillColor(sf::Color(128, 0, 0));
            label += " [SANCTIONED]";
        }
        else if (player->get_name() == game.get_last_arrested_name()) // Red for last arrested
        {
            info.setFillColor(sf::Color(255, 0, 0));
            label += " [DISABLE TO ARREST]";
        }
        else // Default white color for normal players
        {
            info.setFillColor(sf::Color::White);
        }

        info.setString(label); // Set the player's label as the text content
        info.setPosition(700, startY); // Position the text on the screen
        window.draw(info); // Draw the text to the window
        startY += 25; // Move down for the next player entry
    }
}