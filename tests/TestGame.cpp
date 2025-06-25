
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Game.hpp"
#include "Governor.hpp"
using namespace coup;

TEST_CASE("Game basic flow and player rotation") {
    Game game;
    auto p1 = std::make_shared<Governor>(game, "Alice");
    auto p2 = std::make_shared<Governor>(game, "Bob");

    CHECK_NOTHROW(game.add_player(p1));
    CHECK_NOTHROW(game.add_player(p2));

    CHECK(game.get_all_players().size() == 2);
    CHECK(game.turn() == "Alice");

    game.next_turn();
    CHECK(game.turn() == "Bob");

    game.next_turn();
    CHECK(game.turn() == "Alice");
}

TEST_CASE("Game player limit and exceptions") {
    Game game;
    for (int i = 1; i <= 6; ++i) {
        CHECK_NOTHROW(game.add_player(std::make_shared<Governor>(game, "P" + std::to_string(i))));
    }
    CHECK_THROWS(game.add_player(std::make_shared<Governor>(game, "P7")));
}
