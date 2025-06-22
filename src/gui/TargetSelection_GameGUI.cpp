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
 * @brief Displays a list of target players and sets the game state to TargetSelection.
 *
 * This function is used to allow the user to select a target for a role-specific or general action (e.g., arrest, sanction, peek).
 * It builds a list of buttons representing valid target players, executes the provided action when a target is selected,
 * and provides a back button to return to the InGame state.
 *
 * @param action The action to be executed on the selected player.
 * @param includeCurrentPlayer Whether to include the current player as a valid target.
 * @param targets A predefined list of candidate targets to filter and show.
 */
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
        // Filter the provided targets based on includeCurrentPlayer
        for (Player *p : targets)
        {
            if (!includeCurrentPlayer && p == current)
            {
                continue; // Skip current player if not allowed
            }
            finalTargets.push_back(p);
        }
    }
    else
    {
        inGameError = "No targets available."; // No valid targets found
        state = GUIState::InGame;
        return;
    }

    // Create a button for each valid target
    for (Player *p : finalTargets)
    {
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

