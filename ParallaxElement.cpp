#include <string>
#include "ParallaxElement.h"
#include "Constants.h"

using namespace std;
using namespace rtd;
using namespace sf;

ParallaxElement::ParallaxElement()
{
    m_texture.setSmooth(true);
}

void ParallaxElement::setTextureFromFile(const string& path)
{
    if (!m_texture.loadFromFile(path))
        throw runtime_error("Ocurrió un error al intentar leer el archivo '" + path + "'");

    m_sprite.setTexture(m_texture);
    m_texture.setSmooth(true);
    m_sprite.scale(.5, .5);
    m_sprite.setOrigin(0, m_sprite.getLocalBounds().height);

    if (m_endPos <= m_begin)
        m_endPos = m_begin + m_texture.getSize().x;
}

void ParallaxElement::setRepeat(bool value)
{
    m_repeat = value;
}

void ParallaxElement::setRepeatSpace(float value)
{
    m_repeat_space = value;
}

void ParallaxElement::setZ(float value)
{
    m_z = value;
}

void ParallaxElement::setY(int value)
{
    m_y = value;
}

void ParallaxElement::setBegin(int value)
{
    m_begin = value;
    if (m_endPos <= m_begin)
        m_endPos = m_begin + m_texture.getSize().x;
}

void ParallaxElement::setEnd(int value)
{
    m_endPos = value;
    if (m_endPos <= m_begin)
        m_endPos = m_begin + m_texture.getSize().x;
}

void ParallaxElement::draw(RenderWindow& window, float screen_offset)
{
    if ((!m_repeat && screen_offset > m_endPos) ||
        screen_offset + WIN_WIDTH < m_begin)
        return;

    float sprite_width = (m_sprite.getGlobalBounds().width == 0) ? m_texture.getSize().x : m_sprite.getGlobalBounds().width;
    auto textureRect = m_sprite.getTextureRect();

    float sprite_offset = screen_offset / m_z;
    Vector2f drawPos = {m_begin - sprite_offset, WIN_HEIGHT - m_y};

    if (m_repeat)
        while (drawPos.x < m_endPos && drawPos.x < WIN_WIDTH) {
            m_sprite.setPosition(drawPos);
            /*
            if (screen_offset + drawPos.x + sprite_width > m_endPos) {
                auto rect = textureRect;
                rect.width = m_endPos - screen_offset - drawPos.x;
                m_sprite.setTextureRect(rect);
            }*/

            window.draw(m_sprite);
            m_sprite.setTextureRect(textureRect);
            //float width_in_screen = drawPos.x + m_sprite.getGlobalBounds().width - m_sprite.getGlobalBounds().left;
            drawPos.x += m_sprite.getGlobalBounds().width + m_repeat_space;
        }
    else {
        m_sprite.setPosition(drawPos);
        window.draw(m_sprite);
    }
}

bool ParallaxElement::operator>(const ParallaxElement& p) const
{
    return this->m_z > p.m_z;
}

bool rtd::compareParallaxPointersWithGreaterOperator(rtd::ParallaxElement* a, rtd::ParallaxElement* b)
{
    return *a > *b;
}