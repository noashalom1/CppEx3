// Updated Player.hpp

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <set>
#include <map>
#include "Game.hpp"

namespace coup
{

    class Game;

    class Player
    {

    protected:
        Game &game;
        std::string name;
        int coins;
        bool eliminated = false;
        bool mustPerformCoup = false;
        std::string last_action;
        // std::string last_arrest_target;
        bool sanctioned = false;
        // bool disable_to_arrest = false;
        int extra_turns = 0;
        std::string sanctioned_by;
       

    public:
        Player(Game &game, const std::string &name);
        virtual ~Player();
        
        const std::string &get_name() const;
        int get_coins() const;
        virtual std::string role() const = 0;
        void check_turn() const;
        void revive();

        virtual void gather();
        virtual void tax();
        virtual void bribe();
        virtual void arrest(Player &target);
        virtual void sanction(Player &target);
        virtual void coup(Player &target);

        void decrease_coins(int amount);
        void increase_coins(int amount);

        void mark_eliminated();
        bool is_eliminated() const;

        void mark_sanctioned(const std::string &by_whom);
        void clear_sanctioned();
        bool is_sanctioned() const;

        // void set_disable_to_arrest();
        // void clear_disable_to_arrest();
        // bool is_disable_to_arrest() const;

        void set_must_coup(bool value);
        bool must_coup() const;

        bool is_extra_turn() const { return extra_turns > 0; }
        void set_extra_turns(int value) { extra_turns = value; }
        int get_extra_turns() const { return extra_turns; }

        virtual void start_new_turn()
        {
            if (coins >= 10)
            {
                mustPerformCoup = true;
            }
            else
            {
                mustPerformCoup = false;
            }

            // רק אם השחקן הזה הוא זה שביצע את הסנקציה – ננקה את הסנקציה מהשחקנים שסומנו על ידו
            for (Player *p : game.get_players())
            {
                if (p->is_sanctioned() && p->sanctioned_by == name)
                {
                    p->clear_sanctioned();
                }
            }
        }

        void set_last_action(const std::string &action) { last_action = action; }
        std::string get_last_action() const { return last_action; }

        const std::string &name_ref() const { return name; }
    };

}

#endif
