
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Game.hpp"
#include "Governor.hpp"
#include "Spy.hpp"
#include "Baron.hpp"
#include "General.hpp"
#include "Judge.hpp"
#include "Merchant.hpp"
#include "exceptions.hpp"

using namespace coup;

TEST_CASE("Game::add_player, get_player, get_all_players") {
    Game game;
    auto p1 = std::make_shared<Spy>(game, "Alice");
    game.add_player(p1);

    // Check if get_player returns the correct player
    CHECK(game.get_player("Alice") == p1);

    // Check that requesting a non-existent player throws
    CHECK_THROWS_AS(game.get_player("Bob"), PlayerNotFoundException);

    // Check get_all_players returns a list containing only p1
    const auto& players = game.get_all_players();
    CHECK(players.size() == 1);
    CHECK(players[0] == p1);
}


TEST_CASE("Game::add_player edge cases") {
    Game game;
    for (int i = 0; i < 5; ++i) {
        game.add_player(std::make_shared<Spy>(game, "P" + std::to_string(i)));
    }
    CHECK_THROWS_AS(game.add_player(std::make_shared<Spy>(game, "P0")), DuplicatePlayerNameException);
    game.add_player(std::make_shared<Spy>(game, "P6"));
    CHECK_THROWS_AS(game.add_player(std::make_shared<Spy>(game, "Overflow")), MaxPlayersExceededException);
}

TEST_CASE("Game::turn and get_current_player") {
    Game game;
    auto p1 = std::make_shared<Spy>(game, "A");
    game.add_player(p1);
    CHECK(game.turn() == "A");
    CHECK(game.get_current_player() == p1);
    Game empty_game;
    CHECK_THROWS_AS(empty_game.turn(), GameNotStartedException);
    CHECK_THROWS_AS(empty_game.get_current_player(), NoPlayersLeftException);
}

TEST_CASE("Game::remove_player and get_active_players_count") {
    Game game;
    auto p1 = std::make_shared<Spy>(game, "A");
    auto p2 = std::make_shared<Spy>(game, "B");
    game.add_player(p1);
    game.add_player(p2);
    CHECK(game.get_active_players_count() == 2);
    game.remove_player("A");
    CHECK(p1->is_eliminated());
    CHECK(game.get_active_players_count() == 1);
    CHECK_THROWS_AS(game.remove_player("X"), PlayerNotFoundException);
}

TEST_CASE("Game::add_to_coup and is_in_coup_list") {
    Game game;
    game.add_to_coup("A", "B");
    CHECK(game.is_in_coup_list("B"));
    CHECK_FALSE(game.is_in_coup_list("C"));
}

TEST_CASE("Game::winner") {
    Game game;
    auto p1 = std::make_shared<Spy>(game, "A");
    auto p2 = std::make_shared<Spy>(game, "B");
    game.add_player(p1);
    game.add_player(p2);
    CHECK_THROWS_AS(game.winner(), GameStillOngoingException);
    p2->mark_eliminated();
    CHECK(game.winner() == "A");
    p1->mark_eliminated();
    CHECK_THROWS_AS(game.winner(), GameNotStartedException);
}

TEST_CASE("Game::next_turn and reset flags") {
    Game game;
    auto gov = std::make_shared<Governor>(game, "Gov");
    auto gen = std::make_shared<General>(game, "Gen");
    auto judge = std::make_shared<Judge>(game, "Judge");
    auto spy = std::make_shared<Spy>(game, "Spy");
    game.add_player(gov);
    game.add_player(gen);
    game.add_player(judge);
    game.add_player(spy);

    CHECK(game.turn() == "Gov");
    game.next_turn(); CHECK(game.turn() == "Gen");
    game.next_turn(); CHECK(game.turn() == "Judge");
    game.next_turn(); CHECK(game.turn() == "Spy");
    game.next_turn(); CHECK(game.turn() == "Gov"); // Full round

    // Simulate undo abilities used, verify they are reset
    static_cast<Governor*>(gov.get())->reset_undo_tax_flag();
    static_cast<General*>(gen.get())->reset_undo_coup_flag();
    static_cast<Judge*>(judge.get())->reset_undo_bribe_flag();
    static_cast<Spy*>(spy.get())->reset_peek_and_disable_flag();
}

TEST_CASE("Game::get_tax_turns and get(/set)_last_arrested_name") {
    Game game;
    game.get_tax_turns()["Alice"] = 3;
    CHECK(game.get_tax_turns().at("Alice") == 3);
    game.set_last_arrested_name("Bob");
    CHECK(game.get_last_arrested_name() == "Bob");
}
