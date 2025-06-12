#include "GameGUI.hpp"
#include "Player.hpp"
#include <iostream>
#include "exceptions.hpp"
#include "Governor.hpp"
#include "Spy.hpp"
#include "Baron.hpp"
#include "General.hpp"
#include "Judge.hpp"
#include "Merchant.hpp"
#include "TextBox.hpp"
#include <stdexcept>

using namespace coup;
using namespace sf;

namespace
{
    std::vector<std::string> tempNames;
    std::vector<std::string> tempRoles;
    std::vector<std::string> allRoles = {"Governor", "Spy", "Baron", "General", "Judge", "Merchant"};
    std::string setupError;
    std::string actionMessage;

    std::string randomRole()
    {
        return allRoles[rand() % allRoles.size()];
    }
}

GameGUI::GameGUI() : window(VideoMode(1000, 700), "Coup Interactive GUI")
{
    if (!font.loadFromFile("arial.ttf"))
    {
        throw std::runtime_error("Failed to load font");
    }
    srand(time(nullptr));
    nameBox = new TextBox(font, {300, 30}, {50, 50});
    nameBox->setSelected(true);

    addPlayerBtn = new Button("Add Player", font, {150, 40}, {370, 50});
    addPlayerBtn->setAction([this]()
                            {
        std::string name = nameBox->getText();
        if (name.empty()) {
            setupError = "Name cannot be empty";
            return;
        }
        if (tempNames.size() >= 6) {
            setupError = "Cannot add more than 6 players";
            return;
        }
        tempNames.push_back(name);
        tempRoles.push_back(randomRole());
        nameBox->clear();
        setupError.clear(); });

    startGameBtn = new Button("Start Game", font, {150, 40}, {50, 100});
    startGameBtn->setAction([this]()
                            {
        if (tempNames.size() < 2) {
            setupError = "At least 2 players required";
            return;
        }
        for (size_t i = 0; i < tempNames.size(); ++i) {
            const std::string& role = tempRoles[i];
            if (role == "Governor") game.add_player(new Governor(game, tempNames[i]));
            else if (role == "Spy") game.add_player(new Spy(game, tempNames[i]));
            else if (role == "Baron") game.add_player(new Baron(game, tempNames[i]));
            else if (role == "General") game.add_player(new General(game, tempNames[i]));
            else if (role == "Judge") game.add_player(new Judge(game, tempNames[i]));
            else if (role == "Merchant") game.add_player(new Merchant(game, tempNames[i]));
        }

        setupButtons();
        state = GUIState::InGame; });
    demoGameBtn = new Button("Demo Game", font, {150, 40}, {220, 100});
    demoGameBtn->setAction([this]()
                           {
            if (tempNames.size() > 0) {
                setupError = "Clear existing players before demo";
                return;
            }
            tempNames = {"Alice", "Bob", "Carol", "Dave", "Eve", "Frank"};
            tempRoles = {"Spy", "Governor", "General", "Judge", "Baron", "Merchant"};
            setupError.clear(); });
}

void GameGUI::showTargetSelection(std::function<void(Player *)> action, bool includeCurrentPlayer, const std::vector<Player *> &targets)
{
    state = GUIState::TargetSelection;
    targetButtons.clear();
    targetAction = action;

    Player *current = game.get_current_player();
    float y = 150;

    std::vector<Player *> finalTargets;

    if (!targets.empty())
    {
        // סינון לפי includeCurrentPlayer רק אם current נמצא ברשימה
        for (Player *p : targets)
        {
            if (!includeCurrentPlayer && p == current)
            {
                continue; // דלג על current player
            }
            finalTargets.push_back(p);
        }
    }
    else
    {
        // ברירת מחדל – שחקנים חיים בלבד
        const auto &all = game.get_players();
        for (Player *p : all)
        {
            if (!p->is_eliminated() && (includeCurrentPlayer || p != current))
            {
                finalTargets.push_back(p);
            }
        }
    }

    for (Player *p : finalTargets)
    {
        Button btn(p->get_name(), font, {200, 40}, {400, y});
        btn.setAction([this, p]()
                      {
            try {
                targetAction(p);  
                state = GUIState::InGame;
                targetButtons.clear();
            } catch (const GameException& e) {
                inGameError = e.what();
                actionMessage.clear();
                targetButtons.clear();
                state = GUIState::InGame;
            } });
        targetButtons.push_back(btn);
        y += 50;
    }
}

