#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>
#include "ListMenu.h"
#include "Constants.h"
#include "ResourceHandler.h"

using namespace std;
using namespace rtd;
using namespace sf;
using rtd::resourceHandler;

ListMenu::ListMenu(vector<string> buttonTexts) :
    Menu(buttonTexts, WIN_HEIGHT/2),
    m_arrowUp(6, 3),
    m_arrowDown(6, 3)
{
    setNoElementsText();
    setArrows();
    setSounds();

    if (buttonTexts.size() >= 3)
        setFocus(1);
}

void ListMenu::setArrows()
{
    for (int i = 0; i < 2; ++i) {
        CircleShape* arrow = (i == 1)? &m_arrowDown : &m_arrowUp;
        arrow->setFillColor({ 255, 255, 255, 230 });
        arrow->setRotation(i*180.f);
        arrow->setScale(1.f, .6f);
        arrow->setOrigin(arrow->getLocalBounds().width / 2, 0);
    }
    m_arrowDown.setPosition(WIN_WIDTH / 2, m_y_pos + 70);
    m_arrowUp.setPosition(WIN_WIDTH / 2, m_y_pos - 60);
}

void ListMenu::setNoElementsText()
{
    extern ResourceHandler resourceHandler;
    noElementsText.setFillColor({ 255,255,255,200 });
    noElementsText.setOutlineColor({ 0,0,0,180 });
    noElementsText.setOutlineThickness(1.5);
    noElementsText.setFont(*resourceHandler.getMenuFont());
    noElementsText.setCharacterSize(m_font_size / 2);
    noElementsText.setScale(.5, .5);
    noElementsText.setString("No hay elementos");
    noElementsText.setOrigin(noElementsText.getLocalBounds().width / 2, noElementsText.getLocalBounds().height);
    noElementsText.setPosition(WIN_WIDTH / 2, WIN_HEIGHT / 2);
}

void ListMenu::setButtonTexts(const vector<string>& buttonTexts)
{
    Menu::m_buttons = {};

    for (size_t i = 0; i < buttonTexts.size(); ++i)
        Menu::m_buttons.push_back(MenuButton(buttonTexts[i], *m_fontButton, (int)floor(m_font_size / 3.3), 0));

    if (buttonTexts.size() >= 3)
        setFocus(1);
    else if(buttonTexts.size() > 0)
        setFocus(0);
}

string ListMenu::getCurrentButtonText() const
{
    if (m_buttons.size() == 0)
        return "";
    else
        return Menu::m_buttons[getSelectorIndex()].getString();
}

void ListMenu::drawButtons(RenderWindow& window)
{
    int selector_index = getSelectorIndex();
    m_buttons[selector_index].draw(window);
    window.draw(m_selector);

    if (selector_index > 0)
        m_buttons[selector_index - 1].draw(window);

    if (selector_index < m_buttons.size() - 1)
        m_buttons[selector_index + 1].draw(window);

    if (selector_index + 2 <= m_buttons.size() - 1)
        window.draw(m_arrowDown);

    if (selector_index - 2 >= 0)
        window.draw(m_arrowUp);
}

void ListMenu::draw(RenderWindow& window)
{
    window.clear({ 0,0,0 });
    window.draw(m_backgroundSprite);
    if (canReturn())
        window.draw(m_backSprite);

    if (m_use_sprite_title)
        window.draw(m_titleSprite);
    else
        for (int i = 0; i < 2; ++i)
            window.draw(m_title[i]);

    if (m_buttons.size() > 0)
        drawButtons(window);
    else
        window.draw(noElementsText);

    window.display();
}

bool ListMenu::setFocus(int btn_index)
{
    if (btn_index < 0 || btn_index > (int)m_buttons.size() - 1)
        return false;

    int ex_selector_index = getSelectorIndex();

    m_buttons[ex_selector_index].blur();
    m_selector_index = btn_index;
    m_selector.setPosition(m_buttons[m_selector_index].getGlobalBounds().left - 30, m_y_pos + 4);
    m_buttons[m_selector_index].focus();
    m_buttons[m_selector_index].setPosition(WIN_WIDTH / 2, m_y_pos);

    if (m_selector_index > 0)
        m_buttons[m_selector_index-1].setPosition(WIN_WIDTH / 2, m_y_pos - m_btn_distance);

    if (m_selector_index < m_buttons.size()-1)
        m_buttons[m_selector_index+1].setPosition(WIN_WIDTH / 2, m_y_pos + m_btn_distance);

    return true;
}