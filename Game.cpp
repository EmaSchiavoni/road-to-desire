#include <cmath>
#include <SFML/Window/WindowHandle.hpp>
#include "Game.h"
#include "MainMenu.h"

using namespace std;
using namespace rtd;
using namespace sf;

Game::Game() :
    m_title(WIN_TITLE),
    m_sizeHandler(m_window, m_title),
    m_window(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), m_title, sf::Style::None)
{
    m_window.setMouseCursorVisible(false);
    m_window.setVerticalSyncEnabled(true);
    m_window.setTitle(m_title);
    m_currentScene = new MainMenu();
}

void Game::run()
{
    try {
        Clock clock;
        float delta_time = 0;

        while (m_window.isOpen()) {
            processEvents();
            update(delta_time);
            draw();
            if (m_nextScene != nullptr) {
                delete m_currentScene;
                m_currentScene = m_nextScene;
                m_nextScene = nullptr;
            }
            delta_time = clock.restart().asSeconds();
        }
    }
    catch (runtime_error& e) {
        throw e;
    }
}

void Game::processEvents()
{
    Event event;
    while (m_window.pollEvent(event)){

        switch (event.type){
        case Event::Closed:
            quit();
            break;

        case Event::Resized:
            m_sizeHandler.handleResizing();
            break;

        case Event::KeyReleased:
            switch (event.key.code){
            case Keyboard::F11:
                m_sizeHandler.toggleFullscreen();
                break;
            }
            break;
        }
        m_currentScene->processEvents(event);
    }
}

void Game::update(const float& delta_time)
{
    m_currentScene->update(delta_time, *this);
}

void Game::draw()
{
    m_currentScene->draw(m_window);
}

void Game::setScene(Scene* next_scene)
{
    m_nextScene = next_scene;
}

void Game::quit()
{
    m_window.close();
}

Game::~Game()
{
    delete m_currentScene;
}