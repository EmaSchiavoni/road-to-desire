#include "Entity.h"
#include "Constants.h"

using namespace std;
using namespace rtd;
using namespace sf;

Entity::Entity()
{}

void Entity::makeItAnimated(const Texture& texture, float duration)
{
    m_is_animated = true;
    m_animation_duration = duration;
    m_animation_frames_count = texture.getSize().x / texture.getSize().y;
    m_frame_duration = m_animation_duration / m_animation_frames_count;

    float ex_texture_tile_size = (m_sprite.getTexture() != nullptr) ? m_sprite.getTexture()->getSize().y : TILE_SIZE;
    float size = ex_texture_tile_size * m_sprite.getScale().y;
    float scale = size / texture.getSize().y;
    m_sprite.setTexture(texture, true);
    m_sprite.setScale(scale, scale);
    m_sprite.setTextureRect({0, 0, (int)texture.getSize().y, (int)texture.getSize().y });
}

void Entity::unmakeAnimated()
{
    m_is_animated = false;
    m_animated_texture_frame = 0;
    m_animation_frames_count = 0;
    m_animation_duration = 0;
    m_frame_duration = 0;
    m_animation_elapsed_time = 0;
}

void Entity::centerOrigin()
{
    auto bounds = m_sprite.getLocalBounds();
    m_sprite.setOrigin(
        bounds.height / 2,
        bounds.width / 2
    );
}


void Entity::setOrigin(float x, float y)
{
    m_sprite.setOrigin(x, y);
}

void Entity::setScale(float x, float y)
{
    m_sprite.setScale(x, y);
}

void Entity::setColor(const Color& color)
{
    m_sprite.setColor(color);
}

void Entity::setTexture(const Texture& texture, bool resetRect)
{
    m_sprite.setTexture(texture, resetRect);
}

void Entity::setTextureRect(IntRect rectangle)
{
    m_sprite.setTextureRect(rectangle);
}

Vector2f Entity::getScale() const
{
    return m_sprite.getScale();
}

Color Entity::getColor() const
{
    return m_sprite.getColor();
}

const Texture* Entity::getTexture()
{
    return m_sprite.getTexture();
}

void Entity::update(const float& delta_time)
{
    if (isAnimated()) {
        m_animation_elapsed_time += delta_time;
        if (m_animation_elapsed_time > m_animation_duration)
            m_animation_elapsed_time = 0;

        m_animated_texture_frame = (int)floor(m_animation_elapsed_time / m_frame_duration);

        int tile_size_in_texture = m_sprite.getTexture()->getSize().y;
        const auto& current_rect = m_sprite.getTextureRect();

        m_sprite.setTextureRect(IntRect(
            tile_size_in_texture * m_animated_texture_frame,
            current_rect.top,
            tile_size_in_texture,
            tile_size_in_texture
        ));
    }
}

void Entity::draw(sf::RenderWindow& window)
{
    window.draw(m_sprite);
}

void Entity::setPosition(Vector2f position)
{
    m_sprite.setPosition(position);
}

void Entity::setHitbox(const Hitbox& localHitboxBounds)
{
    m_hitbox = localHitboxBounds;
    m_is_custom_hitbox = true;
}

inline bool Entity::isAnimated() const
{
    return m_is_animated;
}

FloatRect Entity::getHitboxGlobalBounds() const
{
    FloatRect spriteBounds = m_sprite.getGlobalBounds();
    FloatRect hitboxGlobalBounds(
        spriteBounds.left + m_hitbox.left,
        spriteBounds.top + m_hitbox.top,
        spriteBounds.width * m_hitbox.widthFactor,
        spriteBounds.height * m_hitbox.heightFactor
    );

    return hitboxGlobalBounds;
}

FloatRect Entity::getGlobalBounds() const
{
    return m_sprite.getGlobalBounds();
}

const sf::Vector2f& Entity::getPosition() const
{
    return m_sprite.getPosition();
}

bool Entity::collideWith(const FloatRect& boxBounds) const
{
    return getHitboxGlobalBounds().intersects(boxBounds);
};