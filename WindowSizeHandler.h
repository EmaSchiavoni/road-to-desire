#pragma once
#include <string>
#include <SFML/Graphics/RenderWindow.hpp>
#include "Constants.h"

namespace rtd {

    class WindowSizeHandler
    {
        float m_aspect_ratio = rtd::WIN_WIDTH / rtd::WIN_HEIGHT;
        bool m_is_fullscreen;
        sf::RenderWindow* m_window;
        std::string m_window_title;
        sf::Vector2u m_last_size;

        int pixelsModificated(int after, int before) const;
    public:
        WindowSizeHandler(sf::RenderWindow& window, std::string title);
        void setWindow(sf::RenderWindow& window, std::string title);
        void toggleFullscreen();
        void handleResizing();
        void adjustHorizontally(const sf::Vector2u& current_size);
        void adjustVertically(const sf::Vector2u& current_size);
        ~WindowSizeHandler();
    };

}