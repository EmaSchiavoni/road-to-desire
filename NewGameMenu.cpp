#include <vector>
#include "NewGameMenu.h"
#include "PlayMenu.h"
#include "Map.h"
#include "MapFile.h"
#include "Profile.h"

using namespace std;
using namespace rtd;
using namespace sf;

extern ResourceHandler resourceHandler;

NewGameMenu::NewGameMenu() :
    Menu({"", "CREAR", "VOLVER "}, 247),
    m_playerNameInput("Nombre", {200, 30}, * resourceHandler.getThinFont(), 64, {0,0,0})
{
    setMusic();
    setTitleText("NUEVA PARTIDA");

    FloatRect inputBox = m_playerNameInput.getLocalBounds();
    m_playerNameInput.setOrigin(inputBox.width / 2, inputBox.top);
    m_playerNameInput.setPosition({ WIN_WIDTH / 2, 237 });
}

NewGameMenu::~NewGameMenu()
{
    resourceHandler.unloadThinFont();
}

bool NewGameMenu::validatePlayerName()
{
    string player_name = m_playerNameInput.getString();
    
    if (!player_name.empty())
        return true;
    else
        return false;
}

void NewGameMenu::update(const float& delta_time, Game& game)
{
    if (itemWasSelected())
        select(game);

    if (escapeWasPressed())
        game.setScene(new PlayMenu());
}

void NewGameMenu::select(Game& game)
{
    setItemWasSelected(false);
    
    switch (getSelectorIndex())
    {
    case 0://GUARDAR PERFIL E IR AL MAPA
    case 1:
        if (validatePlayerName()) {
            extern Profile currentProfile;

            if (currentProfile.loadFromFile(currentProfile.getMapName(), m_playerNameInput.getString().toAnsiString())) {
                //Mostrar dialog para confirmar el reset del perfil;
                break;
            }
            else {
                currentProfile.setMap(currentProfile.getMapName());
                currentProfile.setPlayer(m_playerNameInput.getString().toAnsiString());
                currentProfile.save();
            }
            game.setScene( MapFile().getMap() );
        }
        break;

    case 2://VOLVER
        game.setScene(new PlayMenu());
        break;

    default://ERROR
        throw runtime_error("Se seleccionó una opción fuera del rango del menú.");
        throw runtime_error("Se seleccionó una opción fuera del rango del menú.");
        break;
    }
}

void NewGameMenu::processEvents(Event event)
{
    Menu::processEvents(event);
    switch (event.type) {

    case Event::TextEntered:
        if(getSelectorIndex() == 0)
            m_playerNameInput.input(event.text.unicode);
        break;
    }
}

void NewGameMenu::draw(sf::RenderWindow& window)
{
    vector<Drawable*> drawables = getDrawables();

    window.clear({ 0,0,0 });

    for (size_t i = 0; i < drawables.size(); ++i)
        window.draw( *drawables[i]);
    
    m_playerNameInput.draw(window);

    window.display();
}