#pragma once
#include "Menu.h"
#include "Level.h"
#include "SettingsMenu.h"

namespace rtd {

    class LevelMenu : public rtd::Menu
    {
        rtd::SettingsMenu* m_settingsMenu = nullptr;
        rtd::Level* m_level = nullptr;
        sf::Texture* m_background = nullptr;
        sf::RectangleShape m_blackBg;
        
        void openSettings();
        void closeSettings();

    public:
        LevelMenu(rtd::Level* level, sf::Texture* levelBackground);
        ~LevelMenu();
        void processEvents(sf::Event event) override;
        void update(const float& delta_time, rtd::Game& game) override;
        void draw(sf::RenderWindow& window) override;
        void select(rtd::Game& game) override;
    };

}