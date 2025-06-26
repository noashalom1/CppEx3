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
#include <memory>

using namespace coup;
using namespace sf;

/**
 * @brief Adds buttons for actions that are specific to players of a certain role.
 *
 * Each button is labeled with the action and the player's name, and invokes a given action.
 * The function automatically filters out eliminated players, and supports error handling.
 *
 * @param role The role name to match (e.g., "Judge", "Spy").
 * @param buttonPrefix Prefix to display on each button (e.g., "Undo Coup").
 * @param startY The starting Y-position for placing buttons.
 * @param actionPerPlayer The function to call when a button is clicked.
 * @return int The number of buttons added.
 */
int GameGUI::addRoleActionButtons(const std::string &role,
                                  const std::string &buttonPrefix,
                                  float startY,
                                  std::function<void(const std::shared_ptr<Player> &)> actionPerPlayer)
{
    const std::vector<std::shared_ptr<Player>> &all_players = game.get_all_players();
    std::vector<int> role_players_idx_vec;
    int i = 0;
    for (const auto &p : all_players)
    {
        if (p->role() == role)
        {
            role_players_idx_vec.push_back(i);
        }
        i++;
    }

    if (role_players_idx_vec.empty())
    {
        return 0; // Don't create buttons at all
    }

    float y = startY;
    int count = 0;
    for (int idx : role_players_idx_vec)
    {
        const std::shared_ptr<Player> &p = all_players[idx];

        std::string label = buttonPrefix + " (" + p->get_name() + ")";
        Button btn(label, font, sf::Vector2f(190, 35), sf::Vector2f(800, y));
        y += 40;
        count++;

        // Button action includes validation (e.g., eliminated player) and then executes action
        btn.setAction([this, p, actionPerPlayer]()
                      {
            try {
                if (p->is_eliminated()) {
                    bool was_couped = false;
                    for (const auto& pair : game.get_coup_list()) {
                        if (pair.second == p->get_name()) {
                            was_couped = true;
                            break;
                        }
                    }

                    if (p->role() != "General" || !was_couped) {
                        inGameError = p->get_name() + " is eliminated.";
                        return;
                    }
                }

                actionPerPlayer(p);
            } catch (const std::exception& e) {
                inGameError = e.what();
                actionMessage.clear();
            } });

        buttons.push_back(btn);
    }

    return count;
}

/**
 * @brief Sets up all action buttons for the current player, based on their role.
 *
 * This includes general actions (gather, tax, bribe, arrest, sanction, coup),
 * as well as role-specific special actions (e.g., Invest, Undo Tax, Undo Coup, etc.).
 *
 * The function also adds a "New Game" button that resets the game state.
 */
