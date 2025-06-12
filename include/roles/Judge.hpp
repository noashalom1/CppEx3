#ifndef JUDGE_HPP
#define JUDGE_HPP

#include "Player.hpp"

namespace coup
{

    class Judge : public Player
    {
    private:
        bool used_undo_this_round = false;



    public:
        Judge(Game &game, const std::string &name);
        bool can_undo_bribe() const { return !used_undo_this_round; }
        void mark_undo_bribe_used() { used_undo_this_round = true; }
        void reset_undo_bribe_flag() { used_undo_this_round = false; }
        void undo_bribe(Player &target);
        std::string role() const override;
    };

}

#endif