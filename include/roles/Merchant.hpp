// Author: noapatito123@gmail.com
#pragma once

#include "Player.hpp" 

namespace coup 
{
    // Merchant is a final role derived from Player
    class Merchant final : public Player {
    public:
        Merchant(Game& game, const std::string& name); // Constructor
        ~Merchant() override; // Destructor

        void start_new_turn() override; // Called at the start of each turn
        
        std::string role() const override; // Return role name
    };
}
