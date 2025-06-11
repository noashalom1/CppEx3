#ifndef DECK_HPP
#define DECK_HPP

#include <vector>
#include <string>
#include <random>
#include <ctime>

namespace coup {

    /**
     * @brief קופה של קלפים (Deck) למשחק Coup.
     * 
     * מחלקה זו מאפשרת לשלוף תפקיד אקראי מתוך סט של תפקידים.
     * השיטה מאפשרת שכפול תפקידים — כלומר ייתכן ששני שחקנים יקבלו את אותו תפקיד.
     * 
     * שיטה זו תואמת לדרישות המטלה (אין בעיה שיהיו תפקידים כפולים במשחק).
     */
    class Deck {
    private:
        std::vector<std::string> roles_pool = {
            "Governor", "Spy", "Baron", "General", "Judge", "Merchant"
        };
        std::mt19937 rng; ///< מחולל מספרים אקראיים

    public:
        /**
         * @brief בנאי - מאתחל את מחולל המספרים האקראיים.
         */
        Deck() {
            rng.seed(static_cast<unsigned int>(std::time(nullptr)));
        }

        /**
         * @brief שליפה אקראית של תפקיד מהקופה.
         * 
         * @return std::string שם התפקיד שנבחר
         */
        std::string draw_role() {
            std::uniform_int_distribution<std::size_t> dist(0, roles_pool.size() - 1);
            return roles_pool[dist(rng)];
        }
    };

}

#endif
