#include "Game.h"

#include <format>
#include <optional>

#include "SFML/Graphics/Text.hpp"
#include "Score.h"

Game::Game()
{
    GameConfig config;
    Init(config);
}

Game::Game(const GameConfig &config)
{
    Init(config);
}

void Game::Init(const GameConfig &config)
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
    m_Window.close();
}

void Game::InitWindow(const GameConfig &config)
{
    m_Window.create(sf::VideoMode::getDesktopMode(), config.WindowName, sf::State::Fullscreen);

    float screenRatio = (float)m_Window.getSize().x / m_Window.getSize().y;
    float gameRatio = (float)config.width / config.height;
    sf::View view(sf::FloatRect({0, 0}, {(float)config.width, (float)config.height}));

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

    (config.vsync) ? m_Window.setVerticalSyncEnabled(true) : m_Window.setFramerateLimit(config.framerate);
}

void Game::InitEntities()
{
    PlayerParams params;
    params.viewportSize = m_Window.getView().getSize();
    m_Player1 = Player(params);

    params.playerOne = false;
    m_Player2 = Player(params);

    m_Ball = Ball{20, m_Window.getView().getSize()};
}

void Game::Run()
{
    m_IsRunning = true;
    sf::Clock deltaTimer;

    while (m_IsRunning)
    {
        switch (m_State)
        {
        case (GameState::MainMenu): {
            continue;
        }
        case (GameState::Running): {
            float deltaTime = deltaTimer.restart().asSeconds();
            HandleRunningEvents();
            HandleRunningMovement(deltaTime);
            HandleRunningRendering();
            CheckForWinner();
            continue;
        }
        case (GameState::Winner): {
            HandleWinnerEvents();
            HandleWinnerRendering();
        }
        }
    }
};

void Game::HandleRunningEvents()
{
    while (const std::optional event = m_Window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            m_IsRunning = false;
        }
    }
}

void Game::HandleWinnerEvents()
{
    HandleRunningEvents();
}

void Game::HandleRunningRendering()
{
    ScoreSystem::DrawScore(m_Window, m_Font);
    m_Window.draw(m_Player1);
    m_Window.draw(m_Player2);
    m_Window.draw(m_Ball);
    m_Window.display();
    m_Window.clear(sf::Color::Black);
}

void Game::HandleWinnerRendering()
{
    sf::Text text(m_Font, std::format("{} {}", "The winner is: player ", m_Winner));
    text.setFillColor(sf::Color::White);

    sf::FloatRect textArea = text.getLocalBounds();
    text.setOrigin({textArea.size.x / 2.0f, textArea.size.y / 2.0f});
    text.setPosition(m_Window.getView().getSize() / 2.0f);

    m_Window.draw(text);
    m_Window.display();
    m_Window.clear(sf::Color::Black);
}

void Game::HandleRunningMovement(float deltaTime)
{
    m_Ball.HandleCollision(m_Player1);
    m_Ball.HandleCollision(m_Player2);

    m_Ball.Move(deltaTime);
    m_Player1.Move(deltaTime);
    m_Player2.Move(deltaTime);

    m_Ball.HandleScore();
}

void Game::CheckForWinner()
{
    if (ScoreSystem::GetScore(1) >= 10)
    {
        m_State = GameState::Winner;
        m_Winner = 1;
    }

    if (ScoreSystem::GetScore(2) >= 10)
    {
        m_State = GameState::Winner;
        m_Winner = 2;
    }
}
