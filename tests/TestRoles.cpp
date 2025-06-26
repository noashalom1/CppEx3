
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

void force_turn(Game &game, const std::string &name)
{
    for (int i = 0; i < 20; i++)
    {
        if (game.turn() == name)
            return;
        game.next_turn();
    }
}

TEST_CASE("Governor::tax and undo_tax functionality")
{
    Game game;
    auto gov1 = std::make_shared<Governor>(game, "Gov1");
    auto gov2 = std::make_shared<Governor>(game, "Gov2");
    game.add_player(gov1);
    game.add_player(gov2);

    force_turn(game, "Gov1");
    gov1->tax();
    CHECK(gov1->get_coins() == 3);

    force_turn(game, "Gov2");
    gov2->tax();

    force_turn(game, "Gov1");
    CHECK_NOTHROW(gov1->undo_tax());
    CHECK(gov2->get_coins() == 0);
}

TEST_CASE("Governor::tax edge cases")
{
    Game game;
    auto gov = std::make_shared<Governor>(game, "Gov");
    auto other = std::make_shared<Governor>(game, "Other");
    game.add_player(gov);
    game.add_player(other);

    force_turn(game, "Other");
    CHECK_THROWS_AS(gov->tax(), NotYourTurnException);

    force_turn(game, "Gov");
    gov->set_must_coup(true);
    CHECK_THROWS_AS(gov->tax(), MustPerformCoupException);
    gov->set_must_coup(false);

    force_turn(game, "Gov");
    gov->mark_sanctioned("Other");
    CHECK_THROWS_AS(gov->tax(), SanctionedException);
}

TEST_CASE("Governor::undo_tax edge cases")
{
    Game game;
    auto gov = std::make_shared<Governor>(game, "Gov");
    auto other = std::make_shared<Spy>(game, "Other");
    game.add_player(gov);
    game.add_player(other);

    // Case 1: No recent tax action to undo
    force_turn(game, "Gov");
    CHECK_THROWS_AS(gov->undo_tax(), NoRecentActionToUndoException);

    // Case 2: Target is eliminated
    force_turn(game, "Other");
    other->tax();
    other->mark_eliminated();
    CHECK_THROWS_AS(gov->undo_tax(), TargetIsEliminatedException);

    // Case 3: Governor tries to undo their own tax
    gov->tax();
    CHECK_THROWS_AS(gov->undo_tax(), CannotUndoOwnActionException);

    // Case 4: Undo already used this round
    gov->mark_undo_tax_used();
    CHECK_THROWS_AS(gov->undo_tax(), ActionAlreadyUsedThisRoundException);
    gov->reset_undo_tax_flag();

    // Case 5: Governor is eliminated
    gov->mark_eliminated();
    CHECK_THROWS_AS(gov->undo_tax(), PlayerEliminatedException);
}

TEST_CASE("Judge::undo_tax edge case (ActionTooOldException)")
{
    Game game;
    auto judge = std::make_shared<Judge>(game, "Judge");
    auto gov = std::make_shared<Governor>(game, "Gov");

    game.add_player(judge);
    game.add_player(gov);

    judge->tax();
    force_turn(game, "gov");
    game.next_turn();
    force_turn(game, "Judge");

    CHECK_THROWS_AS(gov->undo_tax(), ActionTooOldException);
}

TEST_CASE("Baron::invest functionality")
{
    Game game;
    auto baron = std::make_shared<Baron>(game, "Baron");
    game.add_player(baron);
    baron->increase_coins(3);
    force_turn(game, "Baron");
    CHECK_NOTHROW(baron->invest());
    CHECK(baron->get_coins() == 6);
}

TEST_CASE("Baron::invest edge cases")
{
    Game game;
    auto baron = std::make_shared<Baron>(game, "Baron");
    auto other = std::make_shared<Governor>(game, "Other");
    game.add_player(baron);
    game.add_player(other);

    force_turn(game, "Other");
    baron->increase_coins(3);
    CHECK_THROWS_AS(baron->invest(), NotYourTurnException);

    force_turn(game, "Baron");
    baron->set_must_coup(true);
    CHECK_THROWS_AS(baron->invest(), MustPerformCoupException);
    baron->set_must_coup(false);

    baron->decrease_coins(3);
    force_turn(game, "Baron");
    CHECK_THROWS_AS(baron->invest(), NotEnoughCoinsException);
}

TEST_CASE("General::undo_coup functionality")
{
    Game game;
    auto judge = std::make_shared<Judge>(game, "Judge");
    auto general = std::make_shared<General>(game, "General");
    auto target = std::make_shared<Spy>(game, "Target");

    game.add_player(judge);
    game.add_player(general);
    game.add_player(target);

    judge->increase_coins(7);
    judge->coup(target);
    general->increase_coins(5);
    CHECK_NOTHROW(general->undo_coup(target));
    CHECK_FALSE(target->is_eliminated());
    CHECK_FALSE(game.is_in_coup_list(target->get_name()));
}

