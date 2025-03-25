#include "Level.h"
#include "LevelFile.h"
#include "LevelMenu.h"
#include "PlayMenu.h"
#include "MapFile.h"
#include "SoundsHandler.h"
#include "SoundPaths.h"
#include "Settings.h"
#include "Profile.h"

using namespace std;
using namespace rtd;
using namespace sf;

extern SoundsHandler globalSoundsHandler;

Level::Level(string map_name, int lvl_index) :
    m_HUD(m_player.getLives()),
    m_movementsLimiter(m_player, m_levelStructure)
{
    LevelFile lvlFile(map_name, lvl_index);
    globalSoundsHandler.setMusic(lvlFile.getMusic());
    m_time_left = lvlFile.getTime();
    m_levelStructure = lvlFile.getLevelStructure();
    m_collectables = lvlFile.getCollectables();
    m_lvl_index = lvl_index;
    m_startTilePos = Vector2f(
        m_levelStructure->getStartTileIndexes().x * TILE_SIZE,
        m_levelStructure->getStartTileIndexes().y * TILE_SIZE
    );
    m_endTilePos = Vector2f(
        m_levelStructure->getEndTileIndexes().x * TILE_SIZE,
        m_levelStructure->getEndTileIndexes().y * TILE_SIZE
    );
    m_player.setPosition(m_startTilePos);
    setSounds();
}

Level::~Level()
{
    if (m_pauseMenu != nullptr) {
        delete m_pauseMenu;
        m_pauseMenu = nullptr;
    }
    delete m_levelStructure;
    for (auto& collectable : m_collectables) {
        delete collectable;
        collectable = nullptr;
    }
    if (m_endAnimation != nullptr)
        delete m_endAnimation;
}

void Level::setSounds()
{
    extern Settings settings;
    m_ballSoundBuffer.loadFromFile( string(LEVEL_SOUNDS_PATH) + "game_over.ogg");
    m_ballSound.setBuffer(m_ballSoundBuffer);
    setSoundsVolume(settings.getSoundsVolume());
}

void Level::setSoundsVolume(float volume)
{
    m_ballSound.setVolume(volume);
    m_player.setSoundsVolume(volume);
}

void Level::drawPlayer(RenderWindow& window)
{
    auto& playerPos = m_player.getPosition();
    Vector2f newPos{ playerPos.x - m_screen_offset, playerPos.y };
    m_player.draw(window, newPos);
}

void Level::drawCollectables(RenderWindow& window)
{
    float screen_limit = m_screen_offset + WIN_WIDTH;

    for (auto& collectable : m_collectables)
    {
        if (collectable->canBeDeleted())
            continue;

        if (collectable->wasCollected()) {
            collectable->draw(window);
            continue;
        }
        auto tilePos = collectable->getPositionAsTile();
        bool is_in_front = m_levelStructure->isValidTile(tilePos.x, tilePos.y) ? m_levelStructure->getTile(tilePos.x, tilePos.y).isTransparent() : true;
        auto pos = collectable->getGlobalPosition();
        auto left_border = pos.x;
        auto right_border = pos.x + collectable->getSize().x;

        if (is_in_front && right_border > m_screen_offset && left_border < screen_limit) {
            collectable->setPosition(Vector2f(left_border - m_screen_offset, pos.y));
            collectable->draw(window);
        }
    }
}

void Level::updateCollectables(const float& delta_time)
{
    if (m_collectables.size() == 0 ||
        m_player.getGlobalBounds().top == 0 && m_player.getPosition().y != 0) //No se ha dibujado
        return;

    auto playerHitbox = m_player.getHitboxGlobalBounds();
    unsigned int collecteds_count = 0;
    auto collectablesEnd = m_collectables.end();
    auto collectableIt = m_collectables.begin();

    while (collectableIt != collectablesEnd)
    {
        auto& collectable = *collectableIt;

        if (collectable->canBeDeleted()) {
            delete collectable;
            collectablesEnd = prev(m_collectables.end(), ++collecteds_count);
            if (m_collectables.size() > collecteds_count)
                collectable = *collectablesEnd;
            continue;
        }

        if (playerHitbox.intersects(collectable->getHitboxGlobalBounds()) && !collectable->wasCollected()) {
            collectable->collect();
            m_player.setCollectable( collectable->getType() );
            m_HUD.setCollectable( collectable->getType() );
        }
        else {
            collectable->update(delta_time);
            ++collectableIt;
        }
    }

    if (collecteds_count > 0) {
        auto exEnd = m_collectables.end();
        m_collectables.erase(collectablesEnd, exEnd);
    }
}

void Level::updatePlayer(const float& delta_time)
{
    auto movementLimits = m_movementsLimiter.getPlayerMovementLimits(m_screen_offset);
    m_player.update(delta_time, movementLimits);
    m_levelStructure->handleTilesHit( m_player.getTilesHitData() );
    updateScreenOffset();
}

void Level::updateTime(const float& delta_time)
{
    m_time_left -= delta_time;
    if (m_time_left < 0)
        m_time_left = 0;
    m_HUD.setTimer(m_time_left);
}

void Level::updateScreenOffset()
{
    m_screen_offset = m_player.getPosition().x - WIN_WIDTH / 2;

    if (m_screen_offset < 0)
        m_screen_offset = 0;
    if (m_screen_offset > (float)m_levelStructure->getSizeInPixels().x - WIN_WIDTH)
        m_screen_offset = (float)m_levelStructure->getSizeInPixels().x - WIN_WIDTH;
}

