#include "Game.h"
#include "SFML/Graphics/Color.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "SFML/Window/WindowEnums.hpp"
#include "Score.h"
#include <format>
#include <optional>

Game::Game()
{
    InitWindow({});
    InitEntities();
    ScoreSystem::Initialize();

    if (!m_Font.openFromFile("assets/font/GameFont.ttf"))
    {
        std::cerr << "Failed to load the font" << std::endl;
    }
}

Game::Game(const GameConfig &config)
{
    InitWindow(config);
    InitEntities();
    ScoreSystem::Initialize();

    if (!m_Font.openFromFile("assets/font/GameFont.ttf"))
    {
        std::cerr << "Failed to load the font" << std::endl;
    }
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

    if (screenRatio >= gameRatio)
    {
        float width = gameRatio / screenRatio;

        view.setViewport(sf::FloatRect({(1.f - width) / 2.f, 0.f}, {width, 1.f}));
    }
    else
    {
        float height = screenRatio / gameRatio;

        view.setViewport(sf::FloatRect({0.f, (1.f - height) / 2.f}, {1.f, height}));
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

    PlayerParams params;
    params.viewportSize = windowSize;
    m_Player1 = Player(params);

    params.playerOne = false;
    m_Player2 = Player(params);

    m_Ball = Ball{20, windowSize};
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
        HandleBallPoints();
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
    DrawScore();
    m_Window.display();
}

void Game::HandleMovement(float deltaTime)
{
    m_Ball.HandleCollision(m_Player1);
    m_Ball.HandleCollision(m_Player2);

    m_Ball.Move(deltaTime);
    m_Player1.Move(deltaTime);
    m_Player2.Move(deltaTime);
}

void Game::HandleBallPoints()
{
    if (m_Ball.getPosition().x < 0)
    {
        ScoreSystem::AddScore(2);
        m_Ball.Reset();
    }

    if (m_Ball.getPosition().x > m_Window.getView().getSize().x)
    {
        ScoreSystem::AddScore(1);
        m_Ball.Reset();
    }
}

void Game::DrawScore()
{
    int Score1 = ScoreSystem::GetScore(1);
    int Score2 = ScoreSystem::GetScore(2);

    sf::Text text(m_Font, std::format("{} | {}", Score1, Score2), 30);
    text.setFillColor(sf::Color::White);

    sf::FloatRect textArea = text.getLocalBounds();
    text.setOrigin({textArea.size.x / 2.0f, textArea.size.y / 2.0f});
    text.setPosition({m_Window.getView().getSize().x / 2.0f, 40});

    m_Window.draw(text);
}
