#include <fstream>
#include <SFML/Graphics/Rect.hpp>
#include "LevelFile.h"
#include "Collectable.h"
#include "Coin.h"
#include "Constants.h"
#include "DragonBall.h"
#include "Hitbox.h"

using namespace std;
using namespace rtd;
using namespace sf;
using rtd::resourceHandler;

LevelFile::LevelFile(string map_name, int lvl_index) :
    FileReader("Maps and levels/" + map_name + "/" + to_string(lvl_index) + ".rdlvl")
{
    m_levelStructure = new LevelStructure(map_name, lvl_index);
    if(!readFile())
        throw runtime_error("ERROR FATAL: el archivo del nivel no existe o esta corrupto.");
    processData();
    closeFile();
}

LevelFile::~LevelFile()
{}

LevelStructure* LevelFile::getLevelStructure() const
{
    return m_levelStructure;
}

vector<Collectable*> LevelFile::getCollectables() const
{
    return m_collectables;
}

float LevelFile::getTime() const
{
    return m_level_time;
}

string LevelFile::getMusic() const
{
    return m_level_music;
}

void LevelFile::processData()
{
    readTime();
    readSize();
    readMusic();
    readStartAndEnd();
    readResources();
    readCollectables();
    readTilemap();
}

void LevelFile::readTime()
{
    m_level_time = m_json["time"].asFloat();
}

void LevelFile::readMusic()
{
    m_level_music = m_json["music"].asString();
}

void LevelFile::readSize()
{
    m_levelStructure->setTilesCount(Vector2u(
        m_json["size"]["x"].asInt(),
        m_json["size"]["y"].asInt()
    ));
}

void LevelFile::readStartAndEnd()
{
    m_levelStructure->setStartTile(Vector2u(
        m_json["startTile"]["x"].asInt(),
        m_json["startTile"]["y"].asInt()
    ));

    m_levelStructure->setEndTile(Vector2u(
        m_json["endTile"]["x"].asInt(),
        m_json["endTile"]["y"].asInt()
    ));
}

void LevelFile::readResources()
{
    m_levelStructure->setBackgroundFromFile( m_json["background"].asString() );
    m_levelStructure->setTileset( m_json["tileset"].asString() );
    readAnimatedTextures();
    readParallaxTextures();
}

void LevelFile::readCollectables()
{
    for (auto& collectableData : m_json["collectables"])
    {
        Vector2u tileIndexes = {
            collectableData["tilePos"]["x"].asUInt(),
            collectableData["tilePos"]["y"].asUInt(),
        };

        Collectable* collectable = nullptr;

        switch ( (Collectable::Type)collectableData["id"].asInt() )
        {
        case Collectable::Type::Coin:
            collectable = new Coin(tileIndexes);
            break;
        case Collectable::Type::DragonBall1:
            collectable = new DragonBall(1, tileIndexes);
            break;
        case Collectable::Type::DragonBall2:
            collectable = new DragonBall(2, tileIndexes);
            break;
        case Collectable::Type::DragonBall3:
            collectable = new DragonBall(3, tileIndexes);
            break;
        case Collectable::Type::DragonBall4:
            collectable = new DragonBall(4, tileIndexes);
            break;
        case Collectable::Type::DragonBall5:
            collectable = new DragonBall(5, tileIndexes);
            break;
        case Collectable::Type::DragonBall6:
            collectable = new DragonBall(6, tileIndexes);
            break;
        case Collectable::Type::DragonBall7:
            collectable = new DragonBall(7, tileIndexes);
            break;
        default:
            collectable = new Coin(tileIndexes);
            break;
        }

        m_collectables.push_back( collectable );
    }
}

void LevelFile::readTilemap()
{
    int x = 0;
    for (const Json::Value& tilesColumn : m_json["tilemap"])
    {
        int y = 0;
        for (const Json::Value& tileData : tilesColumn)
        {
            if (tileData.isNull()) {
                ++y;
                continue;
            }

            Tile* tile = new Tile();
            tile->setTextureId( tileData["texture_id"].asInt() );
            tile->setHarmfulState( tileData["is_harmful"].asBool() );
            tile->setSolidState( tileData["is_solid"].asBool() );
            tile->setAnimatedState( tileData["is_animated"].asBool() );
            tile->setBreakableState(tileData["is_breakable"].asBool());
            tile->setBreakableSide( Ubication(tileData["breakable_side"].asInt()) );
            tile->setBreakableResistance( tileData["hits_to_break"].asInt() );
            if (!tileData["hitbox"].isNull())
                tile->setHitbox(Hitbox(
                    tileData["hitbox"]["position"]["x"].asFloat(),
                    tileData["hitbox"]["position"]["y"].asFloat(),
                    tileData["hitbox"]["size"]["x"].asFloat(),
                    tileData["hitbox"]["size"]["y"].asFloat()
                ));
            m_levelStructure->setTile( Vector2u(x, y++), tile );
        }
        ++x;
    }
}

void LevelFile::readAnimatedTextures()
{
    extern ResourceHandler resourceHandler;
    for (const Json::Value& animationData : m_json["animatedTextures"])
        resourceHandler.preloadAnimatedTileTexture(
            animationData["texture_path"].asString(),
            animationData["duration"].asFloat()
        );
}

void LevelFile::readParallaxTextures()
{
    for (const Json::Value& parallax : m_json["parallaxTextures"]) {
        ParallaxElement* p = new ParallaxElement();
        
        p->setRepeat( parallax["repeat"].asBool() );
        p->setRepeatSpace( parallax["repeat_space"].asFloat() );
        p->setZ( parallax["z"].asFloat() );
        p->setY( parallax["y"].asInt() );
        p->setBegin( parallax["begin"].asInt() );
        p->setEnd( parallax["end"].asInt() );
        p->setTextureFromFile(parallax["img"].asString());

        m_levelStructure->setParallaxElement(p);
    }
}