void Level::checkPlayerInLimits()
{
    auto playerHitbox = m_player.getHitboxGlobalBounds();
    if (playerHitbox.top + playerHitbox.height >= WIN_HEIGHT)
        killPlayer();
}

void Level::checkPlayerHitHarmful()
{
    auto& tilesHitData = m_player.getTilesHitData();
    for(const auto& hitData : tilesHitData)
        if (m_levelStructure->tileIsHarmful(hitData.first)) {
            killPlayer();
            break;
        }
}

void Level::pause()
{
    globalSoundsHandler.pauseMusic();
    m_is_paused = true;
    if (m_pauseMenu == nullptr)
        m_pauseMenu = new LevelMenu(this, m_levelStructure->getBackground());
}

void Level::unpause()
{
    if (m_pauseMenu != nullptr) {
        delete m_pauseMenu;
        m_pauseMenu = nullptr;
    }
    m_is_paused = false;
    m_player.stopWalkingLeft();
    m_player.stopWalkingRight();
    m_player.stopJumping();
    globalSoundsHandler.playMusic();
}

void Level::processEvents(Event event)
{
    if (m_is_paused) {
        m_pauseMenu->processEvents(event);
        return;
    }

    switch (event.type)
    {
    case Event::KeyPressed:
    {
        switch (event.key.code)
        {
        case Keyboard::P:
        case Keyboard::Escape:
            if (!m_game_over && !m_completed)
                pause();
            break;

        case Keyboard::D:
        case Keyboard::Right:
            m_player.startWalkingRight();
            break;

        case Keyboard::A:
        case Keyboard::Left:
            m_player.startWalkingLeft();
            break;

        case Keyboard::W:
        case Keyboard::Up:
        case Keyboard::Space:
            m_player.startJumping();
            break;
        }

        break;
    }

    case Event::KeyReleased:
    {
        switch (event.key.code)
        {
        case Keyboard::D:
        case Keyboard::Right:
            m_player.stopWalkingRight();
            break;

        case Keyboard::A:
        case Keyboard::Left:
            m_player.stopWalkingLeft();
            break;

        case Keyboard::W:
        case Keyboard::Up:
        case Keyboard::Space:
            m_player.stopJumping();
            break;
        }
        break;
    }
    }
}

void Level::draw(RenderWindow& window)
{
    window.clear({ 0,0,0 });

    m_levelStructure->draw(window, m_screen_offset);
    drawCollectables(window);
    drawPlayer(window);
    m_HUD.draw(window);

    if (m_is_paused) {
        m_pauseMenu->draw(window);
        return;
    }

    if (m_gameOverAnimation != nullptr)
        m_gameOverAnimation->draw(window);

    else if (m_endAnimation != nullptr)
        m_endAnimation->draw(window);

    window.display();
}

void Level::update(const float& delta_time, Game& game)
{
    if (m_is_paused) {
        m_pauseMenu->update(delta_time, game);
        return;
    }

    if (m_game_over) {
        if (m_gameOverAnimation->animationsAreCompleted())
            game.setScene( MapFile().getMap() );

        m_gameOverAnimation->update(delta_time);
        return;
    }

    if (m_completed) {
        if (m_endAnimation->animationsAreCompleted())
            game.setScene( MapFile().getMap() );

        m_endAnimation->update(delta_time);
        return;
    }

    m_levelStructure->update(delta_time);
    updateCollectables(delta_time);
    updatePlayer(delta_time);
    updateTime(delta_time);
    checkPlayerInLimits();
    checkPlayerHitHarmful();
    
    if (playerIsInTheEnd())
        finish();

    if (m_player.getLives() == 0)
        gameOver();

    if (m_time_left <= 0)
        gameOver();
}

void Level::setCollectable(Collectable* collectable)
{
    m_collectables.push_back(collectable);
}

bool Level::playerIsInTheEnd()
{
    if (m_screen_offset + WIN_WIDTH < m_endTilePos.x)
        return false;

    auto playerHitbox = m_player.getHitboxGlobalBounds();

    FloatRect endTileHitbox;
    auto endTileIndexes = m_levelStructure->getEndTileIndexes();

    if (m_levelStructure->isValidTile(endTileIndexes.x, endTileIndexes.y))
        endTileHitbox = m_levelStructure->getTile(endTileIndexes.x, endTileIndexes.y).getHitboxGlobalBounds();
    else
        endTileHitbox = {
            m_endTilePos.x - m_screen_offset,
            m_endTilePos.y,
            TILE_SIZE,
            TILE_SIZE
        };

    return playerHitbox.intersects(endTileHitbox);
}

void Level::finish()
{
    extern Profile currentProfile;

    globalSoundsHandler.fadeOutMusic();
    m_endAnimation = new LevelEndAnimation(
        m_player.getLives(),
        m_time_left,
        m_player.getCollectableCount(Collectable::Type::Coin),
        m_player.getDragonBallCount()
    );

    currentProfile.setLevelScore(m_lvl_index, m_endAnimation->getScore());
    currentProfile.setDragonBalls(m_player.getCollectedDragonBallIndexes());
    if(!currentProfile.save())
        throw runtime_error("Ocurrió un error al intentar guardar la partida.");
    currentProfile.updateHighScores();
    m_completed = true;
}

void Level::gameOver()
{
    globalSoundsHandler.fadeOutMusic();
    m_gameOverAnimation = new GameOverAnimation();
    m_game_over = true;
}

void Level::killPlayer()
{
    m_player.kill();
    int player_lives = m_player.getLives();
    m_HUD.setLives(player_lives);
    if(player_lives > 0)
        m_player.setPosition(m_startTilePos);
}