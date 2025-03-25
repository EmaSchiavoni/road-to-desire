#include "LevelMenu.h"
#include "MapFile.h"
#include "Settings.h"
#include "SettingsMenu.h"

using namespace std;
using namespace rtd;
using namespace sf;

LevelMenu::LevelMenu(Level* level, Texture* levelBackground) :
    Menu({ "REANUDAR", "AJUSTES", "SALIR AL MAPA", "SALIR DEL JUEGO" }, 237)
{
    m_blackBg.setFillColor({ 0,0,0,125 });
    m_blackBg.setSize({WIN_WIDTH, WIN_HEIGHT});

    m_level = level;
    Menu::setBackground(levelBackground);
    setTitleText("PAUSA");
}

LevelMenu::~LevelMenu()
{
    if (m_settingsMenu != nullptr)
        delete m_settingsMenu;
}

void LevelMenu::processEvents(Event event)
{
    if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) {
        if (m_settingsMenu != nullptr)
            closeSettings();
        else
            m_level->unpause();
        return;
    }

    if (m_settingsMenu != nullptr)
        m_settingsMenu->processEvents(event);
    else
        Menu::processEvents(event);
}

void LevelMenu::openSettings()
{
    m_settingsMenu = new SettingsMenu();
}

void LevelMenu::closeSettings()
{
    extern Settings settings;
    if (m_settingsMenu != nullptr) {
        m_settingsMenu->save();
        m_level->setSoundsVolume(settings.getSoundsVolume());
        setSoundsVolume(settings.getSoundsVolume());
        delete m_settingsMenu;
        m_settingsMenu = nullptr;
    }
}

void LevelMenu::update(const float& delta_time, rtd::Game& game)
{
    if (m_settingsMenu != nullptr) {
        m_settingsMenu->update(delta_time, game);
        return;
    }
    if (itemWasSelected())
        select(game);
}

void LevelMenu::select(Game& game)
{
    setItemWasSelected(false);

    switch (getSelectorIndex())
    {
    case 0: {//Reanudar
        m_level->unpause();
        break;
    }
    case 1://Ajustes
        openSettings();
        break;
    case 2: {//Salir al mapa
        game.setScene( MapFile().getMap() );
        break;
    }
    case 3: {//Salir del juego
        game.quit();
        break;
    }
    default://Error
        throw runtime_error("Error: la opción seleccionada esta fuera del rango del menú.");
        break;
    }
}

void LevelMenu::draw(RenderWindow& window)
{
    if (m_settingsMenu != nullptr) {
        m_settingsMenu->draw(window);
        return;
    }

    vector<Drawable*> drawables = getDrawables();

    window.draw(m_blackBg);

    for (size_t i = 1; i < drawables.size(); ++i) //Desde 1 para no dibujar el bg
        window.draw(*drawables[i]);

    window.display();
}