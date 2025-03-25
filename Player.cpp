#include "Player.h"
#include "Constants.h"
#include "SoundPaths.h"
#include "Settings.h"

using namespace std;
using namespace rtd;
using namespace sf;

Player::Player()
{
    setSounds();
    m_texture.loadFromFile("Textures/Characters/player.png");
    m_texture.setSmooth(true);
    this->setTexture(m_texture);
    this->setTextureRect({ 0, 0, m_frameSize.x, m_frameSize.y });
    this->setScale(TILE_SIZE / m_frameSize.y, TILE_SIZE / m_frameSize.y);
    m_speed = { 0,0 };
}

void Player::setSounds()
{
    extern Settings settings;
    m_steprSoundBuffer.loadFromFile( string(PLAYER_SOUNDS_PATH) + "step_r.ogg" );
    m_steplSoundBuffer.loadFromFile( string(PLAYER_SOUNDS_PATH) + "step_l.ogg" );
    m_jumpSoundBuffer.loadFromFile(string(PLAYER_SOUNDS_PATH) + "jump.ogg");
    m_killSoundBuffer.loadFromFile( string(PLAYER_SOUNDS_PATH) + "kill.ogg" );
    m_collectSoundBuffer.loadFromFile( string(PLAYER_SOUNDS_PATH) + "collect.ogg" );
    m_ballCollectSoundBuffer.loadFromFile(string(PLAYER_SOUNDS_PATH) + "ball_collected.ogg");
    m_steprSound.setBuffer(m_steprSoundBuffer);
    m_steplSound.setBuffer(m_steplSoundBuffer);
    m_jumpSound.setBuffer(m_jumpSoundBuffer);
    m_killSound.setBuffer(m_killSoundBuffer);
    m_collectSound.setBuffer(m_collectSoundBuffer);
    m_ballCollectSound.setBuffer(m_ballCollectSoundBuffer);
    setSoundsVolume(settings.getSoundsVolume());
}

void Player::setSoundsVolume(float volume)
{
    m_steprSound.setVolume(volume);
    m_steplSound.setVolume(volume);
    m_jumpSound.setVolume(volume);
    m_killSound.setVolume(volume);
    m_collectSound.setVolume(volume);
    m_ballCollectSound.setVolume(volume);
}

void Player::falling(const float& delta_time)
{
    m_speed.y += 1000 * delta_time;
    if (m_speed.y > 1000)
        m_speed.y = 1000;
}

void Player::stopFalling()
{
    if (m_speed.y > 0)
        m_speed.y = 0;
}

void Player::updateHitTilesIndexes(const Vector2f& movementSize, const Vector2<Ubication>& direction, const map<Ubication, pair<float, Vector2i>>& movementLimits)
{
    m_hitTilesIndexes.clear();
    if (movementSize.x == movementLimits.at(direction.x).first) //Si colisionó un tile en x
        if (movementLimits.at(direction.x).second.x >= 0 && movementLimits.at(direction.x).second.y >= 0) //Si es -1 es un borde del nivel.
            setHitTile(Vector2u(
                movementLimits.at(direction.x).second.x,
                movementLimits.at(direction.x).second.y
            ), direction.x);

    if (movementSize.y == movementLimits.at(direction.y).first) //Si colisionó un tile en y
        if (movementLimits.at(direction.y).second.x >= 0 && movementLimits.at(direction.y).second.y >= 0)
            setHitTile(Vector2u(
                movementLimits.at(direction.y).second.x,
                movementLimits.at(direction.y).second.y
            ), direction.y);
}

void Player::setHitTile(const Vector2u& indexes, const Ubication& side)
{
    m_hitTilesIndexes.push_back( make_pair(indexes, side) );
}

const Vector2f& Player::getPosition() const
{
    return m_position;
}

const Vector2f& Player::getSpeed() const
{
    return m_speed;
}

int Player::getLives() const
{
    return m_lives;
}

int Player::getCollectableCount(Collectable::Type type) const
{
    return (m_collectables.count(type))? m_collectables.at(type) : 0;
}

