
#include "doctest.h"
#include "Game.hpp"
#include "Player.hpp"
#include "Governor.hpp"
#include "Spy.hpp"
#include "Baron.hpp"
#include "General.hpp"
#include "Judge.hpp"
#include "Merchant.hpp"
#include "exceptions.hpp"

using namespace coup;

void force_turn1(Game &game, const std::string &name)
{
    for (int i = 0; i < 20; i++)
    {
        if (game.turn() == name)
            return;
        game.next_turn();
    }
}

TEST_CASE("Player actions: gather and tax functionality")
{
    Game game;
    auto p1 = std::make_shared<Spy>(game, "Alice");
    auto p2 = std::make_shared<Governor>(game, "Bob");

    game.add_player(p1);
    game.add_player(p2);

    CHECK(p1->get_coins() == 0);
    p1->gather();
    CHECK(p1->get_coins() == 1);

    p2->gather();
    CHECK(p2->get_coins() == 1);

    p1->tax();
    CHECK(p1->get_coins() == 3); // +2 from tax
}

TEST_CASE("Player actions: gather and tax edge cases")
{
    Game game;
    auto player = std::make_shared<Baron>(game, "Player");
    auto other = std::make_shared<Spy>(game, "Other");
    game.add_player(player);
    game.add_player(other);

    force_turn1(game, "Other");
    CHECK_THROWS_AS(player->tax(), NotYourTurnException);
    CHECK_THROWS_AS(player->gather(), NotYourTurnException);

    force_turn1(game, "Player");
    player->set_must_coup(true);
    CHECK_THROWS_AS(player->tax(), MustPerformCoupException);
    CHECK_THROWS_AS(player->gather(), MustPerformCoupException);
    player->set_must_coup(false);

    force_turn1(game, "Player");
    player->mark_sanctioned("Other");
    CHECK_THROWS_AS(player->tax(), SanctionedException);
    CHECK_THROWS_AS(player->gather(), SanctionedException);
}

TEST_CASE("Player::bribe functionality")
{
    Game game;
    auto p1 = std::make_shared<Spy>(game, "Alice");
    auto p2 = std::make_shared<Spy>(game, "Bob");

    game.add_player(p1);
    game.add_player(p2);

    // Give enough coins to perform bribe
    p1->increase_coins(4);
    CHECK(p1->get_coins() == 4);

    // Player performs bribe
    p1->bribe();

    // Bribe should cost 4 coins and give 2 extra turns
    CHECK(p1->get_coins() == 0);
    // Note: Although bribe() sets extra_turns to 2,
    // it immediately calls game.next_turn(), which consumes 1 turn.
    // Therefore, get_extra_turns() returns 1 instead of 2 right after bribe().
    CHECK(p1->get_extra_turns() == 1);
    CHECK(p1->is_used_bribe() == true);
}

TEST_CASE("Player::bribe edge cases")
{
    Game game;
    auto p1 = std::make_shared<Spy>(game, "Alice");
    auto p2 = std::make_shared<Spy>(game, "Bob");

    game.add_player(p1);
    game.add_player(p2);

    force_turn1(game, "Bob");
    CHECK_THROWS_AS(p1->bribe(), NotYourTurnException);

    force_turn1(game, "Alice");
    p1->set_must_coup(true);
    CHECK_THROWS_AS(p1->bribe(), MustPerformCoupException);
    p1->set_must_coup(false);

    // Not enough coins to bribe
    p1->increase_coins(3); // only 3 coins
    CHECK_THROWS_AS(p1->bribe(), NotEnoughCoinsException);
}

TEST_CASE("Player::arrest functionality")
{
    Game game;
    auto spy = std::make_shared<Spy>(game, "Spy");
    auto gen = std::make_shared<General>(game, "General");
    auto mer = std::make_shared<Merchant>(game, "Merchant");

    game.add_player(spy);
    game.add_player(gen);
    game.add_player(mer);

    spy->increase_coins(1);
    gen->increase_coins(1);
    mer->increase_coins(2);

    force_turn1(game, "General");
    gen->arrest(spy);
    CHECK(spy->get_coins() == 0);
    CHECK(gen->get_coins() == 2);
    CHECK(game.get_last_arrested_name() == "Spy");

    force_turn1(game, "Spy");
    spy->arrest(gen);
    CHECK(spy->get_coins() == 1);
    CHECK(gen->get_coins() == 2);
    CHECK(game.get_last_arrested_name() == "General");

    force_turn1(game, "Spy");
    spy->arrest(mer);
    CHECK(spy->get_coins() == 1);
    CHECK(mer->get_coins() == 0);
    CHECK(game.get_last_arrested_name() == "Merchant");
}

