#pragma once
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>
#include "Game.h"
#include "MenuButton.h"

namespace rtd {

    class Menu : public rtd::Scene
    {
        friend class ListMenu;

        sf::SoundBuffer m_scrollSoundBuffer;
        sf::Sound m_scrollSound;
        sf::Sprite m_backSprite;
        sf::Texture* m_backgroundTexture = nullptr;
        sf::Sprite m_backgroundSprite;
        bool m_has_default_bg = false;

        sf::Text m_title[2];
        sf::Font* m_fontTitle = nullptr;
        sf::Color m_titleColor = { 255,220,45 };
        unsigned int m_font_size = 75;

        sf::Sprite m_titleSprite;
        bool m_use_sprite_title = false;

        int m_title_ypos = 120;
        sf::CircleShape m_selector;

        std::vector<MenuButton> m_buttons;
        sf::Font* m_fontButton = nullptr;
        const int m_btn_distance = 34;

        void moveUp();
        void moveDown();
        void setSounds();
        virtual void setSelector(int index);
        virtual bool setFocus(int btn_index);
        void setTitleSize(int size);
        void setTitleY(float y);
        void setButtonsYPos(float pos);
        int m_selector_index = 0;
        bool m_item_was_selected = false;
        bool m_escape_was_pressed = false;
        bool m_can_return = true;

    public:
        Menu(std::vector<std::string> buttonTexts, float buttons_start_y);
        virtual ~Menu();

        virtual void processEvents(sf::Event event) override;
        virtual void draw(sf::RenderWindow& window) override;
        virtual void update(const float& delta_time, rtd::Game& game) override;
        virtual void select(rtd::Game& game) = 0;

        void setTitleText(std::string title);
        void setTitleSprite(sf::Texture& texture);
        void setBackground(sf::Texture* bgTexture = nullptr);
        void setSoundsVolume(float volume);
        void setMusic() const;
        void setCanReturn(bool value);
        void setEscapeWasPressed(bool value);
        void setItemWasSelected(bool value);
        bool escapeWasPressed() const;
        bool itemWasSelected() const;
        bool canReturn() const;
        int getSelectorIndex() const;
        const rtd::MenuButton& getMenuButton(unsigned int index) const;
        std::vector<sf::Drawable*> getDrawables();
    };

}