int Player::getDragonBallCount() const
{
    int count = 0;

    for (auto collectable = m_collectables.begin(); collectable != m_collectables.end(); ++collectable)
        if (getCollectableId(collectable->first) >= 1 &&
            getCollectableId(collectable->first) <= 7)
            ++count;

    return count;
}

vector<int> Player::getCollectedDragonBallIndexes() const
{
    vector<int> collectedBalls;

    for (auto collectable = m_collectables.begin(); collectable != m_collectables.end(); ++collectable) {
        int collectableId = getCollectableId(collectable->first);
        if (collectableId >= 1 && collectableId <= 7)
            collectedBalls.push_back(collectableId);
    }

    return collectedBalls;
}

vector<unsigned int> Player::getOccuppiedTilesIndexesInX() const
{
    vector<unsigned int> occuppiedTiles;
    unsigned int left_tile = (int)floor(m_position.x / TILE_SIZE);
    unsigned int right_tile = (int)floor((m_position.x + getGlobalBounds().width - 1) / TILE_SIZE);

    for (auto tile_index = left_tile; tile_index <= right_tile; ++tile_index)
        occuppiedTiles.push_back(tile_index);

    return occuppiedTiles;
}

vector<unsigned int> Player::getOccuppiedTilesIndexesInY() const
{
    vector<unsigned int> occuppiedTiles;
    int top_tile = (int)floor(m_position.y / TILE_SIZE);
    int bottom_tile = (int)floor((m_position.y + getGlobalBounds().height - 1) / TILE_SIZE);

    if (top_tile < 0) top_tile = 0;
    if (bottom_tile < 0) top_tile = 0;

    for (auto tile_index = top_tile; tile_index <= bottom_tile; ++tile_index)
        occuppiedTiles.push_back(tile_index);

    return occuppiedTiles;
}

Vector2<vector<unsigned int>> Player::getOccuppiedTilesIndexes() const
{
    return { getOccuppiedTilesIndexesInX(), getOccuppiedTilesIndexesInY() };
}

const vector< pair<Vector2u, Ubication> >& Player::getTilesHitData() const
{
    return m_hitTilesIndexes;
}

void Player::updatePosition(Vector2f movementSize, const map<Ubication, pair<float, Vector2i>>& movementLimits)
{
    Vector2<Ubication> direction(
        (m_speed.x > 0) ? Ubication::right : Ubication::left,
        (m_speed.y < 0) ? Ubication::top : Ubication::bottom
    );

    movementSize.x = (movementSize.x <= movementLimits.at(direction.x).first) ? movementSize.x : movementLimits.at(direction.x).first;
    movementSize.y = (movementSize.y <= movementLimits.at(direction.y).first) ? movementSize.y : movementLimits.at(direction.y).first;

    m_position.x += (direction.x == Ubication::right) ? movementSize.x : -movementSize.x;
    m_position.y += (direction.y == Ubication::bottom) ? movementSize.y : -movementSize.y;

    updateHitTilesIndexes(movementSize, direction, movementLimits);
}

void Player::updateSpeedX()
{
    m_speed.x = 0;

    if (m_walking_right)
        m_speed.x += m_max_speed;

    if (m_walking_left)
        m_speed.x -= m_max_speed;

    if (m_speed.x > m_max_speed)
        m_speed.x = m_max_speed;
    
    if (m_speed.x < -m_max_speed)
        m_speed.x = -m_max_speed;

    if (m_speed.y != 0)
        m_speed.x += m_speed.x * .1;
}

void Player::updateSpeedY(const float& delta_time, const map<Ubication, pair<float, Vector2i>>& movementLimits)
{
    if (m_jumping && m_speed.y == 0 && movementLimits.at(Ubication::bottom).first == 0)
        jump();

    for (const auto& hitTileData : m_hitTilesIndexes)
        if (hitTileData.second == Ubication::top)
            m_speed.y = 0;

    if (movementLimits.at(Ubication::bottom).first != 0)
        falling(delta_time);
    else
        stopFalling();
}

