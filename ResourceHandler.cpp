#include "ResourceHandler.h"
#include <stdexcept>

using namespace std;
using namespace rtd;
using namespace sf;
using rtd::resourceHandler;

ResourceHandler resourceHandler;

ResourceHandler::ResourceHandler()
{
    m_breakableTileHitSoundBuffer.path = "Sounds/FX/Tile/hit_breakable.ogg";
    m_brokenTileSoundBuffer.path = "Sounds/FX/Tile/broken.ogg";

    m_fontDeadJim.path = "Fonts/DEADJIM.TTF";
    m_fontMontserrat.path = "Fonts/Montserrat-ExtraLight.otf";
    m_gameTitleImage.path = "Textures/Statics/menu_title.png";
    m_menuBackground.path = "Textures/Statics/menu_background.jpg";
    m_brokenTileset.path = "Textures/Tilesets/broken.png";
    m_backTexture.path ="Textures/Statics/back.png";
    m_animatedCoinTexture.first.path = "Textures/Animateds/coin.png";
    m_animatedCoinTexture.second = .85;

    m_gameTitleImage.resource.setSmooth(true);
    m_menuBackground.resource.setSmooth(true);
    m_tileset.resource.setSmooth(true);
    m_brokenTileset.resource.setSmooth(true);
    m_backTexture.resource.setSmooth(true);
}

template<typename ResType>
ResType* ResourceHandler::getResource(Resource<ResType>& resource)
{
    if (!resource.is_loaded)
        if (resource.resource.loadFromFile(resource.path))
            resource.is_loaded = true;
        else
            throw runtime_error("Ocurrió un error al intentar cargar el archivo " + resource.path);

    ++resource.elements_using_it;
    return &resource.resource;
}

template<typename ResType>
void ResourceHandler::unloadResource(Resource<ResType>& resource)
{
    if (!resource.is_loaded)
        return;
    
    if (--resource.elements_using_it == 0) {
        resource.resource = ResType();
        resource.is_loaded = false;
    }
}

SoundBuffer* ResourceHandler::getBreakableTileHitSoundBuffer()
{
    return getResource(m_breakableTileHitSoundBuffer);
}

SoundBuffer* ResourceHandler::getBrokenTileSoundBuffer()
{
    return getResource(m_brokenTileSoundBuffer);
}

Font* ResourceHandler::getTitleFont()
{
    return getResource(m_fontDeadJim);
}

Font* ResourceHandler::getThinFont()
{
    return getResource(m_fontMontserrat);
}

Font* ResourceHandler::getMenuFont()
{
    return getResource(m_fontDeadJim);
}

Font* ResourceHandler::getHudFont()
{
    return getResource(m_fontDeadJim);
}

Texture* ResourceHandler::getGameTitleImage()
{
    Texture* texture = getResource(m_gameTitleImage);
    texture->setSmooth(true);
    return texture;
}

Texture* ResourceHandler::getMenuBackground()
{
    return getResource(m_menuBackground);
}

Texture* ResourceHandler::getTileset()
{
    if (m_tileset.path == "")
        throw runtime_error("ERROR: Se intentó obtener el tileset sin haberlo cargado.");
    else
        return getResource(m_tileset);
}

Texture* ResourceHandler::getBrokenTileset()
{
    if (m_brokenTileset.path == "")
        throw runtime_error("ERROR: Se intentó obtener el tileset 'broken' sin haberlo cargado.");
    else
        return getResource(m_brokenTileset);
}

Texture* ResourceHandler::getBackTexture()
{
    if (m_backTexture.path == "")
        throw runtime_error("ERROR: Se intentó obtener la texture 'back' sin haberla cargado.");
    else
        return getResource(m_backTexture);
}

pair<Texture*, float> ResourceHandler::getAnimatedCoin()
{
    Texture* texture = getResource<Texture>(m_animatedCoinTexture.first);
    float animation_duration = m_animatedCoinTexture.second;
    return pair<Texture*, float>(texture, animation_duration);
}

pair<Texture*, float> ResourceHandler::getAnimatedTileTexture(unsigned int id)
{
    if (m_animatedTilesTextures.size() <= id)
        throw runtime_error("ERROR: Se intentó obtener una textura de tile animada que se encuentra fuera de los limites del contenedor.");
    else {
        auto& animatedTextureData = next( m_animatedTilesTextures.begin(), id )->second;
        float animation_duration = animatedTextureData.second;
        Texture* texture = getResource<Texture>(animatedTextureData.first);

        return pair<Texture*, float>(texture, animation_duration);
    }
}

void ResourceHandler::unloadBreakableTileHitSoundBuffer()
{
    unloadResource(m_breakableTileHitSoundBuffer);
}

void ResourceHandler::unloadBrokenTileSoundBuffer()
{
    unloadResource(m_brokenTileSoundBuffer);
}

void ResourceHandler::unloadTitleFont()
{
    unloadResource(m_fontDeadJim);
}

void ResourceHandler::unloadThinFont()
{
    unloadResource(m_fontMontserrat);
}

void ResourceHandler::unloadMenuFont()
{
    unloadResource(m_fontDeadJim);
}

void ResourceHandler::unloadHudFont()
{
    unloadResource(m_fontDeadJim);
}

void ResourceHandler::unloadGameTitleImage()
{
    unloadResource(m_gameTitleImage);
}

void ResourceHandler::unloadMenuBackground()
{
    unloadResource(m_menuBackground);
}

void ResourceHandler::unloadTileset()
{
    unloadResource(m_tileset);
}

void ResourceHandler::unloadBrokenTileset()
{
    unloadResource(m_brokenTileset);
}

void ResourceHandler::unloadBackTexture()
{
    unloadResource(m_backTexture);
}

void ResourceHandler::unloadAnimatedCoin()
{
    unloadResource(m_animatedCoinTexture.first);
}

void ResourceHandler::unloadAnimatedTileTexture(unsigned int id)
{
    unloadResource( next(m_animatedTilesTextures.begin(), id)->second.first );
}

void ResourceHandler::preloadAnimatedTileTexture(const string& path, float animation_duration)
{
    m_animatedTilesTextures.insert({
        path, {Resource<Texture>(), animation_duration}
    });
    m_animatedTilesTextures[path].first.path = path;
}

void ResourceHandler::preloadTileset(const string& path)
{
    m_tileset = Resource<Texture>();
    m_tileset.path = path;
}