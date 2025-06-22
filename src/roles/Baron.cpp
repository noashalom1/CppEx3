// Author: noapatito123@gmail.com
#include "Baron.hpp"
#include "Game.hpp"
#include "exceptions.hpp"

namespace coup
{

    Baron::Baron(Game &game, const std::string &name) : Player(game, name) {}

    void Baron::invest()
    {
        check_turn();
        if (must_coup())
            throw MustPerformCoupException();
        if (coins < 3)
            throw NotEnoughCoinsException(3, coins);
        coins += 3;
        game.next_turn();
    }

    std::string Baron::role() const
    {
        return "Baron";
    }

}
