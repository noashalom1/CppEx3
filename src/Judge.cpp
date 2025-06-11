#include "Judge.hpp"
#include "Game.hpp"
#include "exceptions.hpp"
#include <iostream>

namespace coup {

    Judge::Judge(Game& game, const std::string& name) : Player(game, name) {}

    void Judge::undo_bribe(Player& target) {
        if (target.get_last_action() != "bribe") {
            throw UndoNotAllowed();
        }
        if (target.is_eliminated()) throw TargetIsAlreadyEliminated();
        if (target.get_name() == name) throw GameException("You cannot undo_bribr yourself.");

        std::cout << "Judge has canceled " << target.get_name() << "'s bribe. Turn is now over." << std::endl;

        game.next_turn(); // מונע ממנו לבצע פעולה נוספת
    }


    std::string Judge::role() const {
        return "Judge";
    }

}
