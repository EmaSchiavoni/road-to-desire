#include <stdexcept>
#include <SFML/Audio/SoundBuffer.hpp>
#include "Tile.h"
#include "Constants.h"
#include "ResourceHandler.h"
#include "Settings.h"

using namespace std;
using namespace rtd;
using namespace sf;

extern ResourceHandler resourceHandler;

Tile::Tile()
{
    m_tileset = resourceHandler.getTileset();
    this->setTexture(*m_tileset);
    fixSpriteSize();
}

Tile::~Tile()
{
    makeItNull();
    if (m_brokenSound != nullptr)
        deleteBreakSounds();
}

void Tile::setStaticTile()
{
    m_tileset = resourceHandler.getTileset();
    this->setTexture(*m_tileset);
    fixSpriteSize();
}

void Tile::makeItNull()
{
    if (isNull()) return;

    setSolidState(false);
    setHarmfulState(false);
    setAnimatedState(false);
    setBreakableState(false);
    m_tileset = nullptr;
    resourceHandler.unloadTileset();
    this->setScale(0, 0);
}

void Tile::fixSpriteSize()
{
    float scale = TILE_SIZE / this->getTexture()->getSize().y;
    this->setScale(scale, scale);
}

void Tile::hitToBreak()
{
    if (m_brokenSound == nullptr)
        initBreakableSounds();

    if (++m_received_hits < m_resistance) {
        if (m_brokenTileset == nullptr)
            initBrokenSprite();

        m_hitBreakSound->play();
        unsigned int breaktile_size_in_texture = m_brokenTileset->getSize().y;
        unsigned int breakage_rate = (int)truncf( float(m_received_hits) / float(m_resistance) * 10);
        m_brokenSprite->setTextureRect(IntRect(
            breakage_rate * breaktile_size_in_texture, 
            0,
            breaktile_size_in_texture,
            breaktile_size_in_texture
        ));
        m_brokenSprite->setOrigin(m_brokenSprite->getLocalBounds().width / 2, m_brokenSprite->getLocalBounds().height / 2);
    }
    else {
        m_brokenSound->play();
        makeItNull();
    }
}

void Tile::initBrokenSprite()
{
    m_brokenTileset = resourceHandler.getBrokenTileset();
    m_brokenSprite = new Sprite();
    m_brokenSprite->setTexture(*m_brokenTileset);

    auto tileHitbox = this->getHitboxGlobalBounds();
    Vector2f hitboxCenter = { tileHitbox.left + tileHitbox.width / 2, tileHitbox.top + tileHitbox.height / 2 };
    m_brokenSprite->setPosition(hitboxCenter);
    //m_brokenSprite->setPosition(this->getPosition());
    int tile_size_in_texture = m_brokenTileset->getSize().y;
    Vector2f scale = { tileHitbox.width / m_brokenTileset->getSize().y, tileHitbox.height / m_brokenTileset->getSize().y };
    m_brokenSprite->setScale(scale);
}

void Tile::initBreakableSounds()
{
    extern Settings settings;
    m_hitBreakSound = new Sound(*resourceHandler.getBreakableTileHitSoundBuffer());
    m_brokenSound = new Sound(*resourceHandler.getBrokenTileSoundBuffer());
    m_hitBreakSound->setVolume(settings.getSoundsVolume());
    m_brokenSound->setVolume(settings.getSoundsVolume());
}

void Tile::deleteBrokenSprite()
{
    if (m_brokenSprite == nullptr) return;

    delete m_brokenSprite;
    m_brokenSprite = nullptr;
    m_brokenTileset = nullptr;
    resourceHandler.unloadBrokenTileset();
}

void Tile::deleteBreakSounds()
{
    if (m_brokenSound == nullptr) return;

    delete m_hitBreakSound;
    delete m_brokenSound;
    m_hitBreakSound = nullptr;
    m_brokenSound = nullptr;
}

void Tile::draw(sf::RenderWindow& window)
{
    if (isNull())
        return;

    Entity::draw(window);

    if(m_brokenSprite != nullptr)
        window.draw(*m_brokenSprite);
}

void Tile::setPosition(Vector2f position)
{
    Entity::setPosition(position);
    if (m_brokenSprite != nullptr) {
        auto tileHitbox = this->getHitboxGlobalBounds();
        Vector2f hitboxCenter = { tileHitbox.left + tileHitbox.width / 2, tileHitbox.top + tileHitbox.height / 2 };
        m_brokenSprite->setPosition(hitboxCenter);
    }
}

void Tile::setAnimatedState(bool value)
{
    if (value == isAnimated())
        return;

    if (value) {
        auto animationData = resourceHandler.getAnimatedTileTexture(m_texture_id);
        makeItAnimated(*animationData.first, animationData.second);
        resourceHandler.unloadTileset();
        m_tileset = nullptr;
    }
    else {
        resourceHandler.unloadAnimatedTileTexture(m_texture_id);
        unmakeAnimated();
        setStaticTile();
    }
}

void Tile::setTextureId(unsigned int id)
{
    int tile_size_in_texture = this->getTexture()->getSize().y;
    m_texture_id = id;
    this->setTextureRect(IntRect(
        tile_size_in_texture * m_texture_id,
        0,
        tile_size_in_texture,
        tile_size_in_texture
    ));
}

void Tile::setSolidState(bool value)
{
    m_is_solid = value;
}

void Tile::setHarmfulState(bool value)
{
    m_is_harmful = value;
}

void Tile::setBreakableState(bool value)
{
    if (value == isBreakable())
        return;

    m_is_breakable = value;

    if (!m_is_breakable && m_brokenSprite != nullptr)
        deleteBrokenSprite();
}

void Tile::setBreakableSide(Ubication value)
{
    if (value != Ubication::top && value != Ubication::right && value != Ubication::bottom && value != Ubication::left)
        throw runtime_error("ERROR: Se intentó asignar una ubicación invalida al lado rompible de un tile.");
    m_breakable_side = value;
}

void Tile::setBreakableResistance(unsigned int value)
{
    m_resistance = value;
}

void Tile::hit(Ubication side)
{
    if (isBreakable() && side == getBreakableSide())
        hitToBreak();
}

unsigned int Tile::getTextureId() const
{
    return m_texture_id;
}

unsigned int Tile::getBreakableResistance() const
{
    return m_resistance;
}

rtd::Ubication Tile::getBreakableSide() const
{
    return m_breakable_side;
}

bool Tile::isSolid() const
{
    return m_is_solid;
}

bool Tile::isBreakable() const
{
    return m_is_breakable;
}

bool Tile::isHarmful() const
{
    return m_is_harmful;
}

bool Tile::isTransparent() const
{
    return isNull() || (!isAnimated() && m_texture_id == 0);
}

bool Tile::isNull() const
{
    return m_tileset == nullptr && !isAnimated();
}