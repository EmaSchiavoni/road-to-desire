#include <fstream>
#include "LoadGameMenu.h"
#include "PlayMenu.h"
#include "MapFile.h"
#include "Decrypter.h"
#include "Profile.h"

using namespace std;
using namespace rtd;
using namespace sf;

extern Profile currentProfile;

LoadGameMenu::LoadGameMenu() : ListMenu({})
{
    setMusic();
    setTitleText("Cargar partida");

    setButtonTexts( currentProfile.getSavedProfileNames() );
}

void LoadGameMenu::update(const float& delta_time, rtd::Game& game)
{
    if (itemWasSelected()) {
        currentProfile.loadFromFile(currentProfile.getMapName(), getCurrentButtonText());
        game.setScene( MapFile().getMap() );
        setItemWasSelected(false);
    }

    if (escapeWasPressed())
        game.setScene(new PlayMenu());
}

void LoadGameMenu::select(Game& game)
{}