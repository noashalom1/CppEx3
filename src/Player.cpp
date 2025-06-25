// Author: noapatito123@gmail.com
#include "Player.hpp"
#include "exceptions.hpp"
#include "Game.hpp"
#include <memory>
#include <iostream>

namespace coup
{

    /**
     * @brief Constructs a new Player object with default status and 0 coins.
     */
    Player::Player(Game &game, const std::string &name)
        : eliminated(false),
          disable_to_arrest(false),
          game(game),
          name(name),
          coins(0),
          mustPerformCoup(false),
          sanctioned(false),
          extra_turns(0),
          sanctioned_by(""),
          disable_arrest_turns(0),
          used_bribe(false)
    {
    }

    /**
     * @brief Virtual destructor for Player.
     */
    Player::~Player() {}

    /**
     * @brief Performs the gather action (gain 1 coin).
     * @throws MustPerformCoupException if player must coup.
     * @throws SanctionedException if player is sanctioned.
     */
    void Player::gather()
    {
        check_turn();
        if (must_coup())
            throw MustPerformCoupException();
        if (is_sanctioned() == true)
            throw SanctionedException();
        coins++;
        std::cout << name << " preformed gather! \n"
                  << std::endl;
        game.next_turn();
    }

    /**
     * @brief Performs the tax action (gain 2 coins and record the action).
     * @throws MustPerformCoupException if player must coup.
     * @throws SanctionedException if player is sanctioned.
     */
    void Player::tax()
    {
        check_turn();
        if (must_coup())
            throw MustPerformCoupException();
        if (is_sanctioned() == true)
            throw SanctionedException();
        coins += 2;
        game.get_action_history().emplace_back(name, "tax", game.get_current_round());
        game.get_tax_turns()[name] = game.get_global_turn_index(); // Track tax turn
        std::cout << name << " preformed tax! \n"
                  << std::endl;
        game.next_turn();
    }

    /**
     * @brief Performs the bribe action (pay 4 coins to gain 2 extra turns).
     * @throws MustPerformCoupException if player must coup.
     * @throws NotEnoughCoinsException if player has fewer than 4 coins.
     */
    void Player::bribe()
    {
        check_turn();
        if (must_coup())
            throw MustPerformCoupException();
        if (coins < 4)
            throw NotEnoughCoinsException(4, coins);
        coins -= 4;        // Pay 4 coins
        extra_turns = 2;   // Gain 2 extra turns
        mark_used_bribe(); // Set bribe used flag
        std::cout << name << " preformed bribe! \n"
                  << std::endl;
        game.next_turn();
    }

    /**
     * @brief Arrests the target player and adjusts coins based on role.
     * @throws Multiple exceptions for invalid arrest conditions.
     */
    void Player::arrest(const std::shared_ptr<Player> &target)
    {
        check_turn();
        if (must_coup())
            throw MustPerformCoupException();
        if (is_disable_to_arrest())
            throw ArrestBlockedException();
        if (target->is_eliminated())
            throw TargetIsEliminatedException();
        if (target->get_name() == name)
            throw CannotTargetYourselfException();
        if (target->get_name() == game.get_last_arrested_name())
            throw DuplicateArrestException();

        if (target->role() == "General")
        {
            if (target->get_coins() <= 0)
                throw TargetNoCoinsException();
            coins++; // Steal 1 coin from General
        }
        else if (target->role() == "Merchant")
        {
            if (target->get_coins() <= 1)
                throw TargetNoCoinsException();
            target->coins -= 2; // Merchant loses 2 coins
        }
        else
        {
            if (target->get_coins() <= 0)
                throw TargetNoCoinsException();
            target->coins--; // Target loses 1 coin
            coins++;         // Attacker gains 1 coin
        }
        game.set_last_arrested_name(target->get_name()); // Save last arrested
        std::cout << name << " preformed arrest on " << target->get_name() << "! \n"
                  << std::endl;
        game.next_turn();
    }

    /**
     * @brief Sanctions another player, optionally rewarding Barons.
     * @throws Multiple exceptions for invalid target or insufficient coins.
     */
    void Player::sanction(const std::shared_ptr<Player> &target)
    {
        check_turn();
        if (must_coup())
            throw MustPerformCoupException();
        if (target->is_eliminated())
            throw TargetIsEliminatedException();
        if (target->get_name() == name)
            throw CannotTargetYourselfException();
        if (target->is_sanctioned() == true)
            throw AlreadySanctionedException();
        if (target->role() == "Baron")
            target->increase_coins(1); // Baron gets 1 coin back
        if ((target->role() == "Judge" && coins < 4) || coins < 3)
            throw NotEnoughCoinsException(target->role() == "Judge" ? 4 : 3, coins);
        target->role() == "Judge" ? coins -= 4 : coins -= 3; // Judge costs 4, others cost 3
        target->mark_sanctioned(name);                       // Apply sanction
        std::cout << name << " preformed sanction on " << target->get_name() << "! \n"
                  << std::endl;
        game.next_turn();
    }

    /**
     * @brief Performs a coup on another player, eliminating them.
     * @throws Multiple exceptions for invalid target or insufficient coins.
     */
    void Player::coup(const std::shared_ptr<Player> &target)
    {
        check_turn();
        if (coins < 7)
            throw NotEnoughCoinsException(7, coins);
        if (target->is_eliminated())
            throw TargetIsAlreadyEliminatedException();
        if (target->get_name() == name)
            throw CannotTargetYourselfException();
        game.remove_player(target->get_name());     // Eliminate player
        game.add_to_coup(name, target->get_name()); // Log coup
        coins -= 7;                                 // Pay for coup
        std::cout << name << " preformed coup on " << target->get_name() << "! \n"
                  << std::endl;
        game.next_turn();
    }

    /**
     * @brief Verifies that it's this player's turn.
     * @throws NotYourTurnException if it's not their turn.
     */
    void Player::check_turn() const
    {
        if (game.turn() != name)
            throw NotYourTurnException();
    }

    /**
     * @brief Revives the player if previously eliminated.
     * @throws TargetNotEliminatedException if player is still active.
     */
    void Player::revive()
    {
        if (!is_eliminated())
            throw TargetNotEliminatedException();
        eliminated = false;
    }

    /**
     * @brief Decreases player's coin count.
     * @param amount Number of coins to remove.
     * @throws NotEnoughCoinsException if not enough coins available.
     */
    void Player::decrease_coins(int amount)
    {
        if (coins < amount)
        {
            throw NotEnoughCoinsException(amount, coins);
        }
        coins -= amount;
    }

    /**
     * @brief Increases player's coin count.
     * @param amount Number of coins to add.
     */
    void Player::increase_coins(int amount)
    {
        coins += amount;
    }

    /**
     * @brief Marks the player as sanctioned by another player.
     * @param by_whom Name of the player who sanctioned.
     */
    void Player::mark_sanctioned(const std::string &by_whom)
    {
        sanctioned = true;
        sanctioned_by = by_whom;
    }

    /**
     * @brief Clears any active sanctions on the player.
     */
    void Player::clear_sanctioned()
    {
        sanctioned = false;
        sanctioned_by.clear(); // Reset source of sanction
    }

}