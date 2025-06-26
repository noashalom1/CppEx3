// Author: noapatito123@gmail.com
#pragma once

#include "Player.hpp" 
#include <memory>    

namespace coup {

    // Spy is a final role derived from Player
    class Spy final : public Player {
    private:
        bool used_peek_and_disable = false; // Flag to track if special action was used

    public:
        Spy(Game& game, const std::string& name); // Constructor
        ~Spy() override; // Destructor

        std::string peek_and_disable(const std::shared_ptr<Player>& target); // Special action: peek and disable
        bool can_peek_and_disable() const { return !used_peek_and_disable; } // Check if action is available
        void mark_peek_and_disable_used() { used_peek_and_disable = true; } // Mark action as used
        void reset_peek_and_disable_flag() { used_peek_and_disable = false; } // Reset flag for new round

        std::string role() const override; // Return role name
    };

}
