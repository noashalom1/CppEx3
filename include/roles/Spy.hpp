#ifndef SPY_HPP
#define SPY_HPP

#include "Player.hpp"

namespace coup {

    class Spy : public Player {
    private:
        bool used_peek_and_disable = false;
    public:
        Spy(Game& game, const std::string& name);

        std::string peek_and_disable(Player& target);

        bool can_peek_and_disable() const { return !used_peek_and_disable; }
        void mark_peek_and_disable_used() { used_peek_and_disable = true; }
        void reset_peek_and_disable_flag() { used_peek_and_disable = false; }

        std::string role() const override;
    };

}

#endif
