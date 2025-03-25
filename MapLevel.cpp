#include <stdexcept>
#include "MapLevel.h"

using namespace std;
using namespace rtd;
using namespace sf;

void MapLevel::setNeighbors(map<Ubication, int> neighbors)
{
    if (neighbors.find(Ubication::top) != neighbors.end() && 
        neighbors.find(Ubication::right) != neighbors.end() &&
        neighbors.find(Ubication::bottom) != neighbors.end() &&
        neighbors.find(Ubication::left) != neighbors.end())
    {
        m_neighbors[Ubication::top] = neighbors.find(Ubication::top)->second;
        m_neighbors[Ubication::right] = neighbors.find(Ubication::right)->second;
        m_neighbors[Ubication::bottom] = neighbors.find(Ubication::bottom)->second;
        m_neighbors[Ubication::left] = neighbors.find(Ubication::left)->second;
    }
    else
        throw runtime_error("Ocurrió un error al intentar cargar los niveles vecinos: mapa invalido.");
}

MapLevel::MapLevel()
{
    m_map_part = -1;
    m_is_locked = true;
};

MapLevel::MapLevel(int map_part, Vector2f coordinate) {
    m_map_part = map_part;
    m_coordinates = coordinate;
    m_is_locked = true;
}

int MapLevel::getMapPart() const
{
    return m_map_part;
}

const Vector2f MapLevel::getPosition() const
{
    return m_coordinates;
}

int MapLevel::getNeighborIndex(Ubication side) const
{
    return m_neighbors.find(side)->second;
}

map<Ubication, int> MapLevel::getNeighborsIndex() const
{
    return m_neighbors;
}

void MapLevel::unlock()
{
    m_is_locked = false;
}

bool MapLevel::isLocked() const
{
    return m_is_locked;
}

bool MapLevel::operator==(const MapLevel& maplvl)
{
    return (this->getMapPart() == maplvl.getMapPart()) && (this->getPosition() == maplvl.getPosition());
}