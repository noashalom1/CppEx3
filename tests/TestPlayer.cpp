
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

TEST_CASE("Player::sanction edge cases")
{
    Game game;
    auto p1 = std::make_shared<Spy>(game, "P1");
    auto p2 = std::make_shared<Merchant>(game, "P2");
    auto p3 = std::make_shared<General>(game, "P3");
    auto judge = std::make_shared<Judge>(game, "Judge");

    game.add_player(p1);
    game.add_player(p2);
    game.add_player(p3);
    game.add_player(judge);

    // 1. Not your turn
    force_turn1(game, "P2");
    p1->increase_coins(3);
    CHECK_THROWS_AS(p1->sanction(p2), NotYourTurnException);

    // 2. Must perform coup
    force_turn1(game, "P1");
    p1->set_must_coup(true);
    CHECK_THROWS_AS(p1->sanction(p2), MustPerformCoupException);
    p1->set_must_coup(false);

    // 3. Target is eliminated
    p2->mark_eliminated();
    force_turn1(game, "P1");
    CHECK_THROWS_AS(p1->sanction(p2), TargetIsEliminatedException);
    p2->revive();

    // 4. Cannot sanction yourself
    force_turn1(game, "P1");
    CHECK_THROWS_AS(p1->sanction(p1), CannotTargetYourselfException);

    // 5. Already sanctioned
    force_turn1(game, "P1");
    p1->increase_coins(3);
    p1->sanction(p3); // First sanction
    CHECK(p3->is_sanctioned());

    // Try again before full round
    force_turn1(game, "P2");
    p2->increase_coins(3);
    CHECK_THROWS_AS(p2->sanction(p3), AlreadySanctionedException);

    // 6. Not enough coins to sanction regular player
    force_turn1(game, "P1");
    p1->decrease_coins(p1->get_coins()); // Set coins to 0
    CHECK_THROWS_AS(p1->sanction(p3), NotEnoughCoinsException);

    // 7. Not enough coins to sanction Judge (costs 4)
    p1->increase_coins(3); // Only 3, not enough for Judge
    CHECK_THROWS_AS(p1->sanction(judge), NotEnoughCoinsException);
}

TEST_CASE("Player::coup functionality") {
    Game game;
    auto spy = std::make_shared<Spy>(game, "Spy");
    auto bar = std::make_shared<Baron>(game, "Baron");
    auto extra = std::make_shared<General>(game, "General"); // Third player

    game.add_player(spy);
    game.add_player(bar);
    game.add_player(extra);

    spy->increase_coins(7);

    force_turn1(game, "Spy");
    spy->coup(bar);

    CHECK(spy->get_coins() == 0);
    CHECK(bar->is_eliminated() == true);

    // Check coup list BEFORE turn returns to Spy (before round ends)
    CHECK(game.is_in_coup_list(bar->get_name()) == true); 

    force_turn1(game, "Spy");     // Back to Spy (new round begins)

    // coup_list should be cleared after full round
    CHECK_FALSE(game.is_in_coup_list(bar->get_name())); // coup_list is expected to be empty
}

TEST_CASE("Player::coup edge cases") {
    Game game;
    auto p1 = std::make_shared<Spy>(game, "P1");
    auto p2 = std::make_shared<Baron>(game, "P2");
    auto p3 = std::make_shared<Merchant>(game, "P3");

    game.add_player(p1);
    game.add_player(p2);
    game.add_player(p3);

    // 1. Not your turn
    force_turn1(game, "P2");
    p1->increase_coins(7);
    CHECK_THROWS_AS(p1->coup(p2), NotYourTurnException);

    // 2. Not enough coins
    force_turn1(game, "P1");
    p1->decrease_coins(7); // Now has 0 coins
    CHECK_THROWS_AS(p1->coup(p2), NotEnoughCoinsException);

    // 3. Target already eliminated
    p1->increase_coins(7); // Recharge
    p2->mark_eliminated();
    CHECK_THROWS_AS(p1->coup(p2), TargetIsAlreadyEliminatedException);
    p2->revive();

    // 4. Cannot target yourself
    CHECK_THROWS_AS(p1->coup(p1), CannotTargetYourselfException);
}

TEST_CASE("Player::check_turn") {
    Game game;
    auto p1 = std::make_shared<Spy>(game, "P1");
    auto p2 = std::make_shared<Spy>(game, "P2");
    game.add_player(p1);
    game.add_player(p2);

    force_turn1(game, "P2");
    CHECK_THROWS_AS(p1->check_turn(), NotYourTurnException);

    force_turn1(game, "P1");
    CHECK_NOTHROW(p1->check_turn());
}

TEST_CASE("Player::revive") {
    Game game;
    auto p1 = std::make_shared<Spy>(game, "P1");
    game.add_player(p1);

    CHECK_THROWS_AS(p1->revive(), TargetNotEliminatedException);

    p1->mark_eliminated();
    CHECK(p1->is_eliminated());

    CHECK_NOTHROW(p1->revive());
    CHECK_FALSE(p1->is_eliminated());
}

TEST_CASE("Player::decrease_coins") {
    Game game;
    auto p1 = std::make_shared<Spy>(game, "P1");
    game.add_player(p1);

    p1->increase_coins(3);
    CHECK(p1->get_coins() == 3);

    CHECK_THROWS_AS(p1->decrease_coins(5), NotEnoughCoinsException);

    CHECK_NOTHROW(p1->decrease_coins(2));
    CHECK(p1->get_coins() == 1);
}

TEST_CASE("Player::increase_coins") {
    Game game;
    auto p1 = std::make_shared<Spy>(game, "P1");
    game.add_player(p1);

    CHECK(p1->get_coins() == 0);
    p1->increase_coins(5);
    CHECK(p1->get_coins() == 5);
}

TEST_CASE("Player::mark_sanctioned") {
    Game game;
    auto p1 = std::make_shared<Spy>(game, "P1");
    game.add_player(p1);

    CHECK_FALSE(p1->is_sanctioned());
    p1->mark_sanctioned("P2");
    CHECK(p1->is_sanctioned());
}

TEST_CASE("Player::clear_sanctioned") {
    Game game;
    auto p1 = std::make_shared<Spy>(game, "P1");
    game.add_player(p1);

    p1->mark_sanctioned("P2");
    CHECK(p1->is_sanctioned());

    p1->clear_sanctioned();
    CHECK_FALSE(p1->is_sanctioned());
}
