#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Audio/Sound.hpp>
#include "Ubication.h"
#include "Entity.h"

namespace rtd {

    class Tile : public rtd::Entity
    {
        sf::Sound* m_hitBreakSound = nullptr;
        sf::Sound* m_brokenSound = nullptr;
        sf::Texture* m_tileset = nullptr;
        sf::Texture* m_brokenTileset = nullptr;
        sf::Sprite* m_brokenSprite = nullptr;
        unsigned int m_texture_id = 0;
        bool m_is_solid = false;
        bool m_is_harmful = false;
        bool m_is_breakable = false;
        unsigned int m_resistance = 0;
        unsigned int m_received_hits = 0;
        rtd::Ubication m_breakable_side = Ubication::bottom;

        void setStaticTile();
        void makeItNull();
        void fixSpriteSize();
        void hitToBreak();
        void initBrokenSprite();
        void initBreakableSounds();
        void deleteBrokenSprite();
        void deleteBreakSounds();

    public:
        Tile();
        ~Tile();

        void draw(sf::RenderWindow& window) override;

        void setPosition(sf::Vector2f position) override;
        void setAnimatedState(bool value);
        void setTextureId(unsigned int value);
        void setSolidState(bool value);
        void setHarmfulState(bool value);
        void setBreakableState(bool value);
        void setBreakableSide(rtd::Ubication value);
        void setBreakableResistance(unsigned int value);
        void hit(rtd::Ubication side);

        unsigned int getTextureId() const;
        unsigned int getBreakableResistance() const;
        rtd::Ubication getBreakableSide() const;

        bool isSolid() const;
        bool isBreakable() const;
        bool isHarmful() const;
        bool isTransparent() const;
        bool isNull() const;
    };

}