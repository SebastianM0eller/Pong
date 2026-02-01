#include "Game.h"
#include <optional>

Game::Game()
{
    InitWindow({});
}

Game::Game(const GameConfig &config)
{
    InitWindow(config);
}

Game::~Game()
{
    Shutdown();
}

void Game::InitWindow(const GameConfig &config)
{
    m_Window.create(sf::VideoMode({config.width, config.height}), config.WindowName);
    if (config.vsync)
    {
        m_Window.setVerticalSyncEnabled(true);
    }
    else
    {
        m_Window.setFramerateLimit(config.framerate);
    }
}

void Game::Shutdown()
{
    m_Window.close();
}

void Game::Run()
{
    m_IsRunning = true;

    while (m_IsRunning)
    {
        HandleEvents();
        m_Window.display();
    }
}

void Game::HandleEvents()
{
    while (const std::optional event = m_Window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            m_IsRunning = false;
        }
    }
}
