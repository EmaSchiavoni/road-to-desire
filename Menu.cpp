#include <vector>
#include "Menu.h"
#include "SoundsHandler.h"
#include "SoundPaths.h"
#include "ResourceHandler.h"
#include "Settings.h"

using namespace std;
using namespace rtd;
using namespace sf;

extern SoundsHandler globalSoundsHandler;
extern ResourceHandler resourceHandler;

Menu::Menu(vector<string> buttonTexts, float buttons_start_y) :
    m_selector(9, 3)
{
    m_backSprite.setTexture(*resourceHandler.getBackTexture());
    m_backSprite.setScale(.2f, .2f);
    m_backSprite.setPosition(0, WIN_HEIGHT - 40);
    m_backSprite.setColor({ 255,255,255,125 });

    m_fontButton = resourceHandler.getMenuFont();
    m_selector_index = 0;
    setBackground();

    for (size_t i = 0; i < buttonTexts.size(); ++i)
        m_buttons.push_back(MenuButton(buttonTexts[i], *m_fontButton, (int)floor(m_font_size / 3.3f), buttons_start_y));

    setSelector(0);
    setButtonsYPos(buttons_start_y);
    setFocus(0);
    setSounds();
}

Menu::~Menu()
{
    resourceHandler.unloadMenuFont();
    if (m_can_return)
        resourceHandler.unloadBackTexture();

    if (m_has_default_bg)
        resourceHandler.unloadMenuBackground();

    if (m_use_sprite_title)
        resourceHandler.unloadGameTitleImage();
    else
        resourceHandler.unloadTitleFont();
}

void Menu::moveUp()
{
    if (setFocus(getSelectorIndex() - 1))
        m_scrollSound.play();
}

void Menu::moveDown()
{
    if (setFocus(getSelectorIndex() + 1))
        m_scrollSound.play();
}

void Menu::setSounds()
{
    extern Settings settings;
    m_scrollSoundBuffer.loadFromFile(string(MENU_SOUNDS_PATH) + "scroll.ogg");
    m_scrollSound.setBuffer(m_scrollSoundBuffer);
    setSoundsVolume(settings.getSoundsVolume());
}

void Menu::setSelector(int index)
{
    m_selector_index = index;
    m_selector.setOrigin(m_selector.getLocalBounds().width / 2, m_selector.getLocalBounds().width);
    m_selector.setFillColor({ 255, 255, 255 });
    m_selector.setRotation(90);
    m_selector.setScale(1.f, .6f);
}

bool Menu::setFocus(int btn_index)
{
    if (btn_index < 0 || btn_index >(int)m_buttons.size() - 1)
        return false;

    m_buttons[getSelectorIndex()].blur();
    m_selector_index = btn_index;

    int selector_index = getSelectorIndex();

    m_selector.setPosition(
        m_buttons[selector_index].getGlobalBounds().left - 20,
        m_buttons[selector_index].getPosition().y + 4
    );
    m_buttons[selector_index].focus();
    return true;
}

void Menu::setTitleSize(int size)
{
    for (int i = 0; i < 2; ++i)
        m_title[i].setCharacterSize(size);
}

void Menu::setTitleY(float y)
{
    for (int i = 0; i < 2; ++i)
        m_title[i].setPosition(m_title[i].getPosition().x, y + i * 3);
}

void Menu::setButtonsYPos(float pos_y)
{
    for (size_t i = 0; i < m_buttons.size(); ++i)
        m_buttons[i].setPosition(WIN_WIDTH / 2, pos_y + (m_btn_distance * i));
}

void Menu::processEvents(Event event)
{
    switch (event.type) {

    case Event::KeyReleased:
        switch (event.key.code) {

        case Keyboard::Up:
            moveUp();
            break;

        case Keyboard::Down:
            moveDown();
            break;

        case Keyboard::Enter:
            setItemWasSelected(true);
            globalSoundsHandler.setSound(string(MENU_SOUNDS_PATH) + "select.ogg");
            break;

        case Keyboard::Escape:
            m_escape_was_pressed = true;
            break;
        }
        break;

    }
}

