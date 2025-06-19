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
            throw ActionAlreadyUsedThisRoundException(name, "UNDO COUP");
        }

        if (coins < 5)
        {
            throw NotEnoughCoinsException(5, coins);
        }

        if (!target.is_eliminated())
        {
            throw GameException("Target is not eliminated.");
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

        mark_undo_coup_used();

        return name + " undid coup on " + target.get_name();
    }

    std::string General::role() const
    {
        return "General";
    }

}
