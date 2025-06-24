// Author: noapatito123@gmail.com
#pragma once

#include <vector>
#include <string>
#include <map>
#include <memory>

namespace coup
{

    class Player;

    class Game
    {
    private:
        std::vector<std::shared_ptr<Player>> players_list; // List of all players
        size_t turn_index; // Index of current turn
        size_t global_turn_index; // Total turn counter

        int current_round; // Current round number

        std::map<std::string, int> tax_turns; // Tracks tax turns per player

        std::vector<std::pair<std::string, std::string>> coup_list; // List of coup actions (attacker, target)
        std::string last_arrested_name; // Last arrested player's name
        std::string last_tax_player_name; // Last player who performed tax
        std::vector<std::tuple<std::string, std::string, int>> action_history; // Log of actions (player, action, round)

    public:
        Game(); // Constructor

        virtual ~Game(); // Destructor

        std::vector<std::shared_ptr<Player>> &get_players(); // Get all players
        std::shared_ptr<Player>& get_player(const std::string &name); // Get player by name
        std::shared_ptr<Player>& get_current_player(); // Get current turn player

        int get_active_players_count() const; // Count active (non-eliminated) players
        int get_current_round() const { return current_round; } // Get current round number

        size_t get_global_turn_index() const { return global_turn_index; } // Get global turn index

        std::vector<std::tuple<std::string, std::string, int>> &get_action_history() { return action_history; } // Get action history
        std::vector<std::pair<std::string, std::string>> &get_coup_list() { return coup_list; } // Get list of coup actions
        std::map<std::string, int> &get_tax_turns(); // Get tax turn map

        const std::string &get_last_arrested_name() const; // Get last arrested name
        void set_last_arrested_name(const std::string &name); // Set last arrested name

        void add_player(std::shared_ptr<Player> player); // Add a new player to the game
        void remove_player(std::shared_ptr<Player>& player); // Eliminate a player from the game

        std::string turn() const; // Get the name of the player whose turn it is

        void add_to_coup(const std::string &attacker, const std::string &target); // Add a coup record

        std::string winner() const; // Get the winner of the game

        void next_turn(); // Advance to the next turn
    };

}
