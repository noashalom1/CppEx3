// Author: noapatito123@gmail.com
#pragma once

#include "Player.hpp"

namespace coup {

    class Governor final : public Player {
    private:
        bool used_undo_this_round = false; // Flag to track if undo has been used this round
    public:
        Governor(Game& game, const std::string& name);
        ~Governor() override;

        void tax();
        std::string undo_tax();
        void can_undo_tax() const;
        void mark_undo_tax_used() { used_undo_this_round = true; }
        void reset_undo_tax_flag() { used_undo_this_round = false; }
        
        std::string role() const override;
    };
    
}

