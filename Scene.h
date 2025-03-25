#pragma once
#include <SFML/Graphics/RenderWindow.hpp>

namespace rtd {

    class Game;

    class Scene
    {
    public:
        Scene();
        virtual void draw(sf::RenderWindow& window) = 0;
        virtual void update(const float& delta_time, rtd::Game& game) = 0;
        virtual void processEvents(sf::Event event) = 0;
        virtual ~Scene();
    };

}