void GameGUI::run()
{
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
            else if (event.type == Event::MouseButtonPressed)
            {
                float x = event.mouseButton.x;
                float y = event.mouseButton.y;
                if (state == GUIState::Setup)
                {
                    if (addPlayerBtn->contains(x, y))
                        addPlayerBtn->execute();
                    if (startGameBtn->contains(x, y))
                        startGameBtn->execute();
                    if (demoGameBtn->contains(x, y))
                        demoGameBtn->execute();
                    nameBox->setSelected(nameBox->getText().empty());
                }
                else if (state == GUIState::InGame)
                {
                    for (Button &btn : buttons)
                    {
                        if (btn.contains(x, y))
                        {
                            try
                            {
                                btn.execute();
                            }
                            catch (const GameException &e)
                            {
                                inGameError = e.what();
                                actionMessage.clear();
                            }
                            break;
                        }
                    }
                }
                else if (state == GUIState::TargetSelection)
                {
                    for (Button &btn : targetButtons)
                    {
                        if (btn.contains(x, y))
                        {
                            try
                            {
                                btn.execute();
                                state = GUIState::InGame;
                                targetButtons.clear();
                            }
                            catch (const GameException &e)
                            {
                                inGameError = e.what();
                                actionMessage.clear();
                                state = GUIState::InGame;
                                targetButtons.clear();
                            }
                            break;
                        }
                    }
                }
            }
            else if (event.type == Event::TextEntered)
            {
                if (state == GUIState::Setup)
                    nameBox->handleEvent(event);
            }
        }

        window.clear(Color(50, 50, 50));
        if (state == GUIState::Setup)
        {
            nameBox->draw(window);
            addPlayerBtn->draw(window);
            demoGameBtn->draw(window);
            if (tempNames.size() >= 2)
                startGameBtn->draw(window);

            float y = 160;
            for (size_t i = 0; i < tempNames.size(); ++i)
            {
                Text t;
                t.setFont(font);
                t.setCharacterSize(18);
                t.setFillColor(Color::White);
                t.setString(tempNames[i] + " - " + tempRoles[i]);
                t.setPosition(50, y);
                window.draw(t);
                y += 30;
            }

            if (!setupError.empty())
            {
                Text err;
                err.setFont(font);
                err.setCharacterSize(18);
                err.setFillColor(Color::Red);
                err.setString(setupError);
                err.setPosition(50, 500);
                window.draw(err);
            }
        }
        else
        {
            Player *current = game.get_current_player();
            std::string info = "Turn: " + current->get_name() + " (" + current->role() + "), Coins: " + std::to_string(current->get_coins());

            sf::Text header;
            header.setFont(font);
            header.setCharacterSize(22);
            header.setFillColor(sf::Color::White);
            header.setString(info);
            header.setPosition(300, 20);
            window.draw(header);

            // הודעה על פעולה שבוצעה
            if (!actionMessage.empty())
            {
                sf::Text msgText;
                msgText.setFont(font);
                msgText.setCharacterSize(20);
                msgText.setFillColor(sf::Color::Green);
                msgText.setString(actionMessage);
                msgText.setPosition(70, 610);
                window.draw(msgText);
            }

            // כותרת לרשימת שחקנים
            sf::Text playerListTitle;
            playerListTitle.setFont(font);
            playerListTitle.setCharacterSize(18);
            playerListTitle.setFillColor(sf::Color::Blue);
            playerListTitle.setString("Players In Game:");
            playerListTitle.setPosition(750, 470);
            window.draw(playerListTitle);

            // כותרת לפעולות מיוחדות
            sf::Text specialTitle;
            specialTitle.setFont(font);
            specialTitle.setCharacterSize(18);
            specialTitle.setFillColor(sf::Color::Blue);
            specialTitle.setString("Special Actions:");
            specialTitle.setPosition(800, 20);
            window.draw(specialTitle);

            if (state == GUIState::InGame && targetButtons.empty())
            {
                buttons.clear();
                setupButtons();
            }

            if (state == GUIState::InGame)
            {
                for (const Button &btn : buttons)
                {
                    btn.draw(window);
                }
            }

            drawPlayerList();
            if (state == GUIState::TargetSelection)
            {
                for (const Button &btn : targetButtons)
                {
                    btn.draw(window);
                }
            }
            if (!inGameError.empty())
            {
                sf::Text errText;
                errText.setFont(font);
                errText.setCharacterSize(18);
                errText.setFillColor(sf::Color::Red);
                errText.setString(inGameError);
                errText.setPosition(50, 650);
                window.draw(errText);
            }
        }
        window.display();
    }
}

