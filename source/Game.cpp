#include "Game.h"
#include "SFML/Graphics/Color.hpp"
#include "SFML/System/Vector2.hpp"
#include "Score.h"
#include <optional>

Game::Game()
{
    InitWindow({});
    InitEntities();
}

Game::Game(const GameConfig &config)
{
    InitWindow(config);
    InitEntities();
    ScoreSystem::Initialize();
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

void Game::InitEntities()
{
    sf::Vector2f windowSize{m_Window.getSize()};
    float playerWidth = 20;
    float playerHeight = 80;
    float normalClearance = 0.05;
    float ballRadius = 10;
    sf::Vector2f ballSize{ballRadius * 2, ballRadius * 2};

    m_Player1.setOutlineColor(sf::Color::White);
    m_Player1.setSize({playerWidth, playerHeight});
    m_Player1.setPosition(NormalDeviceToRegular(windowSize, m_Player1.getSize(), {normalClearance, 0.5}));

    m_Player2.setOutlineColor(sf::Color::White);
    m_Player2.setSize({playerWidth, playerHeight});
    m_Player2.setPosition(NormalDeviceToRegular(windowSize, m_Player2.getSize(), {1 - normalClearance, 0.5}));

    m_Ball.setOutlineColor(sf::Color::White);
    m_Ball.setSize(ballSize);
    m_Ball.setPosition(NormalDeviceToRegular(windowSize, ballSize, {0.5, 0.5}));
    m_BallSpeed.x = windowSize.x / 4.0f;
}

void Game::Shutdown()
{
    m_Window.close();
}

void Game::Run()
{
    m_IsRunning = true;
    sf::Clock deltaTimer;

    while (m_IsRunning)
    {
        HandleEvents();
        HandleMovement(deltaTimer.restart().asSeconds());
        HandleRendering();
        m_Window.display();
    }
};

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

void Game::HandleRendering()
{
    m_Window.clear(sf::Color::Black);
    m_Window.draw(m_Player1);
    m_Window.draw(m_Player2);
    m_Window.draw(m_Ball);
    m_Window.display();
}

void Game::HandleMovement(float deltaTime)
{
    HandleBallCollision(m_Player1);
    HandleBallCollision(m_Player2);

    if (m_Ball.getPosition().y < 0)
        m_BallSpeed.y *= -1;
    if (m_Ball.getPosition().y > m_Window.getSize().y)
        m_BallSpeed.y *= -1;

    m_Ball.move(m_BallSpeed * deltaTime);
}

void Game::HandleBallCollision(const sf::RectangleShape &other)
{
    if (other.getGlobalBounds().findIntersection(m_Ball.getGlobalBounds()))
    {
        m_BallSpeed.x *= -1;

        m_BallSpeed.y =
            ((((m_Ball.getPosition().y + m_Ball.getSize().y / 2) - (other.getPosition().y + other.getSize().y / 2)) /
              (other.getSize().y / 2)) *
             m_BallSpeed.x);
    }
}

sf::Vector2f Game::NormalDeviceToRegular(sf::Vector2f windowSize, sf::Vector2f entitySize, sf::Vector2f location)
{
    sf::Vector2f result{windowSize.x * location.x, windowSize.y * location.y};
    result -= entitySize / 2.0f;
    return result;
}
