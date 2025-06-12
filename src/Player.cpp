#include "Player.hpp"
#include "exceptions.hpp"
#include "Game.hpp"

namespace coup {

    Player::Player(Game& game, const std::string& name)
        : game(game), name(name), coins(0) {
    }

    Player::~Player() {}

    const std::string& Player::get_name() const {
        return name;
    }

    int Player::get_coins() const {
        return coins;
    }
    
   void Player::check_turn() const {
        if (game.turn() != name) throw NotYourTurnException();;
    }

    void Player::revive() {
        if (!is_eliminated()) throw GameException("Player is not eliminated.");
        eliminated = false;
    }

    void Player::gather() {
        check_turn();
        if (must_coup()) throw MustPerformCoupException();
        coins++;
        game.next_turn();
    }

    void Player::tax() {
        check_turn();
        if (must_coup()) throw MustPerformCoupException();
        coins += 2;
        game.next_turn();
    }

    void Player::bribe() {
        check_turn();
        if (coins < 4) throw NotEnoughCoinsException(4, coins);
        coins -= 4;
        extra_turn = true; // Allow an extra turn after bribe
        set_last_action("bribe");
        has_done_bribe = true; // Mark that the player has done a bribe
    }

    void Player::arrest(Player& target) {
        check_turn();
        if (target.get_name() == last_arrest_target) throw DuplicateArrestException();
        if (must_coup()) throw MustPerformCoupException();
        if (target.is_eliminated()) throw TargetIsAlreadyEliminated();
        if (target.get_name() == name) throw GameException("You cannot arrest yourself.");
        if(is_disable_to_arrest()) throw GameException("You cannot arrest this player, you are disabled from arresting.");
        if(target.role() == "General"){
            if (target.get_coins() <= 0) throw TargetNoCoinsException();
            coins++;
        }
        else if(target.role() == "Merchant"){
            if (target.get_coins() <= 1) throw TargetNoCoinsException();
            target.coins -= 2; // Merchant loses 2 coins
        } else {
            if (target.get_coins() <= 0) throw TargetNoCoinsException();
            target.coins--;
            coins++;

        }
        last_arrest_target = target.get_name();
        game.next_turn();
    }


    void Player::sanction(Player& target) {
        check_turn();
        if (must_coup()) throw MustPerformCoupException();
        if (target.is_eliminated()) throw TargetIsAlreadyEliminated();
        if (target.get_name() == name) throw GameException("You cannot sanction yourself.");
        if(target.is_sanctioned()) throw GameException("Target is already sanctioned.");
        if(target.role() == "Baron") target.increase_coins(1); // Baron gets 1 coin back
        if((target.role() == "Judge" && coins < 4) || coins < 3) throw NotEnoughCoinsException(target.role() == "Judge" ? 4: 3, coins);
        target.role() == "Judge" ? coins -= 4 : coins -= 3; // Judge costs 4, others cost 3
        target.mark_sanctioned();
        game.next_turn();
    }

    void Player::coup(Player& target) {
        check_turn();
        if (coins < 7) throw NotEnoughCoinsException(7, coins);
        if (target.is_eliminated()) throw TargetIsAlreadyEliminated();
        if (target.get_name() == name) throw GameException("You cannot coup yourself.");
        game.remove_player(&target);
        game.add_to_coup(name, target.get_name());
        coins -= 7;
        game.next_turn();
    }

    void Player::decrease_coins(int amount) {
        if (coins < amount) {
            throw NotEnoughCoinsException(amount, coins);
        }
        coins -= amount;
    }

    void Player::increase_coins(int amount) {
        coins += amount;
    }

    void Player::mark_eliminated() {
        eliminated = true;
    }

    bool Player::is_eliminated() const {
        return eliminated;
    }

    void Player::mark_sanctioned() {
        sanctioned = true;
    }

    void Player::clear_sanctioned() {
        sanctioned = false;
    }

    bool Player::is_sanctioned() const {
        return sanctioned;
    }

    void Player::set_disable_to_arrest() {
        disable_to_arrest = true;
    }

    void Player::clear_disable_to_arrest() {
        disable_to_arrest = false;
    }

    bool Player::is_disable_to_arrest() const {
        return disable_to_arrest;
    }

    void Player::set_must_coup(bool value) {
        mustPerformCoup = value;
    }

    bool Player::must_coup() const {
        return mustPerformCoup;
    }

}