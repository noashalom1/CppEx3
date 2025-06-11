#include "Baron.hpp"
#include "Game.hpp"
#include "exceptions.hpp"

namespace coup {

    Baron::Baron(Game& game, const std::string& name) : Player(game, name) {}

    void Baron::invest() {
        if (coins < 3) throw NotEnoughCoinsException(3, coins);
        coins -= 3;
        coins += 6;
    }

    std::string Baron::role() const {
        return "Baron";
    }

}
