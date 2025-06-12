#ifndef GAMEGUI_HPP
#define GAMEGUI_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <functional>
#include "Button.hpp"
#include "Game.hpp"
#include "TextBox.hpp"

namespace coup {

enum class GUIState {
    Setup,
    InGame,
    TargetSelection
};

class GameGUI {
public:
    enum class GUIState { Setup, InGame, TargetSelection };
    std::string inGameError;

private:
    sf::RenderWindow window;
    sf::Font font;
    std::vector<Button> buttons;
    std::function<void(Player*)> targetAction;
    std::vector<Button> targetButtons;

    Game game;

    GUIState state = GUIState::Setup; 
    void showTargetSelection(std::function<void(Player*)> action, bool includeCurrentPlayer = false, const std::vector<Player*>& targets = {});
    TextBox* nameBox;
    Button* addPlayerBtn;
    Button* startGameBtn;
    Button* demoGameBtn;
    std::vector<std::string> tempNames;
    std::vector<std::string> tempRoles;
    std::vector<std::string> allRoles = {"Governor", "Spy", "Baron", "General", "Judge", "Merchant"};
    std::string setupError;

public:
    GameGUI();
    void run();
    void setupButtons();
    void drawPlayerList();
};

} // namespace coup

#endif
