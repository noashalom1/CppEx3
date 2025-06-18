#include "General.hpp"
#include "Game.hpp"
#include "exceptions.hpp"
#include <algorithm>

namespace coup
{

    General::General(Game &game, const std::string &name) : Player(game, name) {}

    std::string General::undo_coup(Player &attacker, Player &target)
    {
        if (!can_undo_coup())
        {
            throw GameException(name + " already used UNDO COUP this round.");
        }

        if (coins < 5)
        {
            throw NotEnoughCoinsException(5, coins);
        }

        if (!target.is_eliminated())
        {
            throw TargetIsAlreadyEliminated();
        }

        // ודא שהשחקן באמת הודח ע"י מישהו
        bool found = false;
        for (const auto &entry : game.get_coup_list())
        {
            if (entry.second == target.get_name())
            {
                // 🔴 הוספת התנאי: אל תאפשר לבטל אם אתה זה שביצע את ה-COUP
                if (entry.first == name)
                {
                    throw GameException("You cannot undo your own coup.");
                }
                found = true;
                break;
            }
        }

        if (!found)
        {
            throw GameException("Target was not couped.");
        }

        coins -= 5;
        target.revive();

        game.get_coup_list().erase(
            std::remove_if(game.get_coup_list().begin(), game.get_coup_list().end(),
                           [&target](const std::pair<std::string, std::string> &entry)
                           {
                               return entry.second == target.get_name();
                           }),
            game.get_coup_list().end());

        attacker.set_must_coup(false);
        attacker.set_last_action("undo_coup");
        mark_undo_coup_used();

        return name + " undid coup on " + target.get_name();
    }

    std::string General::role() const
    {
        return "General";
    }

}
