#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include <string>

namespace coup {

class Player;

class Game {
private:
    std::vector<Player*> players_list;
    size_t turn_index = 0;

public:
    Game();
    ~Game();

    // מוסיף שחקן למשחק (כל עוד יש מקום)
    void add_player(Player* player);

    // מחזיר את שם השחקן שתורו
    std::string turn() const;

    // מחזיר רשימה של שמות כל השחקנים
    std::vector<std::string> players() const;

    // מחזיר את שם המנצח אם יש אחד בלבד
    std::string winner() const;

    // מעבר לתור הבא (מדלג על מודחים)
    void next_turn();

    // מחזיר את רשימת השחקנים בפורמט מלא
    const std::vector<Player*>& get_players() const;

    // הסרה לוגית של שחקן (סימון כהודח)
    void remove_player(Player* player);

    // מחפש שחקן לפי שם
    Player* get_player(const std::string& name);

    // מחזיר את השחקן הנוכחי שתורו
    Player* get_current_player();

    // בדיקה אם יש גנרל שחוסם את ההפיכה
    bool general_blocks_coup(Player* attacker, Player* target);

    // מבטל פעולה של coup במידה וחסומה
    void cancel_coup(Player* attacker, Player* target);

    // מסמן לשחקן שהוא חייב לבצע coup (אם יש לו ≥10 מטבעות)
    void check_force_coup(Player* current_player);
};

}

#endif