TEST_CASE("Player::arrest edge cases")
{
    Game game;
    auto spy = std::make_shared<Spy>(game, "Spy");
    auto gen = std::make_shared<General>(game, "General");
    auto mer = std::make_shared<Merchant>(game, "Merchant");

    game.add_player(spy);
    game.add_player(gen);
    game.add_player(mer);

    force_turn1(game, "General");
    CHECK_THROWS_AS(spy->arrest(gen), NotYourTurnException);

    force_turn1(game, "Spy");
    spy->set_must_coup(true);
    CHECK_THROWS_AS(spy->arrest(gen), MustPerformCoupException);
    spy->set_must_coup(false);

    force_turn1(game, "Spy");
    spy->set_disable_to_arrest(true);
    CHECK_THROWS_AS(spy->arrest(gen), ArrestBlockedException);
    spy->set_disable_to_arrest(false);

    gen->mark_eliminated();
    force_turn1(game, "Spy");
    CHECK_THROWS_AS(spy->arrest(gen), TargetIsEliminatedException);
    gen->revive();

    force_turn1(game, "Spy");
    CHECK_THROWS_AS(spy->arrest(spy), CannotTargetYourselfException);

    game.set_last_arrested_name("General");
    force_turn1(game, "Spy");
    CHECK_THROWS_AS(spy->arrest(gen), DuplicateArrestException);

    force_turn1(game, "Spy");
    CHECK_THROWS_AS(spy->arrest(mer), TargetNoCoinsException);

    game.set_last_arrested_name("Spy");
    force_turn1(game, "Spy");
    CHECK_THROWS_AS(spy->arrest(gen), TargetNoCoinsException);

    game.set_last_arrested_name("General");
    force_turn1(game, "General");
    CHECK_THROWS_AS(gen->arrest(spy), TargetNoCoinsException);
}

TEST_CASE("Player::sanction functionality")
{
    Game game;
    auto spy = std::make_shared<Spy>(game, "Spy");
    auto bar = std::make_shared<Baron>(game, "Baron");
    auto jud = std::make_shared<Judge>(game, "Judge");

    game.add_player(spy);
    game.add_player(bar);
    game.add_player(jud);

    spy->increase_coins(8);
    bar->increase_coins(8);
    jud->increase_coins(8);

    force_turn1(game, "Baron");
    bar->sanction(spy);
    CHECK(bar->get_coins() == 5);
    CHECK(spy->is_sanctioned() == true);

    spy->clear_sanctioned();
    force_turn1(game, "Spy");
    spy->sanction(bar);
    CHECK(spy->get_coins() == 5);
    CHECK(bar->get_coins() == 6);
    CHECK(bar->is_sanctioned() == true);

    force_turn1(game, "Spy");
    spy->sanction(jud);
    CHECK(spy->get_coins() == 1);
    CHECK(jud->is_sanctioned() == true);
}

TEST_CASE("Player::arrest edge cases")
{
    Game game;
    auto spy = std::make_shared<Spy>(game, "Spy");
    auto bar = std::make_shared<Baron>(game, "Baron");
    auto jud = std::make_shared<Judge>(game, "Judge");

    game.add_player(spy);
    game.add_player(bar);
    game.add_player(jud);

    force_turn1(game, "Baron");
    CHECK_THROWS_AS(spy->sanction(bar), NotYourTurnException);

    force_turn1(game, "Spy");
    spy->set_must_coup(true);
    CHECK_THROWS_AS(spy->sanction(bar), MustPerformCoupException);
    spy->set_must_coup(false);

    bar->mark_eliminated();
    force_turn1(game, "Spy");
    CHECK_THROWS_AS(spy->sanction(bar), TargetIsEliminatedException);
    bar->revive();

    force_turn1(game, "Spy");
    CHECK_THROWS_AS(spy->sanction(spy), CannotTargetYourselfException);

    bar->mark_sanctioned("Spy");

    game.set_last_arrested_name("General");
    force_turn1(game, "Spy");
    CHECK_THROWS_AS(spy->arrest(gen), DuplicateArrestException);

    force_turn1(game, "Spy");
    CHECK_THROWS_AS(spy->arrest(mer), TargetNoCoinsException);

    game.set_last_arrested_name("Spy");
    force_turn1(game, "Spy");
    CHECK_THROWS_AS(spy->arrest(gen), TargetNoCoinsException);

    game.set_last_arrested_name("General");
    force_turn1(game, "General");
    CHECK_THROWS_AS(gen->arrest(spy), TargetNoCoinsException);
}