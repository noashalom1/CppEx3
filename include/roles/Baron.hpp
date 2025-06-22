// Author: noapatito123@gmail.com
#pragma once

#include "Player.hpp"

namespace coup
{

    class Baron final : public Player
    {
    public:
        Baron(Game &game, const std::string &name);
        ~Baron() override;
        
        void invest();
        std::string role() const override;
    };

}
