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

    void Player::gather() {
        check_turn();
        if (must_coup()) throw MustPerformCoupException();
        coins++;
    }

    void Player::tax() {
        check_turn();
        if (must_coup()) throw MustPerformCoupException();
        coins += 2;
    }

    void Player::bribe() {
        check_turn();
        if (must_coup()) throw MustPerformCoupException();
        if (coins < 4) throw NotEnoughCoinsException(4, coins);
        coins -= 4;
    }

    void Player::arrest(Player& target) {
        check_turn();
        if (target.get_name() == last_arrest_target) throw DuplicateArrestException();
        if (must_coup()) throw MustPerformCoupException();
        if (target.get_coins() <= 0) throw TargetNoCoinsException();
        if (target.is_eliminated()) throw TargetIsAlreadyEliminated();
        if (target.get_name() == name) throw GameException("You cannot arrest yourself.");
        target.coins--;
        coins++;
        last_arrest_target = target.get_name();
    }


    void Player::sanction(Player& target) {
        check_turn();
        if (must_coup()) throw MustPerformCoupException();
        if (coins < 3) throw NotEnoughCoinsException(3, coins);
        if (target.is_eliminated()) throw TargetIsAlreadyEliminated();
        if (target.get_name() == name) throw GameException("You cannot sanction yourself.");

        coins -= 3;
        target.mark_sanctioned();
    }

    void Player::coup(Player& target) {
        check_turn();
        if (coins < 7) throw NotEnoughCoinsException(7, coins);
        if (target.is_eliminated()) throw TargetIsAlreadyEliminated();
        if (target.get_name() == name) throw GameException("You cannot coup yourself.");
        game.remove_player(&target);
        coins -= 7;
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