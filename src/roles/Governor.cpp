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
        game.next_turn();
    }

    std::string Governor::undo_tax()
    {
        if (is_eliminated())
        {
            throw GameException(name + " is eliminated.");
        }

        if (!can_undo_tax())
        {
            throw GameException(name + " already used undo this round.");
        }
        
        auto &history = game.get_action_history();
        int currentRound = game.get_current_round();

        for (auto it = history.rbegin(); it != history.rend(); ++it)
        {
            const std::string &actor = std::get<0>(*it);
            const std::string &action = std::get<1>(*it);
            int round = std::get<2>(*it);

            // נחפש רק tax, רק מסבב נוכחי, ורק לא של השחקן שמבצע את undo
            if (action == "tax" && actor != name && round == currentRound)
            {
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
