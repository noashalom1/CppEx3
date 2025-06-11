#include "General.hpp"
#include "Game.hpp"
#include "exceptions.hpp"

namespace coup {

    General::General(Game& game, const std::string& name) : Player(game, name) {}

    void General::undo_coup(Player& attacker, Player& target) {
        if (coins < 5) throw NotEnoughCoinsException(5, coins);
        if (target.is_eliminated()) throw TargetIsAlreadyEliminated();
        if (target.get_name() == name) throw GameException("You cannot undo_coup yourself.");
        coins -= 5;
        game.cancel_coup(&attacker, &target);
    }

    std::string General::role() const {
        return "General";
    }

}
