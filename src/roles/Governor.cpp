#include "Governor.hpp"
#include "exceptions.hpp"
#include "Game.hpp"
#include <iostream>

namespace coup
{

    Governor::Governor(Game &game, const std::string &name) : Player(game, name) {}

    void Governor::tax()
    {
        check_turn();
        if (must_coup())
            throw MustPerformCoupException();
        if (is_sanctioned())
        {
            throw SanctionedException();
        }
        coins += 3;
        game.get_action_history().emplace_back(name, "tax", game.get_current_round());
        game.get_tax_turns()[name] = game.get_global_turn_index(); // עדכון סבב המס האחרון
        game.next_turn();
    }

    void Governor::can_undo_tax() const
    {
        if (is_eliminated())
        {
            throw GameException(name + " is eliminated.");
        }
        if (used_undo_this_round)
        {
            throw GameException(name + " has already used undo this round.");
        }
               
    }

    std::string Governor::undo_tax()
    {
        if (is_eliminated())
        {
            throw GameException(name + " is eliminated.");
        }

        can_undo_tax();
        auto &history = game.get_action_history();
        int global_turn = game.get_global_turn_index();
        int players_count = game.get_active_players_count();

        for (auto it = history.rbegin(); it != history.rend(); ++it)
        {
            const std::string &actor = std::get<0>(*it);
            const std::string &action = std::get<1>(*it);
            int round = game.get_tax_turns()[actor];
            // נחפש רק tax, רק מסבב נוכחי, ורק לא של השחקן שמבצע את undo
            if (action == "tax")
            {
                if((global_turn - round) > (players_count-1)){
                    throw GameException(actor + "'s tax is too old.");
                }

                if(name == actor){
                    throw GameException(name + " cannot undo their own tax.");
                }
                Player *target = game.get_player(actor);
                if (target->is_eliminated())
                {
                    throw TargetIsAlreadyEliminated();
                }

                int amount = (target->role() == "Governor") ? 3 : 2;
                target->decrease_coins(amount);
                std::string message = name + " canceled " + target->get_name() + "'s tax. " +
                                      std::to_string(amount) + " coins were removed.";

                // מחיקת הפעולה מההיסטוריה
                history.erase(std::next(it).base());
                mark_undo_tax_used();

                return message;
            }
        }

        throw GameException("No recent tax by another player to undo.");
    }

    std::string Governor::role() const
    {
        return "Governor";
    }

}
