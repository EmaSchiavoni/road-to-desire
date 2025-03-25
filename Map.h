#pragma once
#include <vector>
#include <string>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>
#include "Game.h"
#include "MapHUD.h"
#include "MapLevel.h"
#include "WonGameAnimation.h"

namespace rtd {

    class Map : public rtd::Scene
    {
        rtd::WonGameAnimation* m_wonGameAnimation = nullptr;
        sf::SoundBuffer m_moveSoundBuffer;
        sf::Sound m_moveSound;
        sf::Sprite m_backSprite;
        bool m_initialized = false;
        std::string m_map_name;
        int m_parts_quantity = -1;
        sf::Texture m_lockTexture;
        sf::Texture m_selectorTexture;
        std::vector<sf::Texture> m_mapTextures;
        std::vector<sf::Sprite> m_mapSprites;
        std::vector<rtd::MapLevel> m_levels;
        rtd::MapLevel* m_level_focus = nullptr;
        int m_level_selected = -1;
        bool m_back_to_menu = false;
        sf::Sprite m_lockSprite;
        sf::Sprite m_selector;
        rtd::MapHUD m_HUD;

        bool gameIsComplete() const;
        int getCurrentPart() const;
        void setFocus(MapLevel& target);
        void loadTextures();
        void generateSelector();
        void generateMapSprites();
        void generateLockSprite();

        void moveFocus(rtd::Ubication direction);
        void updateSelector(float delta_time);

    public:
        Map(rtd::Map& map);
        Map();
        ~Map();
        void draw(sf::RenderWindow& window) override;
        void update(const float& delta_time, rtd::Game& game) override;
        void processEvents(sf::Event event) override;

        void unlockLevel(unsigned int level);
        void unlockUpToLevel(unsigned int last_unlocked_index);
        void addLevel(rtd::MapLevel level);

        rtd::MapLevel& getLevelStructure(int index);
        int getPartsQuantity();

        void setPartsQuantity(int parts_quantity);
        void init();
    };

}