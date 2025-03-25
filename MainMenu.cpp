#include "MainMenu.h"
#include "MapSelectMenu.h"
#include "HighScoresBoard.h"
#include "SettingsMenu.h"
#include "ResourceHandler.h"

using namespace std;
using namespace rtd;
using namespace sf;
using rtd::resourceHandler;

MainMenu::MainMenu() : Menu({ "JUGAR", "PUNTAJES", "AJUSTES", "SALIR" }, 237)
{
    extern ResourceHandler resourceHandler;
    setMusic();
    setCanReturn(false);
    m_titleTexture = resourceHandler.getGameTitleImage();
    setTitleSprite(*m_titleTexture);
}

void MainMenu::select(Game& game)
{
    setItemWasSelected(false); //Para evitar que se ejecute mas de una vez, ya que al ejecutarse en el update,
                               //si queda en true, se ejecuta repetidamente hasta cambiar de escena
    switch (getSelectorIndex())
    {
    case 0://Jugar
        game.setScene(new MapSelectMenu());
        break;

    case 1://Puntuaciones
        game.setScene(new HighScoresBoard());
        break;

    case 2://Ajustes
        game.setScene(new SettingsMenu());
        break;

    case 3://Salir
        game.quit();
        break;

    default://Error
        throw runtime_error("Se seleccionó una opción fuera del rango del menú.");
        break;
    }
}

MainMenu::~MainMenu()
{}