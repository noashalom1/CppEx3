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

using namespace coup;
using namespace sf;

void GameGUI::showTargetSelection(std::function<void(Player *)> action, bool includeCurrentPlayer, const std::vector<Player *> &targets)
{
    state = GUIState::TargetSelection;
    targetButtons.clear();
    targetAction = action;

    Player *current = game.get_current_player();
    float y = 150;

    std::vector<Player *> finalTargets;

    if (!targets.empty())
    {
        // סינון לפי includeCurrentPlayer רק אם current נמצא ברשימה
        for (Player *p : targets)
        {
            if (!includeCurrentPlayer && p == current)
            {
                continue; // דלג על current player
            }
            finalTargets.push_back(p);
        }
    }
    else
    {
        inGameError = "No targets available."; // fallback חסום
        state = GUIState::InGame;
        return;
    }

    for (Player *p : finalTargets)
    {
        Button btn(p->get_name(), font, {200, 40}, {400, y});
        btn.setAction([this, p]()
                      {
            try {
                targetAction(p);  
                state = GUIState::InGame;
                targetButtons.clear();
            } catch (const GameException& e) {
                inGameError = e.what();
                actionMessage.clear();
                targetButtons.clear();
                state = GUIState::InGame;
            } });
        targetButtons.push_back(btn);
        y += 50;
    }
    // כפתור BACK

    Button backBtn("Back", font, {150, 40}, {50, 650});
    backBtn.setAction([this]()
                      {
    targetButtons.clear();
    state = GUIState::InGame;
    inGameError.clear();
    actionMessage.clear(); });
    targetButtons.push_back(backBtn);
}

