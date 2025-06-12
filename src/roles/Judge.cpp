#include "Judge.hpp"
#include "Game.hpp"
#include "exceptions.hpp"
#include <iostream>

namespace coup {

    Judge::Judge(Game& game, const std::string& name) : Player(game, name) {}

    void Judge::undo_bribe(Player& target) {
        if (target.is_eliminated()) throw TargetIsAlreadyEliminated();
        if (target.get_name() == name) throw GameException("You cannot undo_bribe yourself.");

        if (target.is_bribe_done() && target.is_extra_turn() == true) {
            target.set_extra_turn(false);
            std::cout << "Judge has canceled " << target.get_name() << "'s bribe." << std::endl;
        }
        else if (target.is_bribe_done() && target.is_extra_turn() == false) {
            std::cout << "Judge has canceled " << target.get_name() << "'s bribe." << std::endl;
            game.next_turn();
        } else{
            throw UndoNotAllowed();
        }
        
    }

    std::string Judge::role() const {
        return "Judge";
    }

}
