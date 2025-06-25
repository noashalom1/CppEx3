// Author: noapatito123@gmail.com
#include "Game.hpp"
#include "Player.hpp"
#include "exceptions.hpp"
#include <algorithm>
#include <iostream>
#include "General.hpp"
#include "Judge.hpp"
#include "Governor.hpp"
#include "Spy.hpp"

using namespace std;

namespace coup
{

    /**
     * @brief Constructs a new Game object with initial values.
     */
    Game::Game()
        : turn_index(0), global_turn_index(0), current_round(1) {}

    /**
     * @brief Destructor for the Game class.
     */
    Game::~Game() {}

    /**
     * @brief Returns a constant reference to the list of players.
     * @return const std::vector<std::shared_ptr<Player>>& List of players.
     */
    const std::vector<std::shared_ptr<Player>> &Game::get_all_players() const
    {
        return players_list;
    }

    /**
     * @brief Finds a player by name.
     * @param name The name of the player to search.
     * @return std::shared_ptr<Player> Pointer to the found player.
     * @throws PlayerNotFoundException if the player does not exist.
     */
    std::shared_ptr<Player> &Game::get_player(const string &name)
    {
        for (std::shared_ptr<Player> &player : players_list)
        {
            if (player->get_name() == name)
                return player;
        }
        throw PlayerNotFoundException(name);
    }

    /**
     * @brief Returns the current player (based on turn index).
     * @return std::shared_ptr<Player> Pointer to the current player.
     * @throws NoPlayersLeftException if no players are in the game.
     */
    std::shared_ptr<Player> &Game::get_current_player()
    {
        if (players_list.empty())
            throw NoPlayersLeftException();
        return players_list[turn_index];
    }

    /**
     * @brief Counts the number of players still in the game.
     * @return int Number of active (not eliminated) players.
     */
    int Game ::get_active_players_count() const
    {
        int count = 0;
        for (const auto &p : players_list)
        {
            if (!p->is_eliminated())
            {
                count++;
            }
        }
        return count;
    }

    /**
     * @brief Returns a reference to the map of tax turns.
     * @return std::map<std::string, int>& Tax turn mapping.
     */
    std::map<std::string, int> &Game::get_tax_turns()
    {
        return tax_turns;
    }

    /**
     * @brief Gets the name of the last arrested player.
     * @return const std::string& Name of the last arrested player.
     */
    const std::string &Game::get_last_arrested_name() const
    {
        return last_arrested_name;
    }

    /**
     * @brief Sets the name of the last arrested player.
     * @param name Name to set.
     */
    void Game::set_last_arrested_name(const std::string &name)
    {
        last_arrested_name = name;
    }

    /**
     * @brief Adds a new player to the game.
     * @param player Pointer to the player to add.
     * @throws MaxPlayersExceededException if more than 6 players.
     * @throws DuplicatePlayerNameException if name is already used.
     */
    void Game::add_player(const std::shared_ptr<Player> &player)
    {
        if (players_list.size() >= 6)
        {
            throw MaxPlayersExceededException(); // limit reached
        }
        for (const auto &p : players_list)
        {
            if (p->get_name() == player->get_name())
            {
                throw DuplicatePlayerNameException(); // name already taken
            }
        }
        players_list.push_back(std::move(player));
    }

    /**
     * @brief Eliminates a player from the game.
     * @param player Pointer to the player to eliminate.
     */
    void Game::remove_player(const std::string &target)
    {
        for (auto &p : players_list)
        {
            if (p->get_name() == target)
            {
                p->mark_eliminated();
                return;
            }
        }
        throw PlayerNotFoundException(target);
        if (turn_index >= players_list.size())
            turn_index = 0;
    }

    /**
     * @brief Returns the name of the player whose turn it is.
     * @return string Player name.
     * @throws GameNotStartedException if no players are in the game.
     */
    string Game::turn() const
    {
        if (players_list.empty())
            throw GameNotStartedException();
        return players_list[turn_index % players_list.size()]->get_name();
    }

    /**
     * @brief Adds an entry to the coup list (attacker, target).
     * @param attacker Name of the attacking player.
     * @param target Name of the target player.
     */
    void Game::add_to_coup(const std::string &attacker, const std::string &target)
    {
        coup_list.emplace_back(attacker, target);
    }

    /**
     * @brief Determines the winner of the game.
     * @return string Name of the winning player.
     * @throws GameNotStartedException if no players left.
     * @throws GameStillOngoingException if more than one player is active.
     */
    string Game::winner() const
    {
        int alive_count = 0;
        string winner_name;
        for (const std::shared_ptr<coup::Player> &p : players_list)
        {
            if (!p->is_eliminated())
            {
                alive_count++;
                winner_name = p->get_name(); // keep last found alive player
            }
        }
        if (alive_count == 0)
        {
            throw GameNotStartedException();
        }
        else if (alive_count > 1)
        {
            throw GameStillOngoingException();
        }
        return winner_name;
    }

    /**
     * @brief Advances the turn to the next active player.
     * Also resets round-based flags and handles special states.
     */
    void Game::next_turn()
    {
        if (players_list.empty())
            return;

        if (get_current_player()->is_extra_turn())
        {
            int turns = get_current_player()->get_extra_turns();
            turns--;
            get_current_player()->set_extra_turns(turns);
            return; // player gets another turn
        }

        // advance to next living player
        do
        {
            turn_index = (turn_index + 1) % players_list.size();
        } while (players_list[turn_index]->is_eliminated()); // skip non-active player

        global_turn_index++;

        // Find the smallest index of a living player
        size_t min_alive_index = -1;
        for (size_t i = 0; i < players_list.size(); ++i)
        {
            if (!players_list[i]->is_eliminated())
            {
                min_alive_index = i;
                break;
            }
        }

        // If current player is the first living one in order → new round: reset flags for role-based undo abilities
        if (turn_index == min_alive_index)
            {
                current_round++;
                for (std::shared_ptr<Player> &p : players_list)
                {
                    if (p->role() == "Judge")
                        static_cast<Judge *>(p.get())->reset_undo_bribe_flag();
                    if (p->role() == "Governor")
                        static_cast<Governor *>(p.get())->reset_undo_tax_flag();
                    if (p->role() == "General")
                        static_cast<General *>(p.get())->reset_undo_coup_flag();
                    if (p->role() == "Spy")
                        static_cast<Spy *>(p.get())->reset_peek_and_disable_flag();
                }
            }

        std::shared_ptr<Player> &current = players_list[turn_index];

        std::shared_ptr<Player> &prev_player = get_current_player();

        // remove coup records related to current turn player
        coup_list.erase(std::remove_if(coup_list.begin(), coup_list.end(),
                                       [this](const std::pair<std::string, std::string> &entry)
                                       {
                                           return entry.first == turn(); // clear only records of current player
                                       }),
                        coup_list.end());

        current->start_new_turn(); // reset internal states for the new turn

        // decrease "disable to arrest" counter for the previous player
        if (prev_player->get_disable_arrest_turns() > 0)
        {
            prev_player->set_disable_arrest_turns(prev_player->get_disable_arrest_turns() - 1);
            if (prev_player->get_disable_arrest_turns() == 0)
            {
                prev_player->set_disable_to_arrest(false);
                std::cout << prev_player->get_name() << " is no longer blocked from ARREST." << std::endl;
            }
        }
    }

}