TEST_CASE("General::undo_coup - TargetNotEliminated")
{
    Game game;
    auto g = std::make_shared<General>(game, "G");
    auto t = std::make_shared<Governor>(game, "T");
    game.add_player(g);
    game.add_player(t);

    g->increase_coins(5);
    CHECK_THROWS_AS(g->undo_coup(t), TargetNotEliminatedException);
}

TEST_CASE("General::undo_coup - NotEnoughCoins")
{
    Game game;
    auto g = std::make_shared<General>(game, "G");
    auto t = std::make_shared<Governor>(game, "T");
    game.add_player(g);
    game.add_player(t);
    t->mark_eliminated();
    CHECK_THROWS_AS(g->undo_coup(t), NotEnoughCoinsException);
}

TEST_CASE("General::undo_coup - ActionAlreadyUsedThisRound")
{
    Game game;
    auto g = std::make_shared<General>(game, "G");
    auto t = std::make_shared<Governor>(game, "T");
    game.add_player(g);
    game.add_player(t);
    t->mark_eliminated();
    g->increase_coins(5);
    g->mark_undo_coup_used();
    CHECK_THROWS_AS(g->undo_coup(t), ActionAlreadyUsedThisRoundException);
}

TEST_CASE("General::undo_coup throws if target not in coup_list") {
    Game game;
    auto general = std::make_shared<General>(game, "General");
    auto spy = std::make_shared<Spy>(game, "Spy");
    auto baron = std::make_shared<Baron>(game, "Baron");

    game.add_player(general);
    game.add_player(spy);
    game.add_player(baron);

    // Give general enough coins to perform undo_coup
    general->increase_coins(5);

    // Eliminate baron manually (not by coup)
    baron->mark_eliminated();

    // Try undo_coup on baron who is eliminated but NOT in coup_list
    force_turn(game, "General");
    CHECK_THROWS_AS(general->undo_coup(baron), NoCoupToUndoException);
}


TEST_CASE("Judge::undo_bribe functionality")
{
    Game game;
    auto judge = std::make_shared<Judge>(game, "Judge");
    auto target = std::make_shared<Merchant>(game, "Target");
    game.add_player(judge);
    game.add_player(target);

    target->mark_used_bribe();
    target->set_extra_turns(1);
    CHECK_NOTHROW(judge->undo_bribe(target));
    CHECK(target->get_extra_turns() == 0);
}

TEST_CASE("Judge::undo_bribe edge cases")
{
    Game game;
    auto judge = std::make_shared<Judge>(game, "Judge");
    auto target = std::make_shared<Merchant>(game, "Target");
    game.add_player(judge);
    game.add_player(target);
    judge->increase_coins(4);
    target->increase_coins(4);

    judge->mark_eliminated();
    CHECK_THROWS_AS(judge->undo_bribe(target), PlayerEliminatedException);
    judge->revive();
    
    target->reset_used_bribe();
    CHECK_THROWS_AS(judge->undo_bribe(target), UndoNotAllowedException);
    target->mark_used_bribe();

    judge->bribe();
    CHECK_THROWS_AS(judge->undo_bribe(judge), CannotUndoOwnActionException);

    judge->mark_undo_bribe_used();
    CHECK_THROWS_AS(judge->undo_bribe(target), ActionAlreadyUsedThisRoundException);
    judge->reset_undo_bribe_flag();

    target->mark_eliminated();
    CHECK_THROWS_AS(judge->undo_bribe(target), TargetIsEliminatedException);
}

TEST_CASE("Merchant::start_new_turn bonus")
{
    Game game;
    Merchant m(game, "Merch");
    m.increase_coins(3);
    m.start_new_turn();
    CHECK(m.get_coins() == 4);
}

TEST_CASE("Spy::peek_and_disable valid and invalid")
{
    Game game;
    auto spy = std::make_shared<Spy>(game, "Spy");
    auto t = std::make_shared<Governor>(game, "Target");
    game.add_player(spy);
    game.add_player(t);

    spy->mark_eliminated();
    CHECK_THROWS_AS(spy->peek_and_disable(t), PlayerEliminatedException);
    spy->revive();

    CHECK_NOTHROW(spy->peek_and_disable(t));
    CHECK(t->is_disable_to_arrest());

    auto another = std::make_shared<Merchant>(game, "M");
    game.add_player(another);
    CHECK_THROWS_AS(spy->peek_and_disable(another), ActionAlreadyUsedThisRoundException);
}

TEST_CASE("Spy::peek_and_disable throws on self or eliminated target")
{
    Game game;
    auto spy = std::make_shared<Spy>(game, "Spy");
    game.add_player(spy);
    CHECK_THROWS_AS(spy->peek_and_disable(spy), CannotTargetYourselfException);

    auto t = std::make_shared<Governor>(game, "Dead");
    t->mark_eliminated();
    game.add_player(t);
    CHECK_THROWS_AS(spy->peek_and_disable(t), TargetIsEliminatedException);
}
