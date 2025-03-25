#include "Collectable.h"
#include "Constants.h"

using namespace std;
using namespace rtd;
using namespace sf;

Collectable::Collectable(Vector2u positionAsTile)
{
    m_posAsTile = positionAsTile;
    m_globalPosition = {
        m_posAsTile.x * TILE_SIZE + TILE_SIZE/2,
        m_posAsTile.y * TILE_SIZE + TILE_SIZE/2
    };
}

Collectable::~Collectable()
{}

bool Collectable::canBeDeleted()
{
    return wasCollected();
}

void Collectable::setGlobalPosition(float x, float y)
{
    m_globalPosition = {x,y};
}

Vector2u Collectable::getPositionAsTile() const
{
    return m_posAsTile;
}

Vector2f Collectable::getGlobalPosition() const
{
    return m_globalPosition;
}

Vector2f Collectable::getSize() const
{
    return {this->getGlobalBounds().width, this->getGlobalBounds().height};
}

void Collectable::collect()
{
    m_collected = true;
}

bool Collectable::wasCollected() const
{
    return m_collected;
}

Collectable::Type Collectable::getType() const
{
    if (m_type == Collectable::Type::NOT_INITIALIZED)
        throw runtime_error("ERROR: Se intentó obtener el tipo de un objeto Collectable sin inicializar");
    return m_type;
}

int Collectable::getTypeId() const
{
    if (m_type == Collectable::Type::NOT_INITIALIZED)
        throw runtime_error("ERROR: Se intentó obtener el id de tipo de un objeto Collectable sin inicializar");

    return (int)m_type;
}

void Collectable::setType(Collectable::Type type)
{
    m_type = type;
}

string rtd::toString(Collectable::Type type)
{
    switch(type) {
        case Collectable::Type::Coin: return "Coin";
        case Collectable::Type::DragonBall1: return "DragonBall1";
        case Collectable::Type::DragonBall2: return "DragonBall2";
        case Collectable::Type::DragonBall3: return "DragonBall3";
        case Collectable::Type::DragonBall4: return "DragonBall4";
        case Collectable::Type::DragonBall5: return "DragonBall5";
        case Collectable::Type::DragonBall6: return "DragonBall6";
        case Collectable::Type::DragonBall7: return "DragonBall7";
        default: return "";
    }
}

int rtd::getCollectableId(Collectable::Type type)
{
    return static_cast<int>(type);
}