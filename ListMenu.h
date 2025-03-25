#pragma once
#include <vector>
#include "Menu.h"

namespace rtd {

    class ListMenu : public rtd::Menu
    {
        float m_y_pos = WIN_HEIGHT / 2 + 50;
        sf::Text noElementsText;
        sf::CircleShape m_arrowUp, m_arrowDown;

        void setNoElementsText();
        void setArrows();
        void drawButtons(sf::RenderWindow& window);

    protected:
        bool setFocus(int btn_index) override;
        void setButtonTexts(const std::vector<std::string>& buttonTexts);

    public:
        ListMenu(std::vector<std::string> buttonTexts);
        std::string getCurrentButtonText() const;
        void draw(sf::RenderWindow& window) override;
        void select(rtd::Game& game) = 0;
    };

}