#include <fstream>
#include "MapFile.h"
#include "Constants.h"
#include "Profile.h"

using namespace std;
using namespace rtd;

extern Profile currentProfile;

MapFile::MapFile() : FileReader("Maps and levels/" + string(currentProfile.getMapName()) + "/map.rdmap")
{
    if (!readFile())
        throw runtime_error("ERROR FATAL: el archivo del mapa no existe o está corrupto.");
    processData();
    closeFile();
}

Scene* MapFile::getMap()
{
    Scene* mapScene = new Map(m_map);
    return mapScene;
}

void MapFile::processData()
{
    readParts();
    readLevels();
}

void MapFile::readParts()
{
    m_map.setPartsQuantity( m_json["parts"].asInt() );
}

void MapFile::readLevels()
{
    if (m_json["levels"].size() > MAP_MAX_LEVELS_COUNT)
        throw runtime_error("La cantidad de niveles del mapa supera el máximo permitido.");

    int n = 0;
    for (const Json::Value& level : m_json["levels"])
    {
        readLevelPartAndPosition(level);
        readLevelNeighbors(level, n++);
    }
}

void MapFile::readLevelPartAndPosition(const Json::Value& level)
{
    m_map.addLevel(
        MapLevel(
            level["part"].asInt(),
            { level["position"]["x"].asFloat(), level["position"]["y"].asFloat() }
        )
    );
}

void MapFile::readLevelNeighbors(const Json::Value& level, int lvl_index)
{
    m_map.getLevelStructure(lvl_index).setNeighbors(
        map<Ubication, int>{
            { Ubication::top,
                (level["neighbors"]["top"].isNull())? -1 : level["neighbors"]["top"].asInt() },

            { Ubication::right,
                (level["neighbors"]["right"].isNull())? -1 : level["neighbors"]["right"].asInt() },

            { Ubication::bottom,
                (level["neighbors"]["bottom"].isNull())? -1 : level["neighbors"]["bottom"].asInt() },

            { Ubication::left,
                (level["neighbors"]["left"].isNull())? -1 : level["neighbors"]["left"].asInt() },

        }
    );
}