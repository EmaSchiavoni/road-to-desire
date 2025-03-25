#include "PlayMenu.h"
#include "MapSelectMenu.h"
#include "NewGameMenu.h"
#include "LoadGameMenu.h"

using std::runtime_error;
using namespace rtd;

PlayMenu::PlayMenu() :
    Menu({ " NUEVA PARTIDA", "CARGAR PARTIDA", "VOLVER" }, 237)
{
    setMusic();
    setTitleText("JUGAR");
}

void PlayMenu::update(const float& delta_time, Game& game)
{
    if (itemWasSelected())
        select(game);

    if (escapeWasPressed())
        game.setScene(new MapSelectMenu());
}

void PlayMenu::select(Game& game)
{
    setItemWasSelected(false);

    switch (getSelectorIndex())
    {
    case 0: {//NUEVA PARTIDA
        game.setScene(new NewGameMenu());
        break;
    }
    case 1://CARGAR PARTIDA
        game.setScene(new LoadGameMenu());
        break;
    case 2://VOLVER
        game.setScene(new MapSelectMenu());
        break;
    default://ERROR
        throw runtime_error("Se seleccionó una opción fuera del rango del menú.");
        break;
    }
}