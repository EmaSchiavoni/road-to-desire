#include <vector>
#include "SettingsMenu.h"
#include "MainMenu.h"
#include "Settings.h"
#include "ResourceHandler.h"

using namespace std;
using namespace rtd;
using namespace sf;
using rtd::resourceHandler;

extern ResourceHandler resourceHandler;
extern Settings settings;

SettingsMenu::SettingsMenu() :
    Menu({"VOLUMEN DE LA MUSICA", "VOLUMEN DE LOS SONIDOS" }, 247),
    m_musicVolumeInput(to_string((int)ceil(settings.getMusicVolume())), { 30, 20 }, *resourceHandler.getThinFont(), 64, { 0,0,0 }),
    m_soundsVolumeInput(to_string((int)ceil(settings.getSoundsVolume())), { 30, 20 }, *resourceHandler.getThinFont(), 64, { 0,0,0 })
{
    setTitleText("AJUSTES");

    m_inputs[0] = &m_musicVolumeInput;
    m_inputs[1] = &m_soundsVolumeInput;

    auto& musicButton = getMenuButton(0);
    m_musicVolumeInput.setOrigin(0, m_musicVolumeInput.getLocalBounds().top);
    m_musicVolumeInput.setPosition({
        musicButton.getGlobalBounds().left + musicButton.getGlobalBounds().width + 20,
        musicButton.getGlobalBounds().top - 1
    });

    auto& soundButton = getMenuButton(1);
    m_soundsVolumeInput.setOrigin(0, m_soundsVolumeInput.getLocalBounds().top);
    m_soundsVolumeInput.setPosition({
        soundButton.getGlobalBounds().left + soundButton.getGlobalBounds().width + 20,
        soundButton.getGlobalBounds().top - 1
    });
}

int SettingsMenu::getInputValue(TextInput& input) const
{
    return stoi(string(input.getString()));
}

void SettingsMenu::increaseCurrentInput()
{
    int val = getInputValue(*m_inputs[getSelectorIndex()]);
    if (++val <= 100)
        m_inputs[getSelectorIndex()]->setString(to_string(val));
}

void SettingsMenu::reduceCurrentInput()
{
    int val = getInputValue(*m_inputs[getSelectorIndex()]);
    if (--val >= 0)
        m_inputs[getSelectorIndex()]->setString(to_string(val));
}

void SettingsMenu::sanitizeInput(TextInput& input)
{
    if (input.getString().isEmpty())
        input.setString(to_string(0));

    if (getInputValue(input) > 100)
        input.setString(to_string(100));
}

void SettingsMenu::sanitizeAllInputs()
{
    for (int i = 0; i < m_inputs_count; ++i)
        sanitizeInput(*m_inputs[i]);
}

SettingsMenu::~SettingsMenu()
{
    sanitizeAllInputs();
    save();

    for (int i = 0; i < m_inputs_count; ++i)
        resourceHandler.unloadThinFont();
}

void SettingsMenu::update(const float& delta_time, Game& game)
{
    if (itemWasSelected())
        select(game);

    if (escapeWasPressed())
        game.setScene(new MainMenu());
}

void SettingsMenu::select(Game& game)
{

}

void SettingsMenu::processEvents(Event event)
{
    Menu::processEvents(event);
    switch (event.type) {

    case Event::KeyPressed: {
        switch (event.key.code)
        {
        case Keyboard::Escape:
            save();
            break;

        case Keyboard::Right:
            increaseCurrentInput();
            break;

        case Keyboard::Left:
            reduceCurrentInput();
            break;

        case Keyboard::Up:
        case Keyboard::Down:
            sanitizeAllInputs();
            break;
        }
        break;
    }

    case Event::TextEntered:
        if ((event.text.unicode != 8 && m_inputs[getSelectorIndex()]->getStringSize() == 3) ||
            (event.text.unicode != 8 && (event.text.unicode < 48 || event.text.unicode > 57)) )
            break;

            m_inputs[getSelectorIndex()]->input(event.text.unicode);

        break;
    }
}

void SettingsMenu::draw(sf::RenderWindow& window)
{
    vector<Drawable*> drawables = getDrawables();

    window.clear({ 0,0,0 });

    for (size_t i = 0; i < drawables.size(); ++i)
        window.draw(*drawables[i]);

    m_musicVolumeInput.draw(window);
    m_soundsVolumeInput.draw(window);

    window.display();
}

void SettingsMenu::save()
{
    settings.setMusicVolume(getInputValue(m_musicVolumeInput));
    settings.setSoundsVolume(getInputValue(m_soundsVolumeInput));
}