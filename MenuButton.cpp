#include "MenuButton.h"
#include "Constants.h"

namespace rtd {
    class Menu;
}

using namespace std;
using namespace rtd;
using namespace sf;

MenuButton::MenuButton(string content, sf::Font& font, unsigned int font_size, float position_y)
{
    m_text.setFont(font);
    m_text.setLetterSpacing(1.5);
    m_text.setScale(.45f, .45f);
    m_text.setCharacterSize(font_size * 2);
    m_text.setScale(.5f, .5f);
    m_text.setFillColor(m_textBlurColor);
    m_text.setString(content);
    m_text.setOrigin(m_text.getLocalBounds().width / 2, m_text.getLocalBounds().height / 2);
    m_text.setPosition(WIN_WIDTH / 2, position_y);

    m_shadow.setFont(font);
    m_shadow.setLetterSpacing(1.5f);
    m_shadow.setScale(.45f, .45f);
    m_shadow.setCharacterSize(font_size * 2);
    m_shadow.setScale(.5f, .5f);
    m_shadow.setFillColor(m_shadowBlurColor);
    m_shadow.setString(content);
    m_shadow.setOrigin(m_shadow.getLocalBounds().width / 2, m_shadow.getLocalBounds().height / 2);
    m_shadow.setPosition(WIN_WIDTH / 2 + 1, position_y + 1);
}

string MenuButton::getString() const
{
    return m_text.getString();
}

Vector2f MenuButton::getPosition() const
{
    return m_text.getPosition();
}

FloatRect MenuButton::getGlobalBounds() const
{
    return m_text.getGlobalBounds();
}

void MenuButton::setPosition(float x, float y)
{
    m_text.setPosition(x, y);
    m_shadow.setPosition(x + 1, y + 2);
}

void MenuButton::select()
{

}

void MenuButton::focus()
{
    m_text.setFillColor(m_textFocusColor);
    m_shadow.setFillColor(m_shadowFocusColor);
}

void MenuButton::blur()
{
    m_text.setFillColor(m_textBlurColor);
    m_shadow.setFillColor(m_shadowBlurColor);
}

void MenuButton::draw(RenderWindow& window) {
    window.draw(m_shadow);
    window.draw(m_text);
}

vector<Drawable*> MenuButton::getDrawables()
{
    return { &m_shadow, &m_text };
}