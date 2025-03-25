#pragma once
#include <string>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "Constants.h"
#include "Scene.h"
#include "WindowSizeHandler.h"

namespace rtd {

    class Game
    {
        sf::RenderWindow m_window;
        std::string m_currentMusicPath;
        std::string m_title;
        rtd::WindowSizeHandler m_sizeHandler;
        rtd::Scene* m_currentScene = nullptr;
        rtd::Scene* m_nextScene = nullptr;

    public:
        Game();
        void run();
        void processEvents();
        void update(const float& delta_time);
        void draw();
        void setScene(rtd::Scene* next_scene);

        void quit();
        ~Game();
    };

}