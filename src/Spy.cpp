#include "Spy.hpp"
#include "Game.hpp"
#include "exceptions.hpp"
#include <iostream>

namespace coup {

    Spy::Spy(Game& game, const std::string& name) : Player(game, name) {}

    std::string Spy::role() const {
        return "Spy";
    }

    void Spy::spy_peek(Player& target) {
    std::cout << "[SPY] " << get_name() << " peeked at " << target.get_name()
              << " who has " << target.get_coins() << " coins.\n";

    if (target.is_eliminated()) {
        throw GameException("Cannot arrest an eliminated player.");
    }

   this->arrest(target);
}

}
