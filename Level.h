#pragma once
#include <vector>
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include "Scene.h"
#include "LevelStructure.h"
#include "Tile.h"
#include "Player.h"
#include "MovementsLimiter.h"
#include "LevelHUD.h"
#include "LevelEndAnimation.h"
#include "GameOverAnimation.h"
#include "Profile.h"

namespace rtd {

    class Level : public rtd::Scene
    {
        sf::SoundBuffer m_ballSoundBuffer;
        sf::Sound m_ballSound;

        int m_lvl_index;
        float m_screen_offset = 0;
        sf::Vector2f m_startTilePos, m_endTilePos;
        rtd::LevelStructure* m_levelStructure = nullptr;
        rtd::Player m_player;
        rtd::MovementsLimiter m_movementsLimiter;
        std::vector< rtd::Collectable* > m_collectables;
        rtd::LevelHUD m_HUD;
        rtd::LevelEndAnimation* m_endAnimation = nullptr;
        rtd::GameOverAnimation* m_gameOverAnimation = nullptr;
        rtd::Scene* m_pauseMenu = nullptr;
        bool m_is_paused = false;
        bool m_game_over = false;
        bool m_completed = false;
        float m_time_left = 0;
        sf::Text m_timeText;

        void setSounds();
        void drawPlayer(sf::RenderWindow& window);
        void drawCollectables(sf::RenderWindow& window);
        void updateCollectables(const float& delta_time);
        void updatePlayer(const float& delta_time);
        void updateTime(const float& delta_time);
        void updateScreenOffset();
        void checkPlayerInLimits();
        void checkPlayerHitHarmful();
        bool playerIsInTheEnd();
        void pause();
        void finish();
        void gameOver();
        void killPlayer();

    public:
        Level(std::string map_name, int lvl_index);
        ~Level();

        void unpause();
        void setSoundsVolume(float volume);
        void processEvents(sf::Event event) override;
        void draw(sf::RenderWindow& window) override;
        void update(const float& delta_time, rtd::Game& game) override;
        void setCollectable(Collectable* collectable);
    };

}