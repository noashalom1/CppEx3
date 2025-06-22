// Author: noapatito123@gmail.com
#include "Spy.hpp"
#include "Game.hpp"
#include "exceptions.hpp"
#include <iostream>

namespace coup
{

    Spy::Spy(Game &game, const std::string &name) : Player(game, name) {}
    Spy::~Spy() = default;

    std::string Spy::role() const
    {
        return "Spy";
    }

    std::string Spy::peek_and_disable(Player &target)
    {
        if (target.is_eliminated())
            throw TargetIsEliminatedException();

        if (target.get_name() == name)
            throw CannotTargetYourselfException();

        if (!can_peek_and_disable())
            throw ActionAlreadyUsedThisRoundException(name, "peek and disable");

        target.set_disable_to_arrest(true);
        target.set_disable_arrest_turns(1);

        mark_peek_and_disable_used();

        std::string result = this->get_name() + " peeked and disabled " + target.get_name() +
                             " (Coins: " + std::to_string(target.get_coins()) + ")";

        return result;
    }
}
