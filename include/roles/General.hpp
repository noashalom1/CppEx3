// Author: noapatito123@gmail.com
#pragma once

#include "Player.hpp"

namespace coup
{

    class General : public Player
    {
    private:
        bool used_undo_this_round = false;

    public:
        General(Game &game, const std::string &name);
        bool can_undo_coup() const { return !used_undo_this_round; }
        void mark_undo_coup_used() { used_undo_this_round = true; }
        void reset_undo_coup_flag() { used_undo_this_round = false; }
        std::string undo_coup(Player &target);
        std::string role() const override;
    };

}

