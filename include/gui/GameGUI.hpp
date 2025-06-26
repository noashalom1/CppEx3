// Author: noapatito123@gmail.com
#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <functional>
#include "Button.hpp"
#include "TextBox.hpp"
#include "Game.hpp"
#include <memory>

namespace coup
{

    enum class GUIState
    { // GUI states: setup screen, in-game, or target selection
        Setup,
        InGame,
        TargetSelection
    };

    class GameGUI
    {
    public:
        GameGUI();             // Constructor
        void run();            // Start the GUI loop
        void setupButtons();   // Create and arrange buttons
        void drawPlayerList(); // Show current players on screen
        // Add special action buttons for a given role
        int addRoleActionButtons(const std::string &role,
                                 const std::string &buttonPrefix,
                                 float startY,
                                 std::function<void(const std::shared_ptr<Player> &)> actionPerPlayer);
        std::string inGameError; // Stores in-game error message

    private:
        sf::RenderWindow window;                                           // SFML render window
        sf::Font font;                                                     // Font used in the GUI
        std::vector<Button> buttons;                                       // General buttons in the GUI
        std::vector<Button> targetButtons;                                 // Buttons for choosing a target
        std::function<void(const std::shared_ptr<Player> &)> targetAction; // Action to execute on selected target

        TextBox *nameBox;     // Input for player name
        Button *addPlayerBtn; // Button to add a player
        Button *startGameBtn; // Button to start the game
        Button *demoGameBtn;  // Button to start a demo game

        std::vector<std::string> tempNames; // Temp storage for player names
        std::vector<std::string> tempRoles; // Temp storage for player roles
        std::string setupError;             // Setup error message
        std::string actionMessage;          // Message about the last action
        std::string winnerMessage;          // Message shown when someone wins
        bool showVictory = false;           // Flag to indicate if victory screen should be shown

        Game game;                        // Main game object
        GUIState state = GUIState::Setup; // Current GUI state

        // Display menu for selecting a target player
        void showTargetSelection(std::function<void(const std::shared_ptr<Player> &)> action, const std::vector<std::shared_ptr<Player>> targets, bool includeCurrentPlayer = false);

        std::string randomRole(); // Returns a random role
    };

} // namespace coup
