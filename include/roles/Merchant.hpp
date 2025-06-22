// Author: noapatito123@gmail.com
#pragma once

#include "Player.hpp"

namespace coup {

    class Merchant : public Player {
    public:
        Merchant(Game& game, const std::string& name);

        void start_new_turn() override;
        std::string role() const override;

        
    };

}