void GameGUI::setupButtons()
{
    Player *p = game.get_current_player();
    std::string role = p->role();

    Button gatherBtn("Gather", font, sf::Vector2f(150, 40), sf::Vector2f(50, 100));
    gatherBtn.setAction([this]()
                        {
        Player* p = game.get_current_player();
        p->gather();
        p->set_last_action("gather");
        actionMessage = p->get_name() + " performed Gather. Coins: " + std::to_string(p->get_coins());
        inGameError.clear(); });
    buttons.push_back(gatherBtn);

    Button taxBtn("Tax", font, sf::Vector2f(150, 40), sf::Vector2f(50, 160));
    taxBtn.setAction([this]()
                     {
        Player* p = game.get_current_player();
        p->tax();
        p->set_last_action("tax");
        actionMessage = p->get_name() + " performed Tax. Coins: " + std::to_string(p->get_coins());
        inGameError.clear(); });
    buttons.push_back(taxBtn);

    Button bribeBtn("Bribe", font, sf::Vector2f(150, 40), sf::Vector2f(50, 220));
    bribeBtn.setAction([this]()
                       {
        Player* p = game.get_current_player();
        p->bribe();
        p->set_last_action("bribe");
        actionMessage = p->get_name() + " performed Bribe. Coins: " + std::to_string(p->get_coins());
        inGameError.clear(); });
    buttons.push_back(bribeBtn);

    Button arrestBtn("Arrest", font, sf::Vector2f(150, 40), sf::Vector2f(50, 280));
    arrestBtn.setAction([this]()
                        { showTargetSelection([this](Player *target)
                                              {
            Player* p = game.get_current_player();
            p->arrest(*target);
            p->set_last_action("arrest");
            actionMessage = p->get_name() + " arrested " + target->get_name();
            inGameError.clear(); }, false, game.get_players()); });

    buttons.push_back(arrestBtn);

    Button sanctionBtn("Sanction", font, sf::Vector2f(150, 40), sf::Vector2f(50, 340));
    sanctionBtn.setAction([this]()
                          { showTargetSelection([this](Player *target)
                                                {
            Player* p = game.get_current_player();
            p->sanction(*target);
            p->set_last_action("sanction");
            actionMessage = p->get_name() + " sanctioned " + target->get_name();
            inGameError.clear(); }, false, game.get_players()); });
    buttons.push_back(sanctionBtn);

    Button coupBtn("Coup", font, sf::Vector2f(150, 40), sf::Vector2f(50, 400));
    coupBtn.setAction([this]()
                      { showTargetSelection([this](Player *target)
                                            {
            Player* p = game.get_current_player();
            p->coup(*target);
            p->set_last_action("coup");
            actionMessage = p->get_name() + " couped " + target->get_name();
            inGameError.clear(); }, false, game.get_players()); });
    buttons.push_back(coupBtn);

    // Role-specific action: invest for Baron
    if (role == "Baron")
    {
        Button investBtn("Invest", font, sf::Vector2f(150, 40), sf::Vector2f(50, 460));
        investBtn.setAction([this]()
                            {
            Player* p = game.get_current_player();
            if (p->must_coup()) throw MustPerformCoupException();
            static_cast<Baron*>(p)->invest();
            p->set_last_action("invest");
            inGameError.clear(); });
        buttons.push_back(investBtn);
    }

    // Special actions - top right corner

    // Undo Tax (by any active Governor)
    Button undoTaxBtn("Undo Tax", font, sf::Vector2f(180, 35), sf::Vector2f(800, 50));
    undoTaxBtn.setAction([this]()
                         {
    for (Player* p : game.get_players()) {
        if (p->is_eliminated()) continue;
        if (p->role() == "Governor") {
            for (Player* other : game.get_players()) {
                if (other != p && other->get_last_action() == "tax") {
                    static_cast<Governor*>(p)->undo_tax(*other);
                    actionMessage = p->get_name() + " canceled " + other->get_name() + "'s tax.";
                    inGameError.clear();
                    return;
                }
            }
            inGameError = "No player found who performed tax.";
            return;
        }
    }
    inGameError = "No Governor available to undo tax."; });
    buttons.push_back(undoTaxBtn);

    // Undo Bribe (by any active Judge)
    Button undoBribeBtn("Undo Bribe", font, sf::Vector2f(180, 35), sf::Vector2f(800, 100));
    undoBribeBtn.setAction([this]()
                           {
    for (Player* p : game.get_players()) {
        if (p->is_eliminated()) continue;
        if (p->role() == "Judge") {
            for (Player* other : game.get_players()) {
                if (other != p) {
                    static_cast<Judge*>(p)->undo_bribe(*other);
                    actionMessage = p->get_name() + " canceled " + other->get_name() + "'s bribe.";
                    inGameError.clear();
                    return;
                }
            }
            inGameError = "No player found who performed bribe.";
            return;
        }
    }
    inGameError = "No Judge available to undo bribe."; });
    buttons.push_back(undoBribeBtn);

    // Peek & Arrest (by any active Spy)
    Button peekBtn("Peek & Arrest", font, sf::Vector2f(180, 35), sf::Vector2f(800, 150));
    peekBtn.setAction([this]()
                      {
    for (Player* p : game.get_players()) {
        if (p->is_eliminated()) continue;
        if (p->role() == "Spy") {
            for (Player* other : game.get_players()) {
                if (other != p && !other->is_eliminated()) {
                    static_cast<Spy*>(p)->peek_and_arrest(*other);
                    actionMessage = p->get_name() + " peeked and arrested " + other->get_name();
                    inGameError.clear();
                    return;
                }
            }
            inGameError = "No valid target for Spy.";
            return;
        }
    }
    inGameError = "No Spy available to perform Peek & Arrest."; });
    buttons.push_back(peekBtn);

    // Undo Coup (by any active General)
    Button undoCoupBtn("Undo Coup", font, sf::Vector2f(180, 35), sf::Vector2f(800, 200));
    undoCoupBtn.setAction([this]()
                          {
    for (Player* p : game.get_players()) {
        if (p->role() == "General") {
            std::vector<Player*> targets;
            for (const auto& entry : game.get_coup_list()) {
                const std::string& target_name = entry.second;
                try {
                    Player* target = game.get_player(target_name);
                    targets.push_back(target);
                } catch (const PlayerNotFoundException&) {}
            }

            if (targets.empty()) {
                inGameError = "No couped targets to undo.";
                return;
            }

            showTargetSelection(
                [this, p](Player* target) {
                    static_cast<General*>(p)->undo_coup(*p, *target);
                    actionMessage = p->get_name() + " undid coup on " + target->get_name();
                    inGameError.clear();
                },
                false,
                targets
            );
            return;
        }
    }
    inGameError = "No General available to undo coup."; });
    buttons.push_back(undoCoupBtn);

    Button newGameBtn("New Game", font, sf::Vector2f(150, 40), sf::Vector2f(800, 650));
    newGameBtn.setAction([this]()
                         {
        game = Game();
        tempNames.clear();
        tempRoles.clear();
        buttons.clear();
        state = GUIState::Setup; });
    buttons.push_back(newGameBtn);
}

void GameGUI::drawPlayerList()
{
    float startY = 500;
    for (const Player *p : game.get_players())
    {
        sf::Text info;
        info.setFont(font);
        info.setCharacterSize(16);

        std::string label = p->get_name() + " - " + p->role();
        if (p->is_eliminated())
        {
            info.setFillColor(sf::Color(150, 150, 150));
        }
        else if (p->is_sanctioned() && p->is_disable_to_arrest())
        {
            info.setFillColor(sf::Color(255, 100, 0));
            label += " [SANCTIONED, DISABLE TO ARREST]";
        }
        else if (p->is_sanctioned())
        {
            info.setFillColor(sf::Color(255, 140, 0));
            label += " [SANCTIONED]";
        }
        else if (p->is_disable_to_arrest())
        {
            info.setFillColor(sf::Color(200, 0, 0));
            label += " [DISABLE TO ARREST]";
        }
        else
        {
            info.setFillColor(sf::Color::White);
        }

        info.setString(label);
        info.setPosition(750, startY);
        window.draw(info);
        startY += 25;
    }
}
