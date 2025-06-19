#include "GameGUI.hpp"
#include "Player.hpp"
#include "exceptions.hpp"
#include "Governor.hpp"
#include "Spy.hpp"
#include "Baron.hpp"
#include "General.hpp"
#include "Judge.hpp"
#include "Merchant.hpp"
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <ctime>

using namespace coup;
using namespace sf;

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

        if (std::find(tempNames.begin(), tempNames.end(), name) != tempNames.end()) {
            setupError = "Name already exists. Please choose a different one.";
            nameBox->clear();
            nameBox->setSelected(true);
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
                else if (state == GUIState::InGame && !showVictory)
                {
                    for (Button &btn : buttons)
                    {
                        if (btn.contains(x, y))
                        {
                            try
                            {
                                btn.execute();
                                buttons.clear();
                                setupButtons();
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
                else if (showVictory)
                {
                    for (Button &btn : buttons)
                    {
                        if (btn.getLabel() == "New Game" && btn.contains(x, y))
                        {
                            btn.execute();
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

        // ✅ בדיקת ניצחון רק אם עוד לא זוהה ניצחון
        if (!showVictory)
        {
            try
            {
                std::string winner = game.winner();
                winnerMessage = " The winner is: " + winner + "!";
                showVictory = true;
            }
            catch (...)
            {
                showVictory = false;
                winnerMessage.clear();
            }
        }

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

            drawPlayerList();

            if (showVictory)
            {
                sf::Text victoryText;
                victoryText.setFont(font);
                victoryText.setCharacterSize(28);
                victoryText.setFillColor(sf::Color::Green);
                victoryText.setStyle(sf::Text::Bold);
                victoryText.setString(winnerMessage);
                victoryText.setPosition(250, 300);
                window.draw(victoryText);

                for (const Button &btn : buttons)
                {
                    if (btn.getLabel() == "New Game")
                    {
                        btn.draw(window);
                        break;
                    }
                }
            }
            else if (state == GUIState::InGame)
            {
                for (const Button &btn : buttons)
                    btn.draw(window);
            }

            if (state == GUIState::TargetSelection)
            {
                for (const Button &btn : targetButtons)
                    btn.draw(window);
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

std::string GameGUI::randomRole()
{
    std::vector<std::string> roles = {"Governor", "Spy", "Baron", "General", "Judge", "Merchant"};
    return roles[rand() % roles.size()];
}
