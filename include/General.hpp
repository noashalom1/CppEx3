#ifndef GENERAL_HPP
#define GENERAL_HPP

#include "Player.hpp"

namespace coup {

    class General : public Player {
    public:
        General(Game& game, const std::string& name);

        void undo_coup(Player& attacker, Player& target);
        std::string role() const override;
    };

}

#endif
