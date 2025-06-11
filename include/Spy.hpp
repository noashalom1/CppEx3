#ifndef SPY_HPP
#define SPY_HPP

#include "Player.hpp"

namespace coup {

    class Spy : public Player {
    public:
        Spy(Game& game, const std::string& name);

        void spy_peek(Player& target);

        std::string role() const override;
    };

}

#endif
