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

using namespace coup;
using namespace sf;

void GameGUI::showTargetSelection(std::function<void(const std::shared_ptr<Player>&)> action, const std::vector<std::shared_ptr<Player>> targets, bool includeCurrentPlayer)
{
    state = GUIState::TargetSelection;
    targetButtons.clear();
    targetAction = action;

    std::shared_ptr<Player>& current = game.get_current_player();
    float y = 150;

    std::vector<int> finalTargetsIdx;

    if (!targets.empty())
    {
        int idx = 0;
        // Filter the provided targets based on includeCurrentPlayer
        for (const auto& p : targets)
        {
            if (includeCurrentPlayer || p != current)
            {
                finalTargetsIdx.push_back(idx);
            }
            idx++;
        }
    }
    else
    {
        inGameError = "No targets available."; // No valid targets found
        state = GUIState::InGame;
        return;
    }

    // Create a button for each valid target
    for(int i : finalTargetsIdx) {
        const std::shared_ptr<Player>& p = targets[i];
        Button btn(p->get_name(), font, {200, 40}, {400, y});
        btn.setAction([this, p]()
                      {
            try {
                targetAction(p); // Execute the action on the selected player
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

    // BACK button to return to game without choosing a target
    Button backBtn("Back", font, {150, 40}, {50, 650});
    backBtn.setAction([this]()
                      {
    targetButtons.clear();
    state = GUIState::InGame;
    inGameError.clear();
    actionMessage.clear(); });
    targetButtons.push_back(backBtn);
}