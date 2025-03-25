#pragma once
#include "Game.h"
#include "Tile.h"
#include "Collectable.h"
#include "ParallaxElement.h"

namespace rtd {

    class LevelStructure
    {
        int m_lvl_index = -1;
        std::string m_map_name;
        sf::Vector2u m_tilesCount, m_start_tile_indexes, m_end_tile_indexes;
        sf::Texture m_backgroundTexture;
        sf::Sprite m_backgroundSprite;
        std::vector<rtd::ParallaxElement*> m_parallaxElements;
        std::vector<sf::Vector2u> m_animatedTilesIndexes;
        std::vector< std::vector<rtd::Tile*> > m_tilemap;

        void setBackgroundSprite();
        void drawTiles(sf::RenderWindow& window, float screen_offset);
        void drawParallax(sf::RenderWindow& window, float screen_offset);

    public:
        LevelStructure();
        LevelStructure(std::string map_name, int lvl_index);
        ~LevelStructure();
        LevelStructure operator=(const LevelStructure& lvlStructure);

        void update(const float& delta_time);
        void draw(sf::RenderWindow& window, float screen_offset);

        bool isValidTile(int index_x,  int index_y) const;
        void setTilesCount(sf::Vector2u tilesCount);
        void setStartTile(sf::Vector2u start);
        void setEndTile(sf::Vector2u end);
        void setBackgroundFromFile(const std::string& file_path);
        void setTileset(const std::string& path);
        void setTilemap(std::vector< std::vector<Tile*> >& tilemap);
        void setTile(sf::Vector2u tileIndexes, rtd::Tile* tile);
        void setParallaxElement(rtd::ParallaxElement* element);
        void sortParallaxElements();
        void handleTilesHit(const std::vector< std::pair<sf::Vector2u, rtd::Ubication> >& tilesHitIndexes);
        bool tileIsHarmful(sf::Vector2u indexes) const;

        const rtd::Tile& getTile(int index_x, int index_y) const;
        const std::vector< std::vector<Tile*> >& getTilemap() const;
        const sf::Vector2u& getStartTileIndexes() const;
        const sf::Vector2u& getEndTileIndexes() const;
        sf::Vector2u getTilesCount() const;
        sf::Vector2<float> getSizeInPixels() const;
        sf::Texture* getBackground();
        const std::vector<rtd::ParallaxElement*>& getParallaxElements() const;
    };

}