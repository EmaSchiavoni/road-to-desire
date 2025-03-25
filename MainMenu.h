#pragma once
#include <vector>
#include <SFML/Graphics/Text.hpp>
#include "Menu.h"

namespace rtd {

    class MainMenu : public rtd::Menu
    {
        sf::Texture* m_titleTexture;

    public:
        MainMenu();
        void select(rtd::Game& game) override;
        ~MainMenu();
    };

}