void GameGUI::setupButtons()
{
    std::shared_ptr<Player> &p = game.get_current_player();
    std::string role = p->role();

    // Player action: Gather
    Button gatherBtn("Gather", font, sf::Vector2f(150, 40), sf::Vector2f(50, 100));
    gatherBtn.setAction([this]()
                        {
        std::shared_ptr<Player> p = game.get_current_player();
        p->gather();
        actionMessage = p->get_name() + " performed Gather. Coins: " + std::to_string(p->get_coins());
        inGameError.clear(); });
    buttons.push_back(gatherBtn);

    // Player action: Tax
    Button taxBtn("Tax", font, sf::Vector2f(150, 40), sf::Vector2f(50, 160));
    taxBtn.setAction([this]()
                     {
        std::shared_ptr<Player> p = game.get_current_player();
        p->tax();
        actionMessage = p->get_name() + " performed Tax. Coins: " + std::to_string(p->get_coins());
        inGameError.clear(); });
    buttons.push_back(taxBtn);

    // Player action: Bribe
    Button bribeBtn("Bribe", font, sf::Vector2f(150, 40), sf::Vector2f(50, 220));
    bribeBtn.setAction([this]()
                       {
        std::shared_ptr<Player> p = game.get_current_player();
        p->bribe();
        actionMessage = p->get_name() + " performed Bribe. Coins: " + std::to_string(p->get_coins());
        inGameError.clear(); });
    buttons.push_back(bribeBtn);

    // Player action: Arrest
    Button arrestBtn("Arrest", font, sf::Vector2f(150, 40), sf::Vector2f(50, 280));
    arrestBtn.setAction([this]()
                        {
    std::vector<std::shared_ptr<Player>> activePlayers;
    for (const auto &p : game.get_all_players())
    {
        if (!p->is_eliminated() && p != game.get_current_player())
        {
            activePlayers.push_back(p);
        }
    }

    if (activePlayers.empty())
    {
        inGameError = "No living players to arrest.";
        return;
    } 
    showTargetSelection([this](const std::shared_ptr<Player>& target)
                                              {
        try {
            std::shared_ptr<Player> p = game.get_current_player();
            p->arrest(target);  
            actionMessage = p->get_name() + " arrested " + target->get_name() +
                            "! " + p->get_name() + " has " + std::to_string(p->get_coins()) + " coins.";
            inGameError.clear();
        } catch (const std::exception& e) {
            inGameError = e.what();     // Catch the error from arrest
            actionMessage.clear();
        } }, activePlayers); });

    buttons.push_back(arrestBtn);

    // Player action: Sanction
    Button sanctionBtn("Sanction", font, sf::Vector2f(150, 40), sf::Vector2f(50, 340));
    sanctionBtn.setAction([this]()
                          {
        std::vector<std::shared_ptr<Player>> activePlayers;
        for (const auto &p : game.get_all_players())
        {
            if (!p->is_eliminated() && p != game.get_current_player())
            {
                activePlayers.push_back(p);
            }
        }

        if (activePlayers.empty())
        {
            inGameError = "No living players to sanction.";
            return;
        }
        showTargetSelection([this](const std::shared_ptr<Player> &target) {
         try {                                    
            std::shared_ptr<Player> p = game.get_current_player();
            p->sanction(target);
            actionMessage = p->get_name() + " sanctioned " + target->get_name() + "! " + p->get_name() + " has " + std::to_string(p->get_coins()) + " coins.";
            inGameError.clear();
        } catch (const std::exception& e) {
            inGameError = e.what();     // Catch the error from arrest
            actionMessage.clear(); }}, activePlayers); });
    buttons.push_back(sanctionBtn);

    // Player action: Coup
    Button coupBtn("Coup", font, sf::Vector2f(150, 40), sf::Vector2f(50, 400));
    coupBtn.setAction([this]()
                      {
    std::vector<std::shared_ptr<Player>> aliveTargets;
    for (const auto &p : game.get_all_players())
    {
        if (!p->is_eliminated() && p != game.get_current_player())
        {
            aliveTargets.push_back(p);
        }
    }

    if (aliveTargets.empty())
    {
        inGameError = "No living players to coup.";
        return;
    }

    showTargetSelection(
        [this](const std::shared_ptr<Player>& target)
        {
            std::shared_ptr<Player> p = game.get_current_player();
            try {
                p->coup(target);
                actionMessage = p->get_name() + " couped " + target->get_name();
                inGameError.clear();
            } catch (const std::exception& e) {
                inGameError = e.what();
                actionMessage.clear();
            }
        },
        aliveTargets); });
    buttons.push_back(coupBtn);

    // Role-specific action: invest for Baron
    if (role == "Baron")
    {
        Button investBtn("Invest", font, sf::Vector2f(150, 40), sf::Vector2f(50, 460));
        investBtn.setAction([this]()
                            {
            std::shared_ptr<Player> p = game.get_current_player();
            if (p->must_coup()) throw MustPerformCoupException();
            static_cast<Baron*>(p.get())->invest();
            actionMessage = p->get_name() + " Invested " + "! " + p->get_name() + " has " + std::to_string(p->get_coins()) + " coins.";
            inGameError.clear(); });
        buttons.push_back(investBtn);
    }

    // Special actions - top right corner
    float y = 50;

    // Governor: Undo Tax
    y += 40 * addRoleActionButtons("Governor", "Undo Tax", y, [this](const std::shared_ptr<Player> &p)
                                   {
    try {
        std::string msg = static_cast<Governor*>(p.get())->undo_tax();  
        actionMessage = msg;
        inGameError.clear();
    } catch (const std::exception& e) {
        inGameError = e.what();
        actionMessage.clear();
    } });

    // Judge: Undo Bribe
    y += 40 * addRoleActionButtons("Judge", "Undo Bribe", y, [this](const std::shared_ptr<Player> &p)
                                   {
    try {
        std::shared_ptr<Player> current = game.get_current_player();
        std::string msg = static_cast<Judge*>(p.get())->undo_bribe(current);
        actionMessage = msg;
        inGameError.clear();
    } catch (const std::exception& e) {
        inGameError = e.what();
        actionMessage.clear();
    } });

    // Spy: Peek and Disable
    y += 40 * addRoleActionButtons("Spy", "Peek and Disable", y, [this](const std::shared_ptr<Player> &p)
                                   {
    // Filtering targets: only living players and not the player themselves
    std::vector<std::shared_ptr<Player>> filteredTargets;
    for (const auto & target : game.get_all_players()) {
        if (!target->is_eliminated() && target != p) {
            filteredTargets.push_back(target);
        }
    }

    // no valid targets available
    if (filteredTargets.empty()) {
        inGameError = "No valid targets available.";
        return;
    }

    showTargetSelection(
        [this, p](const std::shared_ptr<Player>& target)
        {
            try
            {
                Spy *spy = static_cast<Spy *>(p.get());
                std::string result = spy->peek_and_disable(target);
                actionMessage = result;
                inGameError.clear();
            }
            catch (const std::exception &e)
            {
                inGameError = e.what();
                actionMessage.clear();
            }
        },
        filteredTargets, // Don't include the current player
        true); });

    // General: Undo Coup
    y += 40 * addRoleActionButtons("General", "Undo Coup", y, [this](const std::shared_ptr<Player> &p)
                                   {
    // Get the list of victims from the game itself
    std::vector<std::shared_ptr<Player>> targets;
    for (const auto& entry : game.get_coup_list()) {
        try {
            std::shared_ptr<Player>& target = game.get_player(entry.second);
            if(!target->is_eliminated()) continue;
            targets.push_back(target);
        } catch (...) {}
    }

    showTargetSelection(
        [this, p](const std::shared_ptr<Player>& target) {
            try {
                std::string result = static_cast<General*>(p.get())->undo_coup(target);
                actionMessage = result;
                inGameError.clear();
            } catch (const std::exception& ex) {
                inGameError = ex.what();  // Show error only
            }
        },
        targets
    ); });

    Button newGameBtn("New Game", font, sf::Vector2f(150, 40), sf::Vector2f(800, 650));
    newGameBtn.setAction([this]()
                         {
                             game = Game(); // Reset the game state
                             tempNames.clear(); // Clear temporary names
                             tempRoles.clear(); // Clear temporary roles
                             buttons.clear(); // Clear buttons
                             actionMessage.clear(); // Clear action messages
                             setupError.clear(); // Clear setup error if exists
                             inGameError.clear(); // Clear error messages
                             targetButtons.clear(); // Clear target buttons if they exist
                             state = GUIState::Setup; // Return to setup screen
                         });
    buttons.push_back(newGameBtn);
}