// Author: noapatito123@gmail.com
#pragma once

#include "Player.hpp"

namespace coup
{

    class Baron : public Player
    {
    public:
        Baron(Game &game, const std::string &name);
        void invest();
        std::string role() const override;
    };

}


