#include "Governor.hpp"
#include "exceptions.hpp"
#include "Game.hpp"
#include <iostream>

namespace coup
{

    Governor::Governor(Game &game, const std::string &name) : Player(game, name) {}

    void Governor::tax()
    {
        check_turn();
        if (must_coup())
            throw MustPerformCoupException();
        if (is_sanctioned())
        {
            throw SanctionedException();
        }
        coins += 3;
        game.get_action_history().emplace_back(name, "tax");
        game.next_turn();
    }

   void Governor::undo_tax() {
    auto& history = game.get_action_history();

    if (!can_undo_tax()) {
        throw GameException(name + " already used undo this round.");
    }

    for (auto it = history.rbegin(); it != history.rend(); ++it) {
        const std::string& targetName = it->first;
        const std::string& action = it->second;

        if (action == "tax") {
            if (targetName == name) {
                throw GameException("You cannot undo your own Tax.");
            }

            Player* target = game.get_player(targetName);
            if (target->is_eliminated()) throw TargetIsAlreadyEliminated();

            if (target->role() == "Governor") {
                target->decrease_coins(3);
            } else {
                target->decrease_coins(2);
            }

            
            history.erase(std::next(it).base());
            mark_undo_tax_used();
            std::cout << name << " canceled " << targetName << "'s tax." << std::endl;
            return;
        }
    }

    throw GameException("No recent tax by another player to undo.");
}

    std::string Governor::role() const
    {
        return "Governor";
    }

}
