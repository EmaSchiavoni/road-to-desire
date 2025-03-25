#pragma once
#include <string>
#include <vector>
#include "Map.h"
#include "MapLevel.h"
#include "Scene.h"
#include "FileReader.h"

namespace rtd {

    class MapFile : public rtd::FileReader
    {
        rtd::Map m_map;
        void processData() override;
        void readParts();
        void readLevels();
        void readLevelPartAndPosition(const Json::Value& level);
        void readLevelNeighbors(const Json::Value& level, int lvl_index);

    public:
        MapFile();
        rtd::Scene* getMap();
    };

}