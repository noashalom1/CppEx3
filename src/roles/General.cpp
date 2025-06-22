// Author: noapatito123@gmail.com
#include "General.hpp"
#include "Game.hpp"
#include "exceptions.hpp"
#include <algorithm>

namespace coup
{

    /**
     * @brief Constructor for the General role.
     * @param game Reference to the game object.
     * @param name Name of the player.
     */
    General::General(Game &game, const std::string &name) : Player(game, name) {}
    
    /**
     * @brief Destructor for the General class.
     */
    General::~General() = default;

    /**
     * @brief Allows the General to undo a coup on a specific player.
     * 
     * Revives the eliminated player and removes the coup record, at the cost of 5 coins.
     * Can only be used once per round.
     * 
     * @param target The player to revive.
     * @return std::string Message indicating the action performed.
     * 
     * @throws ActionAlreadyUsedThisRoundException If General already used undo this round.
     * @throws NotEnoughCoinsException If General has fewer than 5 coins.
     * @throws TargetNotEliminatedException If the target is not eliminated.
     */
    std::string General::undo_coup(Player &target)
    {
        if (!can_undo_coup())
        {
            throw ActionAlreadyUsedThisRoundException(name, "UNDO COUP");
        }

        if (coins < 5)
        {
            throw NotEnoughCoinsException(5, coins);
        }

        if (!target.is_eliminated())
        {
            throw TargetNotEliminatedException();
        }

        coins -= 5; // Pay 5 coins
        target.revive(); // Revive the eliminated player

        // Remove the coup record targeting the revived player
        game.get_coup_list().erase(
            std::remove_if(game.get_coup_list().begin(), game.get_coup_list().end(),
                           [&target](const std::pair<std::string, std::string> &entry)
                           {
                               return entry.second == target.get_name(); // Match by target name
                           }),
            game.get_coup_list().end());

        mark_undo_coup_used(); // Mark ability as used this round

        return name + " undid coup on " + target.get_name();
    }

     /**
     * @brief Returns the role name as a string.
     * @return std::string The role name ("General").
     */
    std::string General::role() const
    {
        return "General";
    }

}
