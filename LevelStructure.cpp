#include "LevelStructure.h"
#include "LevelFile.h"

using namespace std;
using namespace rtd;
using namespace sf;
using rtd::resourceHandler;

LevelStructure::LevelStructure() {}

LevelStructure::LevelStructure(string map_name, int lvl_index)
{
    m_lvl_index = lvl_index;
    m_map_name = map_name;
}

LevelStructure::~LevelStructure()
{
    for (auto tileCol : m_tilemap)
        for (auto tile : tileCol)
            if (tile != nullptr) {
                delete tile;
                tile = nullptr;
            }
}

void LevelStructure::update(const float& delta_time)
{
    for (auto& tileIndexes : m_animatedTilesIndexes)
        m_tilemap[tileIndexes.x][tileIndexes.y]->update(delta_time);
}

void LevelStructure::draw(RenderWindow& window, float screen_offset)
{
    window.draw(m_backgroundSprite);
    drawParallax(window, screen_offset);
    drawTiles(window, screen_offset);
}

bool LevelStructure::isValidTile(int index_x, int index_y) const
{
    if (m_tilemap.size() == 0 || index_x < 0 || index_y < 0)
        return false;

    unsigned int last_index_x = int(m_tilemap.size()) - 1;

    if (index_x >= 0 && index_x <= last_index_x)
    {
        if (m_tilemap[index_x].size() == 0)
            return false;

        unsigned int last_index_y = int(m_tilemap[index_x].size()) - 1;
        if (index_y >= 0 && index_y <= last_index_y)
            if (m_tilemap[index_x][index_y] == nullptr)
                return false;
            else
                return !m_tilemap[index_x][index_y]->isNull();
    }

    return false;
}

void LevelStructure::setBackgroundSprite()
{
    float scale = WIN_WIDTH / m_backgroundTexture.getSize().x;
    m_backgroundSprite.setTexture(m_backgroundTexture);
    m_backgroundSprite.setScale(scale, scale);
    m_backgroundSprite.setPosition(0, 0);
}

void LevelStructure::drawTiles(RenderWindow& window, float screen_offset)
{
    //Recorre todos los tiles para, aunque no se deban dibujar, actualizar sus posiciones
    //ya que el hitbox debe estar actualizado para el funcionamiento del MovementLimiter

    Vector2u lvlSize = getTilesCount();

    for (int x = 0; x < lvlSize.x; ++x)
        for (int y = 0; y < lvlSize.y; ++y)
            if (isValidTile(x, y))
            {
                float position_x = x * TILE_SIZE;
                m_tilemap[x][y]->setPosition({ position_x - screen_offset, y * TILE_SIZE });
                if (position_x + TILE_SIZE > screen_offset && position_x < screen_offset + WIN_WIDTH)
                    m_tilemap[x][y]->draw(window);
            }
}

void LevelStructure::drawParallax(RenderWindow& window, float screen_offset)
{
    for (auto element = m_parallaxElements.begin(); element != m_parallaxElements.end(); ++element)
        (*element)->draw(window, screen_offset);
}

void LevelStructure::setTilesCount(Vector2u tilesCount)
{
    m_tilesCount = tilesCount;
}

void LevelStructure::setStartTile(Vector2u start)
{
    m_start_tile_indexes = start;
}

void LevelStructure::setEndTile(Vector2u end)
{
    m_end_tile_indexes = end;
}

void LevelStructure::setBackgroundFromFile(const string& file_path)
{
    m_backgroundTexture.loadFromFile(file_path);
    setBackgroundSprite();
}

void LevelStructure::setTileset(const string& path)
{
    extern ResourceHandler resourceHandler;
    resourceHandler.preloadTileset(path);
}

void LevelStructure::setTilemap(vector< vector<Tile*> >& tilemap)
{
    m_tilemap = tilemap;
}

void LevelStructure::setTile(Vector2u tileIndexes, Tile* tile)
{
    while  (int(m_tilemap.size())-1 < int(tileIndexes.x) )
        m_tilemap.push_back({});

    while ( int(m_tilemap[tileIndexes.x].size()) - 1 < int(tileIndexes.y) )
        m_tilemap[tileIndexes.x].push_back(nullptr);

    m_tilemap[tileIndexes.x][tileIndexes.y] = tile;
    //auto& insertedTile = m_tilemap[tileIndexes.x][tileIndexes.y];
    
    if (tile->isAnimated()) {
        //auto textureIt = m_animatedTextures.begin() + insertedTile.getTextureId();
        //insertedTile.setAnimatedTexture(*textureIt);
        m_animatedTilesIndexes.push_back( tileIndexes );
    }
}

void LevelStructure::setParallaxElement(ParallaxElement* element)
{
    m_parallaxElements.push_back(element);
    sortParallaxElements();
}

void LevelStructure::sortParallaxElements()
{
    sort(
        m_parallaxElements.begin(),
        m_parallaxElements.end(),
        rtd::compareParallaxPointersWithGreaterOperator
    );
}

void LevelStructure::handleTilesHit(const vector< pair<Vector2u, Ubication> >& tilesHitData)
{
    for (const auto& tileHitData : tilesHitData)
    {
        const Vector2u& index = tileHitData.first;
        const Ubication& side = tileHitData.second;
        m_tilemap[index.x][index.y]->hit(side);
    }
}

bool LevelStructure::tileIsHarmful(Vector2u indexes) const
{
    return m_tilemap[indexes.x][indexes.y]->isHarmful();
}

const Tile& LevelStructure::getTile(int index_x, int index_y) const
{
    return *m_tilemap[index_x][index_y];
}

const vector< vector<Tile*> >& LevelStructure::getTilemap() const
{
    return m_tilemap;
}

const Vector2u& LevelStructure::getStartTileIndexes() const
{
    return m_start_tile_indexes;
}

const Vector2u& LevelStructure::getEndTileIndexes() const
{
    return m_end_tile_indexes;
}

Vector2u LevelStructure::getTilesCount() const
{
    return m_tilesCount;
}

Vector2<float> LevelStructure::getSizeInPixels() const
{
    return {m_tilesCount.x * TILE_SIZE, m_tilesCount.y * TILE_SIZE};
}

Texture* LevelStructure::getBackground()
{
    return &m_backgroundTexture;
}

const vector<ParallaxElement*>& LevelStructure::getParallaxElements() const
{
    return m_parallaxElements;
}

LevelStructure LevelStructure::operator=(const LevelStructure& lvlStructure)
{
    this->m_lvl_index = lvlStructure.m_lvl_index;
    this->m_map_name = lvlStructure.m_map_name;
    this->m_tilesCount = lvlStructure.m_tilesCount;
    this->m_start_tile_indexes = lvlStructure.m_start_tile_indexes;
    this->m_end_tile_indexes = lvlStructure.m_end_tile_indexes;
    this->m_start_tile_indexes = lvlStructure.m_start_tile_indexes;
    this->m_backgroundTexture = lvlStructure.m_backgroundTexture;
    this->setBackgroundSprite();
    this->m_animatedTilesIndexes = lvlStructure.m_animatedTilesIndexes;
    this->m_parallaxElements = lvlStructure.m_parallaxElements;
    this->m_tilemap = lvlStructure.m_tilemap;
    return *this;
}