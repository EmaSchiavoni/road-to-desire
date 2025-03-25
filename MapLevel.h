#pragma once
#include <map>
#include <string>
#include <SFML/System/Vector2.hpp>
#include "Ubication.h"

namespace rtd {

    class MapLevel
    {
        int m_map_part;
        sf::Vector2f m_coordinates;
        bool m_is_locked;
        std::map<rtd::Ubication, int> m_neighbors;

    public:
        MapLevel();
        MapLevel(int map_part, sf::Vector2f coordinate);
        int getMapPart() const;
        const sf::Vector2f getPosition() const;
        int getNeighborIndex(rtd::Ubication side) const;
        std::map<rtd::Ubication, int> getNeighborsIndex() const;
        void setNeighbors(std::map<rtd::Ubication, int> neighbors);
        void unlock();
        bool isLocked() const;

        bool operator==(const rtd::MapLevel& maplvl);
    };

}