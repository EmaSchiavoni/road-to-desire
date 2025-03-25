#pragma once
#include "MapSelectMenu.h"

namespace rtd {

    class HighScoresBoard : public rtd::Scene
    {
        sf::Font* m_font = nullptr;
        unsigned int m_font_size = 60;
        float m_font_scale = .3f;
        sf::Color m_textColor = {255, 255, 255};
        sf::Text m_title;
        sf::Text m_text;
        sf::Text m_shadow;
        bool m_map_selected = false;
        bool m_escape_was_pressed = false;
        rtd::MapSelectMenu* m_mapMenu = nullptr;
        sf::Texture* m_backgroundTexture = nullptr;
        sf::Sprite m_backgroundSprite;
        sf::Sprite m_backSprite;

        void setTitle();
        void setBoardText();
        std::string getHighScoresString() const;

    public:
        HighScoresBoard();
        ~HighScoresBoard();
        void processEvents(sf::Event event) override;
        void draw(sf::RenderWindow& window) override;
        void update(const float& delta_time, rtd::Game& game) override;
    };

}