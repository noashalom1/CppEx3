#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include <string>
#include <map>

namespace coup
{

    class Player;

    class Game
    {
    private:
        std::vector<Player *> players_list;
        size_t turn_index = 0;
        std::vector<std::pair<std::string, std::string>> coup_list;
        std::string last_arrested_name;
        std::string last_tax_player_name;
        std::map<std::string, std::pair<std::string, int>> last_actions;
        std::vector<std::pair<std::string, std::string>> action_history; // {player name, action}
    public:
        Game();
        ~Game();

        // מוסיף שחקן למשחק (כל עוד יש מקום)
        void add_player(Player *player);
        std::vector<std::pair<std::string, std::string>>& get_action_history() {
        return action_history;}
        // מחזיר את שם השחקן שתורו
        std::string turn() const;
        std::vector<std::pair<std::string, std::string>> &get_coup_list();
        void add_to_coup(const std::string &attacker, const std::string &target);

        // מחזיר רשימה של שמות כל השחקנים
        std::vector<std::string> players() const;

        // מחזיר את שם המנצח אם יש אחד בלבד
        std::string winner() const;

        // מעבר לתור הבא (מדלג על מודחים)
        void next_turn();

        // מחזיר את רשימת השחקנים בפורמט מלא
        const std::vector<Player *> &get_players() const;

        // הסרה לוגית של שחקן (סימון כהודח)
        void remove_player(Player *player);

        // מחפש שחקן לפי שם
        Player *get_player(const std::string &name);

        // מחזיר את השחקן הנוכחי שתורו
        Player *get_current_player();

        void set_last_arrested_name(const std::string &name);
        const std::string &get_last_arrested_name() const;

        // מסמן לשחקן שהוא חייב לבצע coup (אם יש לו ≥10 מטבעות)
        void check_force_coup(Player *current_player);

        void set_last_tax_player_name(const std::string &name);
        const std::string &get_last_tax_player_name() const;
        std::map<std::string, std::pair<std::string, int>>& get_last_actions() { return last_actions; }
        int get_num_players() const { return static_cast<int>(players_list.size()); }
    };

}

#endif
