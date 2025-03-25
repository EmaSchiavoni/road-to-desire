#pragma once
#include "Menu.h"
#include "TextInput.h"
#include "Profile.h"

namespace rtd {

    class NewGameMenu : public rtd::Menu
    {
        rtd::TextInput m_playerNameInput;
        bool validatePlayerName();

    public:
        NewGameMenu();
        void update(const float& delta_time, rtd::Game& game) override;
        void select(rtd::Game& game);
        void processEvents(sf::Event event) override;
        void draw(sf::RenderWindow& window) override;
        ~NewGameMenu();
    };

}