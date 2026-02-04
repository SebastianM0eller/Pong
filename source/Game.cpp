#include "Game.h"
#include "SFML/Graphics/Color.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "SFML/Window/WindowEnums.hpp"
#include "Score.h"
#include <optional>

Game::Game()
{
    InitWindow({});
    InitEntities();
    ScoreSystem::Initialize();
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
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    m_Window.create(desktop, config.WindowName, sf::State::Fullscreen);

    sf::View view(sf::FloatRect({0, 0}, {(float)config.width, (float)config.height}));

    float screenRatio = static_cast<float>(m_Window.getSize().x) / static_cast<float>(m_Window.getSize().y);
    float gameRatio = static_cast<float>(config.width) / static_cast<float>(config.height);

    std::cout << "SR = " << screenRatio << " and GR = " << gameRatio;

    if (screenRatio >= gameRatio)
    {
        float width = gameRatio / screenRatio;

        view.setViewport(sf::FloatRect({(1.f - width) / 2.f, 0.f}, {width, 1.f}));
        std::cout << " screen>game";
    }
    else
    {
        float height = screenRatio / gameRatio;

        view.setViewport(sf::FloatRect({0.f, (1.f - height) / 2.f}, {1.f, height}));
        std::cout << " screen<game ";
    }

    m_Window.setView(view);

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
    sf::Vector2f windowSize{m_Window.getView().getSize()};
    float playerWidth = 20;
    float playerHeight = 80;
    float normalClearance = 0.05;
    float ballRadius = 10;
    sf::Vector2f ballSize{ballRadius * 2, ballRadius * 2};

    m_Player1.setOutlineColor(sf::Color::White);
    m_Player1.setSize({playerWidth, playerHeight});
    m_Player1.setPosition(NormalDeviceToRegular(windowSize, m_Player1.getSize(), {normalClearance, 0.5}));
    m_Player1.setOutlineThickness(1.0f);

    m_Player2.setOutlineColor(sf::Color::White);
    m_Player2.setSize({playerWidth, playerHeight});
    m_Player2.setPosition(NormalDeviceToRegular(windowSize, m_Player2.getSize(), {1 - normalClearance, 0.5}));

    m_Ball.setOutlineColor(sf::Color::White);
    m_Ball.setSize(ballSize);
    m_Ball.setPosition(NormalDeviceToRegular(windowSize, ballSize, {0.5, 0.5}));
    m_BallSpeed.x = windowSize.x / 2.0f;
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
        float deltaTime = deltaTimer.restart().asSeconds();
        HandleEvents();
        HandleMovement(deltaTime);
        HandleRendering();
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

    if (m_Ball.getPosition().y < 0 && m_BallSpeed.y < 0)
    {
        m_BallSpeed.y *= -1;
        std::cout << "Hit buttom";
    }
    if (m_Ball.getPosition().y + m_Ball.getSize().y > m_Window.getView().getSize().y && m_BallSpeed.y > 0)
    {
        m_BallSpeed.y *= -1;
        std::cout << "Hit buttom" << "\n";
        std::cout << m_Ball.getPosition().y << "\n";
        std::cout << m_Window.getSize().y << "\n";
    }

    m_Ball.move(m_BallSpeed * deltaTime);

    constexpr sf::Keyboard::Key P1MoveUp = sf::Keyboard::Key::W;
    constexpr sf::Keyboard::Key P1MoveDown = sf::Keyboard::Key::S;
    constexpr sf::Keyboard::Key P2MoveUp = sf::Keyboard::Key::I;
    constexpr sf::Keyboard::Key P2MoveDown = sf::Keyboard::Key::K;

    float VerticalSpeedMultiplier{0.5};

    if (sf::Keyboard::isKeyPressed(P1MoveDown))
        m_Player1.move({0, static_cast<float>(m_Window.getSize().y) * deltaTime * VerticalSpeedMultiplier});
    if (sf::Keyboard::isKeyPressed(P1MoveUp))
        m_Player1.move({0, -static_cast<float>(m_Window.getSize().y * deltaTime * VerticalSpeedMultiplier)});

    if (sf::Keyboard::isKeyPressed(P2MoveDown))
        m_Player2.move({0, static_cast<float>(m_Window.getSize().y * deltaTime * VerticalSpeedMultiplier)});
    if (sf::Keyboard::isKeyPressed(P2MoveUp))
        m_Player2.move({0, -static_cast<float>(m_Window.getSize().y * deltaTime * VerticalSpeedMultiplier)});
}

void Game::HandleBallCollision(const sf::RectangleShape &other)
{
    if (other.getGlobalBounds().findIntersection(m_Ball.getGlobalBounds()))
    {
        m_BallSpeed.x *= -1;

        m_BallSpeed.y =
            (((m_Ball.getPosition().y + m_Ball.getSize().y / 2) - (other.getPosition().y + other.getSize().y / 2)) /
             (other.getSize().y / 2)) *
            m_BallSpeed.x;

        // Correct the direction, in case the ball speed is negative
        if (m_BallSpeed.x < 0)
            m_BallSpeed.y *= -1;
    }
}

sf::Vector2f Game::NormalDeviceToRegular(sf::Vector2f windowSize, sf::Vector2f entitySize, sf::Vector2f location)
{
    sf::Vector2f result{windowSize.x * location.x, windowSize.y * location.y};
    result -= entitySize / 2.0f;
    return result;
}
