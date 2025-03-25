#pragma once
#include <string>
#include "Level.h"
#include "FileReader.h"

namespace rtd {

    class LevelFile : public rtd::FileReader
    {
        rtd::LevelStructure* m_levelStructure = nullptr;
        std::vector< rtd::Collectable* > m_collectables;
        std::string m_level_music;
        float m_level_time = 0;

        void processData() override;
        void readTime();
        void readMusic();
        void readSize();
        void readStartAndEnd();
        void readResources();
        void readAnimatedTextures();
        void readParallaxTextures();
        void readCollectables();
        void readTilemap();

    public:
        LevelFile(std::string map_name, int lvl_index);
        ~LevelFile();
        rtd::LevelStructure* getLevelStructure() const;
        std::vector< rtd::Collectable* > getCollectables() const;
        float getTime() const;
        std::string getMusic() const;
    };

}