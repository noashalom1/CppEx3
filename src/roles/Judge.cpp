#include "Judge.hpp"
#include "Game.hpp"
#include "exceptions.hpp"
#include <iostream>

namespace coup
{

    Judge::Judge(Game &game, const std::string &name) : Player(game, name) {}

    std::string Judge::undo_bribe(Player &target)
    {
        if (is_eliminated())
        {
            throw GameException(name + " is eliminated.");
        }

        if (!can_undo_bribe())
        {
            throw GameException(name + " already used undo this round.");
        }

        if(!target.is_used_bribe())
        {
            throw GameException(target.get_name() + " has not used bribe.");
        }

        if (&target == this)
        {
            throw GameException("You can't undo your own bribe.");
        }

        if (target.is_eliminated())
        {
            throw TargetIsAlreadyEliminated();
        }

         std::string msg;

        if (target.get_extra_turns() == 1)
        {
            msg = name + " has canceled " + target.get_name() + "'s bribe.";
            target.set_extra_turns(0);
        }
        else if (target.get_extra_turns() == 0)
        {
            msg = name + " has canceled " + target.get_name() + "'s bribe (after it took effect).";
            game.next_turn();
        }
        else
        {
            throw GameException("Target has not done a bribe or has already undone it.");
        }

        mark_undo_bribe_used();
        return msg;
    }

    std::string Judge::role() const
    {
        return "Judge";
    }
}