#include "MovementsLimiter.h"

using namespace std;
using namespace rtd;
using namespace sf;

MovementsLimiter::MovementsLimiter(const Player& player, LevelStructure*& levelStructure) :
	m_player(player),
    m_levelStructure(levelStructure)
{}

bool MovementsLimiter::tileIntersectsProjection(int index_x, int index_y, const FloatRect& projection) const
{
    if (m_levelStructure->isValidTile(index_x, index_y) && m_levelStructure->getTile(index_x, index_y).isSolid())
        if (projection.intersects( m_levelStructure->getTile(index_x, index_y).getHitboxGlobalBounds() ))
            return true;

    return false;
}

pair<float, Vector2i> MovementsLimiter::getDistanceToNearestSolidOnTop(const float& screen_offset) const
{
    FloatRect playerHitbox = m_player.getHitboxGlobalBounds();
    FloatRect playerHitboxProjection(playerHitbox.left, 0, playerHitbox.width, playerHitbox.top);

    int playerHitboxLeftTileX = (int)trunc((screen_offset + playerHitbox.left + 1) / TILE_SIZE);
    int playerHitboxRightTileX = (int)trunc((screen_offset + playerHitbox.left + playerHitbox.width - 1) / TILE_SIZE);
    int playerHitboxTopTileY = (int)trunc((playerHitbox.top + 1) / TILE_SIZE);

    float distance = m_levelStructure->getSizeInPixels().y;
    Vector2i tileIndexes = { -1, -1 };

    for (int x = playerHitboxLeftTileX; x <= playerHitboxRightTileX; ++x)
        for (int y = playerHitboxTopTileY; y >= 0; --y)
            if (tileIntersectsProjection(x, y, playerHitboxProjection))
            {
                auto tileHitbox = m_levelStructure->getTile(x, y).getHitboxGlobalBounds();
                float new_distance = playerHitbox.top - (tileHitbox.top + tileHitbox.height);
                if (new_distance < distance) {
                    distance = new_distance;
                    tileIndexes = { x, y };
                    break;
                }
            }

    return make_pair(distance, tileIndexes);
}

pair<float, Vector2i> MovementsLimiter::getDistanceToNearestSolidOnRight(const float& screen_offset) const
{
    auto& playerPosition = m_player.getPosition();
    FloatRect playerHitbox = m_player.getHitboxGlobalBounds();
    FloatRect playerHitboxProjection;
    playerHitboxProjection.left = playerHitbox.left + playerHitbox.width;
    playerHitboxProjection.top = playerHitbox.top;
    playerHitboxProjection.width = m_levelStructure->getSizeInPixels().x - screen_offset - playerHitboxProjection.left;
    playerHitboxProjection.height = playerHitbox.height;

    int playerHitboxRightTileX = (int)trunc((screen_offset + playerHitbox.left + playerHitbox.width - 1) / TILE_SIZE);
    int playerHitboxTopTileY = (int)trunc((playerHitbox.top + 1) / TILE_SIZE);
    int playerHitboxBottomTileY = (int)trunc((playerHitbox.top + playerHitbox.height - 1) / TILE_SIZE);

    float distance = playerHitboxProjection.width;
    Vector2i tileIndexes = { -1, -1 };

    for (int y = playerHitboxTopTileY; y <= playerHitboxBottomTileY; ++y)
        for (int x = playerHitboxRightTileX; x <= m_levelStructure->getTilesCount().x - 1; ++x)
            if (tileIntersectsProjection(x, y, playerHitboxProjection))
            {
                auto tileHitbox = m_levelStructure->getTile(x, y).getHitboxGlobalBounds();
                float new_distance = tileHitbox.left - (playerHitbox.left + playerHitbox.width);
                if (new_distance < distance) {
                    distance = new_distance;
                    tileIndexes = { x, y };
                    break;
                }
            }

    return make_pair(distance, tileIndexes);
}

pair<float, Vector2i> MovementsLimiter::getDistanceToNearestSolidOnLeft(const float& screen_offset) const
{
    FloatRect playerHitbox = m_player.getHitboxGlobalBounds();
    FloatRect playerHitboxProjection(-screen_offset, playerHitbox.top, screen_offset + playerHitbox.left, playerHitbox.height);

    int playerHitboxLeftTileX = (int)trunc((screen_offset + playerHitbox.left + 1) / TILE_SIZE);
    int playerHitboxTopTileY = (int)trunc((playerHitbox.top + 1) / TILE_SIZE);
    int playerHitboxBottomTileY = (int)trunc((playerHitbox.top + playerHitbox.height - 1) / TILE_SIZE);

    float distance = playerHitboxProjection.width;
    Vector2i tileIndexes = { -1, -1 };

    for (int y = playerHitboxTopTileY; y <= playerHitboxBottomTileY; ++y)
        for (int x = playerHitboxLeftTileX; x >= 0; --x)
            if (tileIntersectsProjection(x, y, playerHitboxProjection))
            {
                auto tileHitbox = m_levelStructure->getTile(x, y).getHitboxGlobalBounds();
                float new_distance = playerHitbox.left - (tileHitbox.left + tileHitbox.width);
                if (new_distance < distance) {
                    distance = new_distance;
                    tileIndexes = { x, y };
                    break;
                }
            }

    return make_pair(distance, tileIndexes);
}

pair<float, Vector2i> MovementsLimiter::getDistanceToNearestSolidOnBottom(const float& screen_offset) const
{
    FloatRect playerHitbox = m_player.getHitboxGlobalBounds();
    FloatRect playerHitboxProjection(playerHitbox.left, playerHitbox.top + playerHitbox.height, playerHitbox.width, m_levelStructure->getSizeInPixels().y - (playerHitbox.top + playerHitbox.height));

    int playerHitboxLeftTileX = (int)trunc((screen_offset + playerHitbox.left + 1) / TILE_SIZE);
    int playerHitboxRightTileX = (int)trunc((screen_offset + playerHitbox.left + playerHitbox.width - 1) / TILE_SIZE);
    int playerHitboxBottomTileY = (int) trunc((playerHitbox.top + playerHitbox.height - 1) / TILE_SIZE);

    float distance = playerHitboxProjection.height;
    Vector2i tileIndexes = { -1, -1 };

    for (int x = playerHitboxLeftTileX; x <= playerHitboxRightTileX; ++x)
        for (int y = playerHitboxBottomTileY; y <= m_levelStructure->getTilesCount().y - 1; ++y)
            if (tileIntersectsProjection(x, y, playerHitboxProjection))
            {
                auto tileHitbox = m_levelStructure->getTile(x, y).getHitboxGlobalBounds();
                float new_distance = tileHitbox.top - (playerHitbox.top + playerHitbox.height);
                if (new_distance < distance) {
                    distance = new_distance;
                    tileIndexes = { x, y };
                    break;
                }
            }

    return make_pair(distance, tileIndexes);
}

map<Ubication, pair<float, Vector2i>> MovementsLimiter::getPlayerMovementLimits(const float& screen_offset) const
{
    map<Ubication, pair<float, Vector2i>> limits{
        {Ubication::top, getDistanceToNearestSolidOnTop(screen_offset)},
        {Ubication::right, getDistanceToNearestSolidOnRight(screen_offset)},
        {Ubication::bottom, getDistanceToNearestSolidOnBottom(screen_offset)},
        {Ubication::left, getDistanceToNearestSolidOnLeft(screen_offset)}
    };

    return limits;
}