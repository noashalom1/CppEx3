// Author: noapatito123@gmail.com
#pragma once

#include <string>
#include <set>
#include <map>
#include "Game.hpp"


namespace coup
{

    class Game;

    class Player
    {
    private:
        bool eliminated;
        bool disable_to_arrest;

    protected:
        Game &game;
        std::string name;
        int coins;

        bool mustPerformCoup;

        bool sanctioned;
        int extra_turns;
        std::string sanctioned_by;

        int disable_arrest_turns;
        bool used_bribe;

    public:
        Player(Game &game, const std::string &name);
        virtual ~Player();

        const std::string &get_name() const;

        virtual std::string role() const = 0;
        void check_turn() const;
        void revive();

        virtual void gather();
        virtual void tax();
        virtual void bribe();
        virtual void arrest(Player &target);
        virtual void sanction(Player &target);
        virtual void coup(Player &target);

        int get_coins() const { return coins; }
        void decrease_coins(int amount);
        void increase_coins(int amount);

        void mark_eliminated();
        bool is_eliminated() const;

        void mark_sanctioned(const std::string &by_whom);
        void clear_sanctioned();
        bool is_sanctioned() const;

        void set_disable_to_arrest(bool value) { disable_to_arrest = value; }
        bool is_disable_to_arrest() const { return disable_to_arrest; }
        void set_disable_arrest_turns(int n) { disable_arrest_turns = n; }
        int get_disable_arrest_turns() const { return disable_arrest_turns; }

        void set_must_coup(bool value);
        bool must_coup() const;

        bool is_used_bribe() const { return used_bribe; }
        void mark_used_bribe() { used_bribe = true; }
        void reset_used_bribe() { used_bribe = false; }

        bool is_extra_turn() const { return extra_turns > 0; }
        void set_extra_turns(int value) { extra_turns = value; }
        int get_extra_turns() const { return extra_turns; }

        const std::string &name_ref() const { return name; }

        virtual void start_new_turn()
        {
            mustPerformCoup = (coins >= 10); // Automatically enforce COUP requirement if player has 10 or more coins

             // Clear SANCTIONED status from players sanctioned by this player
            for (Player *p : game.get_players())
            {
                if (p->is_sanctioned() && p->sanctioned_by == name)
                {
                    p->clear_sanctioned();
                }
            }
            reset_used_bribe(); // Reset used bribe flag at the beginning of the turn
        }
    };

}
