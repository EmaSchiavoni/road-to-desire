#include "WindowSizeHandler.h"

using namespace std;
using namespace rtd;
using namespace sf;

WindowSizeHandler::WindowSizeHandler(RenderWindow& window, string title)
{
    m_window = &window;
    m_window_title = title;
    m_window->setPosition({ 0,0 });
    m_window->setSize( {VideoMode::getDesktopMode().width, VideoMode::getDesktopMode().height} );
    m_last_size = m_window->getSize();
    m_is_fullscreen = true;
}

void WindowSizeHandler::setWindow(RenderWindow& window, string title)
{
    m_window = &window;
    m_window_title = title;
    m_last_size = m_window->getSize();
    m_is_fullscreen = (m_last_size.x == VideoMode::getDesktopMode().width && m_last_size.y == VideoMode::getDesktopMode().height)? true : false;
}

void WindowSizeHandler::toggleFullscreen()
{
    if (m_is_fullscreen)
    {
        m_window->create(VideoMode(854, 480), m_window_title, sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);
        m_last_size.x = 854;
        m_last_size.y = 480;
        m_is_fullscreen = false;
    }
    else
    {
        m_window->create(VideoMode(854, 480), m_window_title, sf::Style::None);
        m_last_size.x = VideoMode::getDesktopMode().width;
        m_last_size.y = VideoMode::getDesktopMode().height;
        m_window->setSize(m_last_size);
        m_window->setPosition({ 0,0 });
        m_is_fullscreen = true;
    }
}


int WindowSizeHandler::pixelsModificated(int after, int before) const
{
    int result = after - before;
    if (result < 0)
        result = result * -1;

    return result;
}

void WindowSizeHandler::handleResizing()
{
    Vector2u current_size = m_window->getSize();
    float current_aspect_ratio = trunc(((float)current_size.x / current_size.y) * 1000) / 1000;

    if (current_aspect_ratio != m_aspect_ratio)
        if (pixelsModificated(current_size.x, m_last_size.x) > pixelsModificated(current_size.y, m_last_size.y))
            adjustVertically(current_size);
        else
            adjustHorizontally(current_size);
}

void WindowSizeHandler::adjustHorizontally(const Vector2u& current_size)
{
    m_last_size.y = current_size.y;
    m_last_size.x = trunc(current_size.y * m_aspect_ratio);
    m_window->setSize(m_last_size);
}


void WindowSizeHandler::adjustVertically(const Vector2u& current_size)
{
    m_last_size.x = current_size.x;
    m_last_size.y = trunc(current_size.x / m_aspect_ratio);
    m_window->setSize(m_last_size);
}

WindowSizeHandler::~WindowSizeHandler()
{
    m_window = nullptr;
}