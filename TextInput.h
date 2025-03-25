#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Event.hpp>
#include "Constants.h"

namespace rtd {

    class TextInput
    {
        sf::Text m_text;
        sf::RectangleShape m_inputBox;
        sf::RectangleShape m_outline[2];
        sf::CircleShape m_sides[2];
        bool m_align_center;
        const int max_characters = rtd::PLAYER_NAME_MAX_LENGTH;

        void setAlignCenter();
        void setAlignLeft();
    public:
        TextInput(std::string initialText, sf::Vector2f size, const sf::Font& font, unsigned int font_size, sf::Color background_color);
        void input(sf::Uint32 unicode_key_pressed);
        void alignCenter();
        void alignLeft();
        void setOutline(sf::Color color);
        void setColor(sf::Color color);
        void setBackground(sf::Color color);
        void setOrigin(float x, float y);
        void setPosition(sf::Vector2f pos);
        void setString(std::string string);
        sf::Vector2f getPosition() const;
        sf::FloatRect getLocalBounds() const;
        sf::String getString() const;
        std::size_t getStringSize() const;
        void draw(sf::RenderWindow& window);
        ~TextInput();
    };

}