// Author: noapatito123@gmail.com
#include "Spy.hpp"
#include "Game.hpp"
#include "exceptions.hpp"
#include <iostream>

namespace coup
{

    /**
     * @brief Constructor for the Spy role.
     * @param game Reference to the Game instance.
     * @param name Player's name.
     */
    Spy::Spy(Game &game, const std::string &name) : Player(game, name) {}

    /**
     * @brief Destructor for the Spy class.
     */
    Spy::~Spy() = default;

    /**
     * @brief Performs the Spy's special action: peek and disable arrest.
     *
     * Allows the Spy to peek at another player's coin count and prevent them from being arrested for one turn.
     *
     * @param target The player to peek and disable.
     * @return std::string A message describing the result of the action.
     *
     * @throws TargetIsEliminatedException if the target is eliminated.
     * @throws CannotTargetYourselfException if the target is the Spy themself.
     * @throws ActionAlreadyUsedThisRoundException if Spy already used this action this round.
     */
    std::string Spy::peek_and_disable(const std::shared_ptr<Player>& target)
    {
        if (is_eliminated())
            throw PlayerEliminatedException(name); 

        if (target->get_name() == this->get_name())
            throw CannotTargetYourselfException(); 
        
        if (!can_peek_and_disable())
            throw ActionAlreadyUsedThisRoundException(name, "peek and disable");

        if (target->is_eliminated())
            throw TargetIsEliminatedException();

        target->set_disable_to_arrest(true); // Prevent arrest
        target->set_disable_arrest_turns(1); // Only for 1 turn

        mark_peek_and_disable_used(); // Mark action used for this round

        std::string result = this->get_name() + " peeked and disabled " + target->get_name() +
                             " (Coins: " + std::to_string(target->get_coins()) + ")";
        std::cout << name << " preformed peek and disable on " << target->get_name() << "! \n" << std::endl;       
        return result;
    }

    /**
     * @brief Returns the role name ("Spy").
     * @return std::string The role name.
     */
    std::string Spy::role() const
    {
        return "Spy";
    }
}
