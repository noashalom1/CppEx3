#ifndef GOVERNOR_HPP
#define GOVERNOR_HPP

#include "Player.hpp"

namespace coup {

    class Governor : public Player {
    public:
        Governor(Game& game, const std::string& name);

        void tax();
        void undo_tax(Player& target);
        std::string role() const override;
    };

}

#endif
