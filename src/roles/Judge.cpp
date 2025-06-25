// Author: noapatito123@gmail.com
#include "Judge.hpp"
#include "Game.hpp"
#include "exceptions.hpp"
#include <iostream>

namespace coup
{

    /**
     * @brief Constructor for the Judge role.
     * @param game Reference to the Game instance.
     * @param name Name of the player.
     */
    Judge::Judge(Game &game, const std::string &name) : Player(game, name) {}

    /**
     * @brief Destructor for the Judge class.
     */
    Judge::~Judge() = default;

    /**
     * @brief Cancels a bribe action made by another player.
     *
     * Cancels the extra turn granted by a bribe, depending on when it's used.
     *
     * @param target The player whose bribe is to be undone.
     * @return std::string A message describing the undo.
     *
     * @throws PlayerEliminatedException If the Judge is eliminated.
     * @throws ActionAlreadyUsedThisRoundException If already used undo this round.
     * @throws UndoNotAllowedException If target didn't bribe.
     * @throws CannotUndoOwnActionException If trying to undo self.
     * @throws TargetIsEliminatedException If target is eliminated.
     * @throws InvalidBribeUndoException If target has more than one extra turn (invalid state).
     */
    std::string Judge::undo_bribe(const std::shared_ptr<Player>& target)
    {
        if (is_eliminated())
        {
            throw PlayerEliminatedException(name); // Judge is eliminated
        }

        if (!target->is_used_bribe())
        {
            throw UndoNotAllowedException(target->get_name(), " bribe"); // Target didn't bribe
        }

        if (target->get_name() == this->get_name())
        {
            throw CannotUndoOwnActionException(name, "bribe"); // Cannot undo own bribe
        }

        if (!can_undo_bribe())
        {
            throw ActionAlreadyUsedThisRoundException(name, "UNDO BRIBE"); // Already used this round
        }

        if (target->is_eliminated())
        {
            throw TargetIsEliminatedException(); // Can't undo eliminated player
        }

        std::string msg;

        if (target->get_extra_turns() == 1)
        {
            msg = name + " has canceled " + target->get_name() + "'s bribe.";
            target->set_extra_turns(0); // Remove extra turn
        }
        else if (target->get_extra_turns() == 0)
        {
            msg = name + " has canceled " + target->get_name() + "'s bribe (after it took effect).";
            game.next_turn(); // Force turn advance if bribe already took effect
        }
        else
        {
            throw InvalidBribeUndoException(); // Unexpected state
        }

        mark_undo_bribe_used(); // Prevent further undo this round
        std::cout << name << " preformed undo-bribe on " << target->get_name() << "! \n" << std::endl;
        return msg;
    }

    /**
     * @brief Returns the role name ("Judge").
     * @return std::string The role name.
     */
    std::string Judge::role() const
    {
        return "Judge";
    }
}