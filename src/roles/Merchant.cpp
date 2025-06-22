// Author: noapatito123@gmail.com
#include "Merchant.hpp"
#include "Game.hpp"
#include "exceptions.hpp"

namespace coup {

    /**
     * @brief Constructor for the Merchant role.
     * @param game Reference to the Game instance.
     * @param name Player's name.
     */
    Merchant::Merchant(Game& game, const std::string& name) : Player(game, name) {}

    /**
     * @brief Destructor for the Merchant class.
     */
    Merchant::~Merchant() = default;

    /**
     * @brief Overrides the start of turn behavior for Merchant.
     * 
     * If the Merchant has at least 3 coins at the start of their turn, they gain 1 bonus coin.
     */
    void Merchant::start_new_turn() {
        Player::start_new_turn(); // Call base class logic

        if (coins >= 3) {
            coins += 1; // Bonus coin if Merchant has at least 3
        }
    }

    /**
     * @brief Returns the role name ("Merchant").
     * @return std::string The role name.
     */
    std::string Merchant::role() const {
        return "Merchant";
    }

}
