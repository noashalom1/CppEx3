#ifndef SPY_HPP
#define SPY_HPP

#include "Player.hpp"

namespace coup {

    class Spy : public Player {
    public:
        Spy(Game& game, const std::string& name);

        void peek_and_arrest(Player& target);

        std::string role() const override;
    };

}

#endif
