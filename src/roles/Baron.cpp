// Author: noapatito123@gmail.com
#include "Baron.hpp"
#include "Game.hpp"
#include "exceptions.hpp"

namespace coup
{

     /**
     * @brief Constructor for the Baron role.
     * @param game Reference to the game object.
     * @param name Name of the player.
     */
    Baron::Baron(Game &game, const std::string &name) : Player(game, name) {}
    
    /**
     * @brief Destructor for the Baron class.
     */
    Baron::~Baron() = default;

    /**
     * @brief Performs the 'invest' action for Baron (gain 3 coins).
     * @throws MustPerformCoupException if Baron must perform a coup.
     * @throws NotEnoughCoinsException if Baron has fewer than 3 coins.
     */
    void Baron::invest()
    {
        check_turn(); // Ensure it's Baron's turn
        if (must_coup())
            throw MustPerformCoupException();
        if (coins < 3)
            throw NotEnoughCoinsException(3, coins);
        coins += 3; // Gain 3 coins
        game.next_turn(); // Advance to next player's turn
    }

    /**
     * @brief Returns the role name as a string.
     * @return std::string The role name ("Baron").
     */
    std::string Baron::role() const
    {
        return "Baron";
    }

}
