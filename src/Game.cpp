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

        // advance to next living player
        do {
            turn_index = (turn_index + 1) % players_list.size();
        } while (players_list[turn_index]->is_eliminated());

        Player* current = players_list[turn_index];
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

    bool Game::general_blocks_coup(Player* attacker, Player* target) {
        for (Player* p : players_list) {
            if (!p->is_eliminated() && p != attacker && p->role() == "General") {
                General* g = dynamic_cast<General*>(p);
                if (g && g->get_coins() >= 5) {
                    g->undo_coup(*attacker, *target);
                    return true;
                }
            }
        }
        return false;
    }

    void Game::cancel_coup(Player* attacker, Player* target) {
        if (target->is_eliminated()) {
            target->mark_eliminated(); // just to be sure
        }

        std::cout << "[Coup Cancelled] " << attacker->get_name()
                  << "'s coup against " << target->get_name()
                  << " was blocked by a General." << std::endl;
    }

}
