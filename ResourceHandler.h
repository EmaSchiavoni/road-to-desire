#pragma once
#include <string>
#include <vector>
#include <map>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include "Resource.h"

namespace rtd {

    class ResourceHandler
    {
        rtd::Resource<sf::SoundBuffer> m_breakableTileHitSoundBuffer;
        rtd::Resource<sf::SoundBuffer> m_brokenTileSoundBuffer;
        rtd::Resource<sf::Font> m_fontDeadJim;
        rtd::Resource<sf::Font> m_fontMontserrat;
        rtd::Resource<sf::Texture> m_gameTitleImage;
        rtd::Resource<sf::Texture> m_menuBackground;
        rtd::Resource<sf::Texture> m_tileset;
        rtd::Resource<sf::Texture> m_brokenTileset;
        rtd::Resource<sf::Texture> m_backTexture;
        std::pair< rtd::Resource<sf::Texture>, float> m_animatedCoinTexture;
        std::map< std::string, std::pair< rtd::Resource<sf::Texture>, float>  > m_animatedTilesTextures;

        template<typename ResType>
        ResType* getResource(rtd::Resource<ResType>& resource);

        template<typename ResType>
        void unloadResource(rtd::Resource<ResType>& resource);

    public:
        ResourceHandler();

        sf::SoundBuffer* getBreakableTileHitSoundBuffer();
        sf::SoundBuffer* getBrokenTileSoundBuffer();
        sf::Font* getTitleFont();
        sf::Font* getThinFont();
        sf::Font* getMenuFont();
        sf::Font* getHudFont();
        sf::Texture* getGameTitleImage();
        sf::Texture* getMenuBackground();
        sf::Texture* getTileset();
        sf::Texture* getBrokenTileset();
        sf::Texture* getBackTexture();
        std::pair<sf::Texture*, float> getAnimatedCoin();
        std::pair<sf::Texture*, float> getAnimatedTileTexture(unsigned int id);


        void unloadBreakableTileHitSoundBuffer();
        void unloadBrokenTileSoundBuffer();
        void unloadTitleFont();
        void unloadThinFont();
        void unloadMenuFont();
        void unloadHudFont();
        void unloadGameTitleImage();
        void unloadMenuBackground();
        void unloadTileset();
        void unloadBackTexture();
        void unloadBrokenTileset();
        void unloadAnimatedCoin();
        void unloadAnimatedTileTexture(unsigned int id);
        void preloadAnimatedTileTexture(const std::string& path, float animation_duration);
        void preloadTileset(const std::string& path);
    };

    //Objeto global para reutilizar recursos en memoria
    extern rtd::ResourceHandler resourceHandler;

}