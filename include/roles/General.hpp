// Author: noapatito123@gmail.com
#pragma once

#include "Player.hpp" 

namespace coup
{
    // General is a final role derived from Player
    class General final : public Player
    {
    private:
        bool used_undo_this_round = false; // Flag to track if undo_coup was used this round

    public:
        General(Game &game, const std::string &name); // Constructor
        ~General() override; // Destructor

        bool can_undo_coup() const { return !used_undo_this_round; } // Check if undo_coup is available
        void mark_undo_coup_used() { used_undo_this_round = true; } // Mark undo_coup as used
        void reset_undo_coup_flag() { used_undo_this_round = false; } // Reset the flag for a new round
        std::string undo_coup(const std::shared_ptr<Player>& target); // Undo the last coup on target

        std::string role() const override; // Return role name
    };
}
