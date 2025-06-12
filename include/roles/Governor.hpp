#ifndef GOVERNOR_HPP
#define GOVERNOR_HPP

#include "Player.hpp"

namespace coup {

    class Governor : public Player {
    private:
        bool used_undo_this_round = false; // Flag to track if undo has been used this round
    public:
        Governor(Game& game, const std::string& name);

        void tax();
        void undo_tax();
        std::string role() const override;
        bool can_undo_tax() const { return !used_undo_this_round; }
        void mark_undo_tax_used() { used_undo_this_round = true; }
        void reset_undo_tax_flag() { used_undo_this_round = false; }
    };
    
}

#endif
