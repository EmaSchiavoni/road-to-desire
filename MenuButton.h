#pragma once
#include <string>
#include <vector>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

namespace rtd {

    class MenuButton
    {
        sf::Text m_text;
        sf::Text m_shadow;
        sf::Color m_textBlurColor = { 240, 240, 240 };
        sf::Color m_textFocusColor = { 255, 255, 255 };
        sf::Color m_shadowBlurColor = { 0, 0, 0, 155 };
        sf::Color m_shadowFocusColor = { 0, 0, 0, 255 };

    public:
        MenuButton(std::string content, sf::Font& font, unsigned int font_size, float position_y);
        std::string getString() const;
        sf::Vector2f getPosition() const;
        sf::FloatRect getGlobalBounds() const;
        void setPosition(float x, float y);
        void select();
        void focus();
        void blur();
        void draw(sf::RenderWindow& window);
        std::vector<sf::Drawable*> getDrawables();
    };

}