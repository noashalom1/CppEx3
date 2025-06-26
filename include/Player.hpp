// Author: noapatito123@gmail.com
#pragma once

#include <string>
#include <set>
#include <map>
#include "Game.hpp"
#include <memory>

namespace coup
{

    class Game;

    class Player
    {
    private:
        bool eliminated; // Is the player eliminated from the game
        bool disable_to_arrest; // Is the player protected from arrest

    protected:
        Game &game; // Reference to the game instance
        std::string name; // Player name
        int coins; // Number of coins the player has
        bool mustPerformCoup; // Flag indicating whether the player must perform a coup
        bool sanctioned; // Is the player sanctioned
        int extra_turns; // Number of extra turns
        std::string sanctioned_by; // Name of the player who sanctioned this player
        int disable_arrest_turns; // Number of turns arrest is disabled
        bool used_bribe; // Flag for whether the player used bribe last turn
       

    public:
        Player(Game &game, const std::string &name); // Constructor
        virtual ~Player(); // Destructor
        virtual std::string role() const = 0; // Get role name (pure virtual)

        virtual void gather(); // Gain 1 coin
        virtual void tax();  // Gain 2 coins
        virtual void bribe();  // Bribe for extra turn
        virtual void arrest(const std::shared_ptr<Player> &target); // Arrest another player
        virtual void sanction(const std::shared_ptr<Player> &target); // Sanction another player
        virtual void coup(const std::shared_ptr<Player> &target); // Eliminate another player

        const std::string &get_name() const { return name; } // Get player name
        void check_turn() const; // Check if it's this player's turn
        void revive(); // Revive player (used by General)
        int get_coins() const { return coins; } // Get coin count
        void decrease_coins(int amount); // Reduce coin count
        void increase_coins(int amount); // Increase coin count

        void mark_eliminated() { eliminated = true; } // Mark player as eliminated
        bool is_eliminated() const { return eliminated; } // Is player eliminated

        void mark_sanctioned(const std::string &by_whom); // Sanction the player
        void clear_sanctioned(); // Remove sanction
        bool is_sanctioned() const { return sanctioned; } // Is player currently sanctioned

        void set_disable_to_arrest(bool value) { disable_to_arrest = value; } // Set arrest protection
        bool is_disable_to_arrest() const { return disable_to_arrest; } // Is arrest protection active
        void set_disable_arrest_turns(int n) { disable_arrest_turns = n; } // Set turns for arrest protection
        int get_disable_arrest_turns() const { return disable_arrest_turns; } // Get remaining turns of arrest protection

        void set_must_coup(bool value) { mustPerformCoup = value; } // Force player to coup
        bool must_coup() const { return mustPerformCoup; } // Check if player must coup

        bool is_used_bribe() const { return used_bribe; } // Did player use bribe
        void mark_used_bribe() { used_bribe = true; } // Mark bribe used
        void reset_used_bribe() { used_bribe = false; } // Reset bribe usage

        bool is_extra_turn() const { return extra_turns > 0; } // Does player have extra turns
        void set_extra_turns(int value) { extra_turns = value; } // Set number of extra turns
        int get_extra_turns() const { return extra_turns; } // Get number of extra turns

        virtual void start_new_turn() // Start of new turn
        {
            mustPerformCoup = (coins >= 10); // Automatically enforce COUP if player has 10+ coins

            // Clear sanctions the player applied to others
            for (const std::shared_ptr<Player> &player : game.get_all_players())
            {
                if (player->is_eliminated())
                    continue;
                if (player->is_sanctioned() && player->sanctioned_by == name)
                {
                    player->clear_sanctioned();
                }
            }
            reset_used_bribe(); // Reset bribe status for new turn
        }
    };

}
