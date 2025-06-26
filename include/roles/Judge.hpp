// Author: noapatito123@gmail.com
#pragma once

#include "Player.hpp" 

namespace coup
{
    // Judge is a final role derived from Player
    class Judge final : public Player
    {
    private:
        bool used_undo_this_round = false; // Flag to track if undo_bribe was used this round

    public:
        Judge(Game &game, const std::string &name); // Constructor
        ~Judge() override; // Destructor

        bool can_undo_bribe() const { return !used_undo_this_round; } // Check if undo_bribe is available
        void mark_undo_bribe_used() { used_undo_this_round = true; } // Mark undo_bribe as used
        void reset_undo_bribe_flag() { used_undo_this_round = false; } // Reset the flag for a new round
        std::string undo_bribe(const std::shared_ptr<Player>& target); // Undo a bribe on a target player
        
        std::string role() const override; // Return role name
    };
}
