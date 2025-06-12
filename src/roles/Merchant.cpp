#include "Merchant.hpp"
#include "Game.hpp"
#include "exceptions.hpp"

namespace coup {

    Merchant::Merchant(Game& game, const std::string& name) : Player(game, name) {}

     void Merchant::start_new_turn() {
        Player::start_new_turn();  // קריאה לפונקציית הבסיס

        if (coins >= 3) {
            coins += 1; // ✅ בונוס אוטומטי אם יש לפחות 3 מטבעות
        }
    }

    std::string Merchant::role() const {
        return "Merchant";
    }

}
