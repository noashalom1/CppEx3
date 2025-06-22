// Author: noapatito123@gmail.com
#include "Merchant.hpp"
#include "Game.hpp"
#include "exceptions.hpp"

namespace coup {

    Merchant::Merchant(Game& game, const std::string& name) : Player(game, name) {}
    Merchant::~Merchant() = default;

     void Merchant::start_new_turn() {
        Player::start_new_turn();  

        if (coins >= 3) {
            coins += 1; 
        }
    }

    std::string Merchant::role() const {
        return "Merchant";
    }

}
