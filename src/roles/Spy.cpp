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

    void Spy::peek_and_arrest(Player &target)
    {
        if (target.is_eliminated())
        {
            throw GameException("Cannot arrest an eliminated player.");
        }
        if (target.get_name() == name)
            throw GameException("You cannot peek & arrest yourself.");

        std::cout << "[SPY] " << get_name() << " peeked at " << target.get_name()
                  << " who has " << target.get_coins() << " coins.\n";

        this->arrest(target);
        set_last_action("peek_and_arrest");
    }

}
