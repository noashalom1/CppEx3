// Author: noapatito123@gmail.com
#pragma once

#include "Player.hpp"

namespace coup
{

    // Governor is a final role derived from Player
    class Governor final : public Player {
    private:
        bool used_undo_this_round = false; // Flag to track if undo has been used this round

    public:
        Governor(Game& game, const std::string& name); // Constructor
        ~Governor() override; // Destructor

        void tax(); // Special tax action
        std::string undo_tax(); // Undo the last tax action
        void mark_undo_tax_used() { used_undo_this_round = true; } // Mark undo as used
        void reset_undo_tax_flag() { used_undo_this_round = false; } // Reset undo flag
        
        std::string role() const override; // Return role name
    };
}