void Menu::draw(RenderWindow& window)
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

    for (size_t i = 0; i < m_buttons.size(); ++i)
        m_buttons[i].draw(window);

    window.draw(m_selector);
    window.display();
}

void Menu::update(const float& delta_time, Game& game)
{
    if (itemWasSelected())
        select(game);
}

void Menu::setSoundsVolume(float volume)
{
    m_scrollSound.setVolume(volume);
}

void Menu::setTitleText(string title)
{
    if (m_use_sprite_title) {
        resourceHandler.unloadGameTitleImage();
        m_use_sprite_title = false;
    }
    m_fontTitle = resourceHandler.getTitleFont();

    for (int i = 0; i < 2; ++i) {
        m_title[i].setFont(*m_fontTitle);
        m_title[i].setString(title);
        m_title[i].setCharacterSize(m_font_size * 2);
        m_title[i].setScale(.5f, .5f);

        sf::FloatRect titleBox = m_title[i].getLocalBounds();
        m_title[i].setOrigin(titleBox.width / 2.0f, titleBox.top);
        m_title[i].setPosition(WIN_WIDTH / 2 - i * 3.f, m_title_ypos - i * 5.f);
    }

    m_title[0].setFillColor({ 0,0,0 });
    m_title[1].setFillColor(m_titleColor);
}

void Menu::setTitleSprite(sf::Texture& texture)
{
    m_titleSprite.setTexture(texture);
    m_titleSprite.setScale(.5, .5);

    sf::FloatRect title_box = m_titleSprite.getLocalBounds();
    m_titleSprite.setOrigin(title_box.left + title_box.width / 2, title_box.top);
    m_titleSprite.setPosition(WIN_WIDTH / 2, m_title_ypos - 20.f);

    if (m_fontTitle != nullptr) {
        resourceHandler.unloadTitleFont();
        m_fontTitle = nullptr;
    }
    m_use_sprite_title = true;
}

void Menu::setBackground(Texture* bgTexture)
{
    if (bgTexture != nullptr && m_has_default_bg) {
        resourceHandler.unloadMenuBackground();
        m_has_default_bg = false;
    }

    if (bgTexture == nullptr)
        m_has_default_bg = true;

    Texture& texture = (bgTexture != nullptr) ? *bgTexture : *resourceHandler.getMenuBackground();
    float scale = WIN_WIDTH / texture.getSize().x;
    m_backgroundSprite.setTexture(texture);
    m_backgroundSprite.setScale(scale, scale);
    m_backgroundSprite.setPosition(0, 0);
}

void Menu::setMusic() const
{
    globalSoundsHandler.setMusic(string(MUSIC_PATH) + "menu.ogg");
}

void Menu::setCanReturn(bool value)
{
    if (!value && value != m_can_return)
        resourceHandler.unloadBackTexture();

    m_can_return = value;
}

void Menu::setEscapeWasPressed(bool value)
{
    m_escape_was_pressed = value;
}

void Menu::setItemWasSelected(bool value)
{
    m_item_was_selected = value;
}

bool Menu::escapeWasPressed() const
{
    return m_escape_was_pressed;
}

bool Menu::itemWasSelected() const
{
    return m_item_was_selected;
}

bool Menu::canReturn() const
{
    return m_can_return;
}

int Menu::getSelectorIndex() const
{
    return m_selector_index;
}

const MenuButton& Menu::getMenuButton(unsigned int index) const
{
    if (index > m_buttons.size())
        index = m_buttons.size() - 1;

    return m_buttons[index];
}

vector<Drawable*> Menu::getDrawables()
{
    vector<Drawable*> v = { &m_backgroundSprite };

    if (canReturn())
        v.push_back(&m_backSprite);

    if (m_use_sprite_title)
        v.push_back(&m_titleSprite);
    else
        for (int i = 0; i < 2; ++i)
            v.push_back(&m_title[i]);

    for (size_t i = 0; i < m_buttons.size(); ++i) {
        auto btn_v = m_buttons[i].getDrawables();
        for (size_t j = 0; j < btn_v.size(); ++j)
            v.push_back(btn_v[j]);
    }

    v.push_back(&m_selector);

    return v;
}