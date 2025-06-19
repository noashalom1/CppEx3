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

    Game::Game() {}

    Game::~Game() {}

    const std::vector<Player *> &Game::get_players() const // גם לדאוג להוריד איפה שיש
    {
        return players_list;
    }
    void Game::add_player(Player *player)
    {
        if (players_list.size() >= 6)
        {
            throw MaxPlayersExceededException();
        }
        for (const auto &p : players_list)
        {
            if (p->get_name() == player->get_name())
            {
                throw DuplicatePlayerNameException();
            }
        }
        players_list.push_back(player);
    }
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

    std::vector<std::pair<std::string, std::string>> &Game::get_coup_list()
    {
        return coup_list;
    }

    std::map<std::string, int> &Game::get_tax_turns()
    {
        return tax_turns;
    }

    void Game::add_to_coup(const std::string &attacker, const std::string &target)
    {
        coup_list.emplace_back(attacker, target);
    }

    string Game::turn() const
    {
        if (players_list.empty())
            throw GameNotStartedException();
        return players_list[turn_index % players_list.size()]->get_name();
    }

    vector<string> Game::players() const
    {
        vector<string> names;
        for (auto p : players_list)
        {
            names.push_back(p->get_name());
        }
        return names;
    }

    string Game::winner() const
    {
        int alive_count = 0;
        string winner_name;
        for (auto p : players_list)
        {
            if (!p->is_eliminated())
            {
                alive_count++;
                winner_name = p->get_name();
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

    void Game::next_turn()
    {
        if (players_list.empty())
            return;

        if (get_current_player()->is_extra_turn())
        {
            int turns = get_current_player()->get_extra_turns();
            turns--;
            get_current_player()->set_extra_turns(turns);
            return; // נשאר באותו תור
        }

        Player *prev_player = get_current_player();

        // advance to next living player
        do
        {
            turn_index = (turn_index + 1) % players_list.size();
        } while (players_list[turn_index]->is_eliminated());

        global_turn_index++;

        // ✅ התנאי הנכון לאיפוס FLAGS בתחילת סבב חדש
        if (global_turn_index % get_active_players_count() == 0)
        {
            current_round++;
            for (Player *p : players_list)
            {
                if (p->role() == "Judge")
                    static_cast<Judge *>(p)->reset_undo_bribe_flag();
                if (p->role() == "Governor")
                    static_cast<Governor *>(p)->reset_undo_tax_flag();
                if (p->role() == "General")
                    static_cast<General *>(p)->reset_undo_coup_flag();
                if (p->role() == "Spy")
                    static_cast<Spy *>(p)->reset_peek_and_disable_flag();
            }
        }

        Player *current = players_list[turn_index];

        // מנקה רשומות coup לפי תוקף
        coup_list.erase(std::remove_if(coup_list.begin(), coup_list.end(),
                                       [this](const std::pair<std::string, std::string> &entry)
                                       {
                                           return entry.first == turn();
                                       }),
                        coup_list.end());

        // הפחתת טיימרים של פעולות רגילות
        for (auto it = last_actions.begin(); it != last_actions.end();)
        {
            it->second.second--;
            if (it->second.second <= 0)
            {
                it = last_actions.erase(it);
            }
            else
            {
                ++it;
            }
        }

        current->set_last_action("");
        current->start_new_turn();

        // ניהול מצב של DISABLE TO ARREST
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

    void Game::check_force_coup(Player *current_player)
    {
        // מבוטל: הלוגיקה מועברת לGUI והשחקן חופשי לבחור מטרה
        if (current_player->get_coins() >= 10)
        {
            current_player->set_must_coup(true);
        }
        else
        {
            current_player->set_must_coup(false);
        }
    }

    void Game::remove_player(Player *player)
    {
        player->mark_eliminated();
        if (turn_index >= players_list.size())
            turn_index = 0;
    }

    Player *Game::get_player(const string &name)
    {
        for (auto p : players_list)
        {
            if (p->get_name() == name)
                return p;
        }
        throw PlayerNotFoundException(name);
    }

    Player *Game::get_current_player()
    {
        if (players_list.empty())
            throw NoPlayersLeft();
        return players_list[turn_index];
    }

    void Game::set_last_arrested_name(const std::string &name)
    {
        last_arrested_name = name;
    }

    const std::string &Game::get_last_arrested_name() const
    {
        return last_arrested_name;
    }
    void Game::set_last_tax_player_name(const std::string &name)
    {
        last_tax_player_name = name;
    }

    const std::string &Game::get_last_tax_player_name() const
    {
        return last_tax_player_name;
    }

}
