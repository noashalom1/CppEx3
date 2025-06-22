// Author: noapatito123@gmail.com
#pragma once

#include "Player.hpp"

namespace coup {

    class Merchant final : public Player {
    public:
        Merchant(Game& game, const std::string& name);
        ~Merchant() override;

        void start_new_turn() override;
        std::string role() const override;

        
    };

}


