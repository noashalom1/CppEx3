#include "Spy.hpp"
#include "Game.hpp"
#include "exceptions.hpp"
#include <iostream>

namespace coup
{

    Spy::Spy(Game &game, const std::string &name) : Player(game, name) {}

    std::string Spy::role() const
    {
        return "Spy";
    }

    std::string Spy::peek_and_disable(Player &target)
    {
        if (target.is_eliminated())
            throw GameException("Cannot peek and disable an eliminated player.");

        if (target.get_name() == name)
            throw GameException("You cannot peek and disable yourself.");

        if (!can_peek_and_disable())
            throw GameException(name + " already used peek and disable this round.");

        target.set_disable_to_arrest(true);
        target.set_disable_arrest_turns(1);
        set_last_action("peek_and_disable");

        mark_peek_and_disable_used();

        std::string result = this->get_name() + " peeked and disabled " + target.get_name() +
                             " (Coins: " + std::to_string(target.get_coins()) + ")";

        return result;
    }
}
