#include <fstream>
#include "MapSelectMenu.h"
#include "MainMenu.h"
#include "PlayMenu.h"
#include "Profile.h"

using namespace std;
using namespace rtd;
using namespace sf;

MapSelectMenu::MapSelectMenu() : ListMenu({})
{
    setTitleText("Mapa");
    setButtonTexts(getMapList());
}

vector<string> MapSelectMenu::getMapList() const
{
    vector<string> mapList;
    ifstream file("Maps and levels/map_list.txt");

    if (file.fail())
        throw runtime_error("Ocurrió un error al intentar leer la lista de mapas.");

    string map_name;

    while (getline(file, map_name))
        mapList.push_back(map_name);

    return mapList;
}

void MapSelectMenu::update(const float& delta_time, rtd::Game& game)
{
    if (itemWasSelected()) {
        extern Profile currentProfile;
        currentProfile.setMap( getCurrentButtonText() );
        game.setScene(new PlayMenu());
        setItemWasSelected(false);
    }

    if (escapeWasPressed())
        game.setScene(new MainMenu());
}

void MapSelectMenu::select(Game& game)
{}