#pragma once
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "Hitbox.h"

namespace rtd {

    class Entity
    {
        sf::Sprite m_sprite;
        rtd::Hitbox m_hitbox;
        bool m_is_custom_hitbox = false;
        bool m_is_animated = false;
        unsigned int m_animated_texture_frame = 0;
        unsigned int m_animation_frames_count = 0;
        float m_animation_duration = 0;
        float m_frame_duration = 0;
        float m_animation_elapsed_time = 0;

    protected:
        void makeItAnimated(const sf::Texture& texture, float duration);
        void unmakeAnimated();
        void centerOrigin();
        void setOrigin(float x, float y);
        void setScale(float x, float y);
        void setColor(const sf::Color& color);
        void setTexture(const sf::Texture& texture, bool resetRect = false);
        void setTextureRect(sf::IntRect rectangle);
        sf::Vector2f getScale() const;
        sf::Color getColor() const;
        const sf::Texture* getTexture();

    public:
        Entity();
        virtual void update(const float& delta_time);
        virtual void draw(sf::RenderWindow& window);
        virtual void setPosition(sf::Vector2f position);
        void setHitbox(const rtd::Hitbox& hitbox);
        inline bool isAnimated() const;
        sf::FloatRect getHitboxGlobalBounds() const;
        sf::FloatRect getGlobalBounds() const;
        const sf::Vector2f& getPosition() const;
        bool collideWith(const sf::FloatRect& boxBounds) const;
    };

}