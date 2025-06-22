// Author: noapatito123@gmail.com
#include "Governor.hpp"
#include "exceptions.hpp"
#include "Game.hpp"
#include <iostream>

namespace coup
{

    /**
     * @brief Constructor for the Governor role.
     * @param game Reference to the Game instance.
     * @param name Player's name.
     */
    Governor::Governor(Game &game, const std::string &name) : Player(game, name) {}
    
    /**
     * @brief Destructor for the Governor class.
     */
    Governor::~Governor() = default;

    /**
     * @brief Performs the tax action (gain 3 coins and logs it).
     * @throws MustPerformCoupException if player must coup.
     * @throws SanctionedException if player is sanctioned.
     */
    void Governor::tax()
    {
        check_turn();
        if (must_coup())
            throw MustPerformCoupException();
        if (is_sanctioned())
            throw SanctionedException();

        coins += 3;
        game.get_action_history().emplace_back(name, "tax", game.get_current_round());
        game.get_tax_turns()[name] = game.get_global_turn_index(); // עדכון סבב המס האחרון
        game.next_turn();
    }

    /**
     * @brief Checks if Governor is allowed to perform undo_tax this round.
     * @throws PlayerEliminatedException if player is eliminated.
     * @throws ActionAlreadyUsedThisRoundException if already used this round.
     */
    void Governor::can_undo_tax() const
    {
        if (is_eliminated())
        {
            throw PlayerEliminatedException(name);
        }
        if (used_undo_this_round)
        {
            throw ActionAlreadyUsedThisRoundException(name, "UNDO TAX");
        }
    }

    /**
     * @brief Cancels the most recent valid tax action by another player.
     * 
     * Decreases that player's coins and removes the action from history.
     * 
     * @return std::string Message describing the undo.
     * 
     * @throws PlayerEliminatedException if Governor is eliminated.
     * @throws CannotUndoOwnActionException if trying to undo own action.
     * @throws ActionTooOldException if the tax is from too many turns ago.
     * @throws TargetIsEliminatedException if the taxed player is eliminated.
     * @throws NoRecentActionToUndoException if no tax action found to cancel.
     */
    std::string Governor::undo_tax()
    {
        if (is_eliminated())
        {
            throw PlayerEliminatedException(name);
        }

        can_undo_tax(); // Check conditions
        auto &history = game.get_action_history();
        int global_turn = game.get_global_turn_index();
        int players_count = game.get_active_players_count();

         // Iterate in reverse to find the most recent tax
        for (auto it = history.rbegin(); it != history.rend(); ++it)
        {
            const std::string &actor = std::get<0>(*it);
            const std::string &action = std::get<1>(*it);
            int round = game.get_tax_turns()[actor];
            // Match only recent tax actions by other players
            if (action == "tax")
            {
                if ((global_turn - round) > (players_count - 1))
                {
                    throw ActionTooOldException(actor, "tax");
                }

                if (name == actor)
                {
                    throw CannotUndoOwnActionException(name, "tax");
                }
                Player *target = game.get_player(actor);
                if (target->is_eliminated())
                {
                    throw TargetIsEliminatedException();
                }

                int amount = (target->role() == "Governor") ? 3 : 2; // Governor taxed 3, others 2
                target->decrease_coins(amount); // Remove coins from target
                std::string message = name + " canceled " + target->get_name() + "'s tax. " +
                                      std::to_string(amount) + " coins were removed.";

                history.erase(std::next(it).base()); // Remove action from history
                mark_undo_tax_used(); // Mark as used this round

                return message;
            }
        }

        throw NoRecentActionToUndoException("tax"); // No valid tax found
    }

    /**
     * @brief Returns the role name ("Governor").
     * @return std::string The role name.
     */
    std::string Governor::role() const
    {
        return "Governor";
    }

}
