#pragma once
#include <map>
#include <vector>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Audio.hpp>
#include "Entity.h"
#include "Ubication.h"
#include "Constants.h"
#include "Collectable.h"

namespace rtd {

    class Game;

    class Player : public rtd::Entity
    {
        sf::SoundBuffer m_collectSoundBuffer, m_steprSoundBuffer, m_steplSoundBuffer;
        sf::SoundBuffer m_jumpSoundBuffer, m_killSoundBuffer, m_ballCollectSoundBuffer;
        sf::Sound m_collectSound, m_steprSound, m_steplSound;
        sf::Sound m_jumpSound, m_killSound, m_ballCollectSound;
        float m_distanceRun = 0;

        sf::Texture m_texture;
        sf::Vector2f m_speed; //pixels per sec
        sf::Vector2f m_position;
        std::vector< std::pair<sf::Vector2u, rtd::Ubication> > m_hitTilesIndexes;
        sf::Vector2i m_frameSize = { 216, 240 };
        sf::Vector2i m_currentAnimFrame;
        float m_running_animation_duration = 0.66f;
        float m_frame_duration = 0.11f;
        float m_animation_elapsed_time = 0;

        const int m_max_speed = 120;
        bool m_jumping = false;
        bool m_walking_right = false;
        bool m_walking_left = false;
        int m_lives = 3;
        std::map< rtd::Collectable::Type, int > m_collectables;
        int m_coins = 0;

        void setSounds();
        void falling(const float& delta_time);
        void stopFalling();
        void updateHitTilesIndexes(const sf::Vector2f& movementSize, const sf::Vector2<rtd::Ubication>& direction, const std::map<rtd::Ubication, std::pair<float, sf::Vector2i>>& movementLimits);
        void setHitTile(const sf::Vector2u& indexes, const rtd::Ubication& side);
        std::vector<unsigned int> getOccuppiedTilesIndexesInX() const;
        std::vector<unsigned int> getOccuppiedTilesIndexesInY() const;

        void updatePosition(sf::Vector2f movementSize, const std::map<rtd::Ubication, std::pair<float, sf::Vector2i>>& movementLimits);
        void updateSpeedX();
        void updateSpeedY(const float& delta_time, const std::map<rtd::Ubication, std::pair<float, sf::Vector2i>>& movementLimits);
        void updateAnimationFrame(const float& delta_time);
        void applyAnimationFrame();
        void playStepsSound(sf::Vector2f movementSize);

    public:
        Player();

        //Devuelve la posicion global respecto al nivel, no la posicion del sprite en la pantalla.
        const sf::Vector2f& getPosition() const;
        const sf::Vector2f& getSpeed() const;
        int getLives() const;
        int getCollectableCount( rtd::Collectable::Type type ) const;
        int getDragonBallCount() const;
        std::vector <int> getCollectedDragonBallIndexes() const;
        sf::Vector2< std::vector<unsigned int> > getOccuppiedTilesIndexes() const;
        const std::vector< std::pair<sf::Vector2u, rtd::Ubication> >& getTilesHitData() const;

        void update(const float& delta_time, const std::map<rtd::Ubication, std::pair<float, sf::Vector2i>>& movementLimits);
        void draw(sf::RenderWindow& window, const sf::Vector2f& pos);
        void setPosition(sf::Vector2f globalPosition);
        void setSoundsVolume(float volume);
        void startJumping();
        void stopJumping();
        void startWalkingRight();
        void stopWalkingRight();
        void startWalkingLeft();
        void stopWalkingLeft();
        void jump();
        void kill();
        void setCollectable( rtd::Collectable::Type type, int count = 1 );
    };

}