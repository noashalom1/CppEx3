#include "Game.hpp"
#include "Player.hpp"
#include "exceptions.hpp"
#include <algorithm>
#include <iostream>
#include "General.hpp"

using namespace std;

namespace coup {

    Game::Game() {}

    Game::~Game() {}

    const std::vector<Player*>& Game::get_players() const {
        return players_list;
    }

    void Game::add_player(Player* player) {
        if (players_list.size() >= 6) {
            throw MaxPlayersExceededException();
        }
        for (const auto& p : players_list) {
            if (p->get_name() == player->get_name()) {
                throw DuplicatePlayerNameException();
            }
        }
        players_list.push_back(player);
    }

    std::vector<std::pair<std::string, std::string>>& Game::get_coup_list()  {
        return coup_list;
    }

    void Game::add_to_coup(const std::string &attacker, const std::string &target) {
    coup_list.emplace_back(attacker, target);
    }

    string Game::turn() const {
        if (players_list.empty()) throw GameNotStartedException();
        return players_list[turn_index % players_list.size()]->get_name();
    }

    vector<string> Game::players() const {
        vector<string> names;
        for (auto p : players_list) {
            names.push_back(p->get_name());
        }
        return names;
    }

    string Game::winner() const {
        int alive_count = 0;
        string winner_name;
        for (auto p : players_list) {
            if (!p->is_eliminated()) {
                alive_count++;
                winner_name = p->get_name();
            }
        }
        if (alive_count == 0) {
            throw GameNotStartedException();
        } else if (alive_count > 1) {
            throw GameStillOngoingException();
        }
        return winner_name;
    }

    void Game::next_turn() {
        if (players_list.empty()) return;
        if(get_current_player()->is_extra_turn()) {
            // אם השחקן הנוכחי קיבל תור נוסף, הוא לא עובר לתור הבא
            get_current_player()->set_extra_turn(false);
            return;
        }
        if(get_current_player()->is_bribe_done()) {
            get_current_player()->set_bribe_done(false);
        }
        // advance to next living player
        do {
            turn_index = (turn_index + 1) % players_list.size();
        } while (players_list[turn_index]->is_eliminated());

        Player* current = players_list[turn_index];
        coup_list.erase(std::remove_if(coup_list.begin(), coup_list.end(),
        [this](const std::pair<std::string, std::string>& entry) {
            return entry.first == turn();
        }), coup_list.end());
        current->start_new_turn();
    }


    void Game::check_force_coup(Player* current_player) {
        // מבוטל: הלוגיקה מועברת לGUI והשחקן חופשי לבחור מטרה
        if (current_player->get_coins() >= 10) {
            current_player->set_must_coup(true);
        } else {
            current_player->set_must_coup(false);
        }
    }

    void Game::remove_player(Player* player) {
        player->mark_eliminated();
        if (turn_index >= players_list.size()) turn_index = 0;
    }

    Player* Game::get_player(const string& name) {
        for (auto p : players_list) {
            if (p->get_name() == name) return p;
        }
        throw PlayerNotFoundException(name);
    }

    Player* Game::get_current_player() {
        if (players_list.empty()) throw NoPlayersLeft();
        return players_list[turn_index];
    }

}
