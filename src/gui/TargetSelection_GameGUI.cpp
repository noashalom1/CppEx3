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

/**
 * @brief Displays the target selection screen.
 *
 * This method presents buttons for choosing a player from the provided list of targets.
 * If no targets are available, it returns to the game state with an error message.
 *
 * Each button corresponds to a valid target and triggers the provided action upon selection.
 * Also includes a "Back" button to cancel the selection and return to the main game screen.
 *
 * @param action The function to call when a target is selected.
 * @param targets The list of potential target players.
 * @param includeCurrentPlayer Whether the current player is allowed as a target.
 */
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