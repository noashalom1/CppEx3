#include "Governor.hpp"
#include "exceptions.hpp"
#include "Game.hpp"
#include <iostream>

namespace coup {

    Governor::Governor(Game& game, const std::string& name) : Player(game, name) {}

    void Governor::tax() {
        if (game.turn() != name) throw NotYourTurnException();
        if (is_sanctioned()) {
        throw SanctionedException();
        }
        coins += 3;
        game.next_turn();
    }

    void Governor::undo_tax(Player& target) {
        if (target.get_last_action() != "tax") {
            throw UndoNotAllowedException();
        }
        if (target.is_eliminated()) throw TargetIsAlreadyEliminated();
        if (target.get_name() == name) throw GameException("You cannot undo_tax yourself.");

        target.decrease_coins(2);
        std::cout << "Governor canceled " << target.get_name() << "'s tax. 2 coins were removed." << std::endl;

    }

    std::string Governor::role() const {
        return "Governor";
    }

}
