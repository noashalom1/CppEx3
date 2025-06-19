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
        std::vector<Player *> players_list; // צריכה לעבור על הקוד ולהוריד איפה שיש- כי זה סתם בדיקות כפולות של אם יש שחקנים חיים
        size_t turn_index = 0;
        size_t global_turn_index = 0; // אינדקס גלובלי לסבב הנוכחי, לא תלוי בשחקן
        std::map<std::string, int > tax_turns;

        std::vector<std::pair<std::string, std::string>> coup_list;
        std::string last_arrested_name;
        std::string last_tax_player_name;
        std::map<std::string, std::pair<std::string, int>> last_actions;
        std::vector<std::tuple<std::string, std::string, int>> action_history; // שם שחקן, פעולה, מספר סבב
        int current_round = 1;

    public:
        Game();
        ~Game();

        // מוסיף שחקן למשחק (כל עוד יש מקום)
        void add_player(Player *player);
        std::vector<std::tuple<std::string, std::string, int>> &get_action_history()
        {
            return action_history;
        }
        int get_current_round() const { return current_round; }
        int get_active_players_count() const;
        // מחזיר את שם השחקן שתורו
        std::string turn() const;
        std::vector<std::pair<std::string, std::string>> &get_coup_list();
        std::map<std::string, int > &get_tax_turns();
      
        void add_to_coup(const std::string &attacker, const std::string &target);
        size_t get_global_turn_index() const { return global_turn_index; }
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
        std::map<std::string, std::pair<std::string, int>> &get_last_actions() { return last_actions; }
        int get_num_players() const { return static_cast<int>(players_list.size()); }
    };

}

#endif
