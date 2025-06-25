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

namespace coup {

enum class GUIState {
    Setup,
    InGame,
    TargetSelection
};

class GameGUI {
public:
    GameGUI();
    void run();
    void setupButtons();
    void drawPlayerList();
    int addRoleActionButtons(const std::string &role,
                             const std::string &buttonPrefix,
                             float startY,
                             std::function<void(const std::shared_ptr<Player>&)> actionPerPlayer);
    std::string inGameError;

private:
    sf::RenderWindow window;
    sf::Font font;
    std::vector<Button> buttons;
    std::vector<Button> targetButtons;
    std::function<void(const std::shared_ptr<Player>&)> targetAction;

    TextBox* nameBox;
    Button* addPlayerBtn;
    Button* startGameBtn;
    Button* demoGameBtn;

    std::vector<std::string> tempNames;
    std::vector<std::string> tempRoles;
    std::string setupError;
    std::string actionMessage;
    std::string winnerMessage;
    bool showVictory = false;

    Game game;
    GUIState state = GUIState::Setup;

    void showTargetSelection(std::function<void(const std::shared_ptr<Player>&)> action, const std::vector<std::shared_ptr<Player>> targets, bool includeCurrentPlayer = false);
    std::string randomRole();
};

} // namespace coup

