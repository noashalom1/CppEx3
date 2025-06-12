#include "General.hpp"
#include "Game.hpp"
#include "exceptions.hpp"
#include <algorithm>

namespace coup {

    General::General(Game& game, const std::string& name) : Player(game, name) {}

    void General::undo_coup(Player& attacker, Player& target) {
        if (coins < 5) throw NotEnoughCoinsException(5, coins);
        if (!target.is_eliminated()) throw TargetIsAlreadyEliminated();
        coins -= 5;
        
       target.revive();
       game.get_coup_list().erase(
            std::remove_if(game.get_coup_list().begin(), game.get_coup_list().end(),
                           [&target](const std::pair<std::string, std::string>& entry) {
                               return entry.second == target.get_name();
                           }),
            game.get_coup_list().end()
        );
        
        attacker.set_must_coup(false);
        attacker.clear_disable_to_arrest();
        attacker.set_last_action("undo_coup");
    }

    std::string General::role() const {
        return "General";
    }

}
