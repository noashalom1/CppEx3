// Author: noapatito123@gmail.com
#pragma once

#include "Player.hpp" 

namespace coup
{
    // Baron is a final role derived from Player
    class Baron final : public Player
    {
    public:
        Baron(Game &game, const std::string &name); // Constructor
        ~Baron() override; // Destructor

        void invest(); // Special action for Baron

        std::string role() const override; // Returns the role name
    };
}
