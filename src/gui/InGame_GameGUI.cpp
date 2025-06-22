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
 * @brief Adds role-specific action buttons for each player with a given role.
 *
 * For each player with the specified role, creates a button labeled with a prefix and the player's name.
 * Each button, when clicked, executes the given action on the specific player.
 *
 * @param role The role name (e.g., "Governor", "Spy").
 * @param buttonPrefix The text prefix to use for each button label.
 * @param startY The starting Y position for placing buttons.
 * @param actionPerPlayer A lambda function to perform the action on a player.
 * @return int The number of buttons created.
 */
int GameGUI::addRoleActionButtons(const std::string &role,
                                  const std::string &buttonPrefix,
                                  float startY,
                                  std::function<void(Player *)> actionPerPlayer)
{
    std::vector<Player *> rolePlayers;
    for (Player *p : game.get_players())
    {
        if (p->role() == role)
        {
            rolePlayers.push_back(p);
        }
    }

    if (rolePlayers.empty())
    {
        return 0; // Don't create buttons at all
    }

    float y = startY;
    int count = 0;
    for (Player *p : rolePlayers)
    {
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
 * @brief Sets up the GUI buttons for the current player's available actions.
 *
 * Adds general action buttons (gather, tax, bribe, arrest, sanction, coup),
 * and role-specific or special buttons depending on the player's role.
 */
void GameGUI::setupButtons()
{
    Player *p = game.get_current_player();
    std::string role = p->role();

    // Player action: Gather
    Button gatherBtn("Gather", font, sf::Vector2f(150, 40), sf::Vector2f(50, 100));
    gatherBtn.setAction([this]()
                        {
        Player* p = game.get_current_player();
        p->gather();
        actionMessage = p->get_name() + " performed Gather. Coins: " + std::to_string(p->get_coins());
        inGameError.clear(); });
    buttons.push_back(gatherBtn);

    // Player action: Tax
    Button taxBtn("Tax", font, sf::Vector2f(150, 40), sf::Vector2f(50, 160));
    taxBtn.setAction([this]()
                     {
        Player* p = game.get_current_player();
        p->tax();
        actionMessage = p->get_name() + " performed Tax. Coins: " + std::to_string(p->get_coins());
        inGameError.clear(); });
    buttons.push_back(taxBtn);

    // Player action: Bribe
    Button bribeBtn("Bribe", font, sf::Vector2f(150, 40), sf::Vector2f(50, 220));
    bribeBtn.setAction([this]()
                       {
        Player* p = game.get_current_player();
        p->bribe();
        actionMessage = p->get_name() + " performed Bribe. Coins: " + std::to_string(p->get_coins());
        inGameError.clear(); });
    buttons.push_back(bribeBtn);

    // Player action: Arrest
    Button arrestBtn("Arrest", font, sf::Vector2f(150, 40), sf::Vector2f(50, 280));
    arrestBtn.setAction([this]()
                        { showTargetSelection([this](Player *target)
                                              {
        try {
            Player* p = game.get_current_player();
            p->arrest(*target);  
            actionMessage = p->get_name() + " arrested " + target->get_name() +
                            "! " + p->get_name() + " has " + std::to_string(p->get_coins()) + " coins.";
            inGameError.clear();
        } catch (const std::exception& e) {
            inGameError = e.what();     // Catch the error from arrest
            actionMessage.clear();
        } }, false, game.get_players()); });

    buttons.push_back(arrestBtn);

    // Player action: Sanction
    Button sanctionBtn("Sanction", font, sf::Vector2f(150, 40), sf::Vector2f(50, 340));
    sanctionBtn.setAction([this]()
                          { showTargetSelection([this](Player *target)
                                                {
            Player* p = game.get_current_player();
            p->sanction(*target);
            actionMessage = p->get_name() + " sanctioned " + target->get_name() + "! " + p->get_name() + " has " + std::to_string(p->get_coins()) + " coins.";
            inGameError.clear(); }, false, game.get_players()); });
    buttons.push_back(sanctionBtn);

    // Player action: Coup
    Button coupBtn("Coup", font, sf::Vector2f(150, 40), sf::Vector2f(50, 400));
    coupBtn.setAction([this]()
                      {
    std::vector<Player*> aliveTargets;
    for (Player* p : game.get_players())
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
        [this](Player* target)
        {
            Player* p = game.get_current_player();
            try {
                p->coup(*target);
                actionMessage = p->get_name() + " couped " + target->get_name();
                inGameError.clear();
            } catch (const std::exception& e) {
                inGameError = e.what();
                actionMessage.clear();
            }
        },
        false,
        aliveTargets); });
    buttons.push_back(coupBtn);

    // Role-specific action: invest for Baron
    if (role == "Baron")
    {
        Button investBtn("Invest", font, sf::Vector2f(150, 40), sf::Vector2f(50, 460));
        investBtn.setAction([this]()
                            {
            Player* p = game.get_current_player();
            if (p->must_coup()) throw MustPerformCoupException();
            static_cast<Baron*>(p)->invest();
            actionMessage = p->get_name() + " Invested " + "! " + p->get_name() + " has " + std::to_string(p->get_coins()) + " coins.";
            inGameError.clear(); });
        buttons.push_back(investBtn);
    }

    // Special actions - top right corner
    float y = 50;

    // Governor: Undo Tax
    y += 40 * addRoleActionButtons("Governor", "Undo Tax", y, [this](Player *p)
                                   {
    try {
        std::string msg = static_cast<Governor*>(p)->undo_tax();  
        actionMessage = msg;
        inGameError.clear();
    } catch (const std::exception& e) {
        inGameError = e.what();
        actionMessage.clear();
    } });

    // Judge: Undo Bribe
    y += 40 * addRoleActionButtons("Judge", "Undo Bribe", y, [this](Player *p)
                                   {
    try {
        Player* current = game.get_current_player();
        std::string msg = static_cast<Judge*>(p)->undo_bribe(*current);
        actionMessage = msg;
        inGameError.clear();
    } catch (const std::exception& e) {
        inGameError = e.what();
        actionMessage.clear();
    } });

    // Spy: Peek and Disable
    y += 40 * addRoleActionButtons("Spy", "Peek and Disable", y, [this](Player *p)
                                   {
    // Filtering targets: only living players and not the player themselves
    std::vector<Player*> filteredTargets;
    for (Player* target : game.get_players()) {
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
        [this, p](Player *target)
        {
            try
            {
                Spy *spy = static_cast<Spy *>(p);
                std::string result = spy->peek_and_disable(*target);
                actionMessage = result;
                inGameError.clear();
            }
            catch (const std::exception &e)
            {
                inGameError = e.what();
                actionMessage.clear();
            }
        },
        true, // Don't include the current player
        filteredTargets); });

    // General: Undo Coup
    y += 40 * addRoleActionButtons("General", "Undo Coup", y, [this](Player *p)
                                   {
    // Get the list of victims from the game itself
    std::vector<Player*> targets;
    for (const auto& entry : game.get_coup_list()) {
        std::cout << "  Attacker: " << entry.first << ", Target: " << entry.second << std::endl;
      
        try {
            Player* target = game.get_player(entry.second);
            if(!target->is_eliminated()) continue;
            targets.push_back(target);
        } catch (...) {}
    }

    showTargetSelection(
        [this, p](Player* target) {
            try {
                std::string result = static_cast<General*>(p)->undo_coup(*target);
                actionMessage = result;
                inGameError.clear();
            } catch (const std::exception& ex) {
                inGameError = ex.what();  // Show error only
            }
        },
        false,
        targets
    ); });

    Button newGameBtn("New Game", font, sf::Vector2f(150, 40), sf::Vector2f(800, 650));
    newGameBtn.setAction([this]()
                         {
                             game = Game();           // Reset the game state
                             tempNames.clear();       // Clear temporary names
                             tempRoles.clear();       // Clear temporary roles
                             buttons.clear();         // Clear buttons
                             actionMessage.clear();   // Clear action messages
                             inGameError.clear();     // Clear error messages
                             targetButtons.clear();   // Clear target buttons if they exist
                             state = GUIState::Setup; // Return to setup screen
                         });
    buttons.push_back(newGameBtn);
}