void Player::applyAnimationFrame()
{
    this->setTextureRect(IntRect(
        m_frameSize.x * m_currentAnimFrame.x,
        m_frameSize.y * m_currentAnimFrame.y,
        m_frameSize.x,
        m_frameSize.y
    ));
}

void Player::updateAnimationFrame(const float& delta_time)
{
    m_animation_elapsed_time += delta_time;

    if (m_speed.y != 0) {
        m_currentAnimFrame.y = (m_speed.x >= 0) ? 2 : 3;
        m_currentAnimFrame.x = 1;
        m_animation_elapsed_time = 0;
        return;
    }

    if (m_walking_right) {
        if (m_currentAnimFrame.y == 0)
            m_animation_elapsed_time = (m_animation_elapsed_time > m_running_animation_duration) ? 0 : m_animation_elapsed_time;
        else {
            m_currentAnimFrame.y = 0;
            m_animation_elapsed_time = 0;
        }
        m_currentAnimFrame.x = (int)floor(m_animation_elapsed_time / m_frame_duration) + 1;
        return;
    }

    if (m_walking_left) {
        if (m_currentAnimFrame.y == 1)
            m_animation_elapsed_time = (m_animation_elapsed_time > m_running_animation_duration) ? 0 : m_animation_elapsed_time;
        else {
            m_currentAnimFrame.y = 1;
            m_animation_elapsed_time = 0;
        }
        m_currentAnimFrame.x = (int)floor(m_animation_elapsed_time / m_frame_duration) + 1;
        return;
    }

    if (m_currentAnimFrame.y > 1 && m_speed.y == 0) { //Si no esta en el aire y el frame actual no es caminando/parado
        m_currentAnimFrame.x = 0;
        if (m_animation_elapsed_time > .2)
            m_currentAnimFrame = { 0, m_currentAnimFrame.y - 2 };
    }
}

void Player::playStepsSound(Vector2f movementSize)
{
    m_distanceRun += movementSize.x;

    if (movementSize.y == 0 && m_distanceRun > 25) {
        m_distanceRun = 0;
        if (rand() % 3 > 1)
            m_steprSound.play();
        else
            m_steplSound.play();
    }
}

void Player::update(const float& delta_time, const map<Ubication, pair<float, Vector2i>>& movementLimits)
{
    Vector2f movementSize(
        abs(delta_time * m_speed.x),
        abs(delta_time * m_speed.y)
    );
    updatePosition(movementSize, movementLimits);
    updateSpeedX();
    updateSpeedY(delta_time, movementLimits);
    updateAnimationFrame(delta_time);
    applyAnimationFrame();
    playStepsSound(movementSize);
}

void Player::draw(RenderWindow& window, const Vector2f& pos)
{
    Entity::setPosition(pos);
    Entity::draw(window);
}

void Player::setPosition(Vector2f globalPosition)
{
    m_position = globalPosition;
    m_hitTilesIndexes.clear();
}

void Player::startWalkingRight()
{
    m_walking_right = true;
}

void Player::startWalkingLeft()
{
    m_walking_left = true;
}

void Player::stopWalkingRight()
{
    m_walking_right = false;
    m_currentAnimFrame = {0, 0};
    applyAnimationFrame();
}

void Player::stopWalkingLeft()
{
    m_walking_left = false;
    m_currentAnimFrame = { 0, 1 };
    applyAnimationFrame();
}

void Player::startJumping()
{
    m_jumping = true;
}

void Player::stopJumping()
{
    m_jumping = false;
    m_currentAnimFrame.x = 0;
    applyAnimationFrame();
}

void Player::jump()
{
    m_speed.y -= 400;
    m_jumpSound.play();
}

void Player::kill()
{
    --m_lives;
    m_killSound.play();
}

void Player::setCollectable(Collectable::Type type, int count)
{
    int current_count = (m_collectables.count(type)) ? m_collectables[type] : 0;
    m_collectables[type] = current_count + count;

    if (getCollectableId(type) >= 1 && getCollectableId(type) <= 7)
        m_ballCollectSound.play();
    else
        m_collectSound.play();
}