#pragma once
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

namespace rtd {

    class ParallaxElement
    {
        sf::Texture m_texture;
        sf::Sprite m_sprite;
        bool m_repeat = false;
        float m_repeat_space = 0;
        float m_z = 0;
        int m_y = 0; //Y = 0 es el borde inferior de la pantalla, contrario a las coordenadas;
        float m_begin = 0;
        float m_endPos = 0;

    public:
        ParallaxElement();
        void setTextureFromFile(const std::string& path);
        void setRepeat(bool value);
        void setRepeatSpace(float value);
        void setZ(float value);
        void setY(int value);
        void setBegin(int value);
        void setEnd(int value);
        void draw(sf::RenderWindow& window, float screen_offset);

        bool operator>(const rtd::ParallaxElement& p) const;
    };

    bool compareParallaxPointersWithGreaterOperator(rtd::ParallaxElement* a, rtd::ParallaxElement* b);

}