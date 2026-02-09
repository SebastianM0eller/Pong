#include "Game.h"
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
    if (!m_Font.openFromFile("assets/font/GameFont.ttf"))
    {
        std::cerr << "Failed to load the font" << std::endl;
    }

    InitWindow(config);
    InitEntities(false);
    ScoreSystem::Initialize();
}

Game::~Game()
{
    m_Window.close();
}

void Game::InitWindow(const GameConfig &config)
{
    m_Window.create(sf::VideoMode::getDesktopMode(), config.WindowName /*, sf::State::Fullscreen*/);

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

void Game::InitEntities(bool singlePlayer)
{
    PlayerParams params;
    params.viewportSize = m_Window.getView().getSize();
    m_Player1 = Player(params);

    params.playerOne = false;
    if (singlePlayer)
        params.AI = true;
    m_Player2 = Player(params);

    m_Ball = Ball{20, m_Window.getView().getSize()};

    ButtonConfig quitConfig;
    quitConfig.text = "Quit To Menu";
    quitConfig.font = m_Font;
    quitConfig.location = {3.0f / 4.0f * m_Window.getView().getSize().x, 2.0f / 3.0f * m_Window.getView().getSize().y};
    quitConfig.textSize = m_Window.getView().getSize().y / 30;
    m_QuitButton = Button(quitConfig);

    ButtonConfig retryConfig;
    retryConfig.text = "Play Again";
    retryConfig.font = m_Font;
    retryConfig.location = {1.0f / 4.0f * m_Window.getView().getSize().x, 2.0f / 3.0f * m_Window.getView().getSize().y};
    retryConfig.textSize = m_Window.getView().getSize().y / 30;
    m_RetryButton = Button(retryConfig);

    sf::Vector2f Location = {1.0f / 2.0f * m_Window.getView().getSize().x,
                             2.0f / 5.0f * m_Window.getView().getSize().y};

    ButtonConfig playConfig;
    playConfig.text = "1 Player";
    playConfig.font = m_Font;
    playConfig.location = {Location};
    playConfig.textSize = m_Window.getView().getSize().y / 30;
    m_PlayOneButton = Button(playConfig);

    ButtonConfig twoPlayer;
    playConfig.text = "2 Player";
    playConfig.font = m_Font;
    playConfig.location = {Location + sf::Vector2f(0, playConfig.textSize * 2.0f)};
    playConfig.textSize = m_Window.getView().getSize().y / 30;
    m_PlayTwoButton = Button(playConfig);

    ButtonConfig exitConfig;
    exitConfig.text = "Quit Game";
    exitConfig.font = m_Font;
    exitConfig.location = {Location + sf::Vector2f(0, playConfig.textSize * 4.0f)};
    exitConfig.textSize = playConfig.textSize;
    m_MainQuitButton = Button(exitConfig);
}

void Game::Run()
{
    m_IsRunning = true;
    sf::Clock deltaTimer;

    while (m_IsRunning)
    {
        float deltaTime = deltaTimer.restart().asSeconds();
        switch (m_State)
        {
        case (GameState::MainMenu): {
            HandleMenuEvents();
            HandleMenuRendering();
            break;
        }
        case (GameState::Running): {
            HandleRunningEvents();
            HandleRunningMovement(deltaTime);
            HandleRunningRendering();
            CheckForWinner();
            break;
        }
        case (GameState::Winner): {
            HandleWinnerEvents();
            HandleWinnerRendering();
            break;
        }
        default:
            break;
        }
    }
};

void Game::HandleMenuEvents()
{
    while (const std::optional event = m_Window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            QuitGame();
        }

        if (const auto *mouseEvent = event->getIf<sf::Event::MouseButtonPressed>())
        {
            if (mouseEvent->button == sf::Mouse::Button::Left)
            {
                sf::Vector2f viewCoords = m_Window.mapPixelToCoords(mouseEvent->position);

                if (m_MainQuitButton.IsClicked(viewCoords))
                    QuitGame();
                else if (m_PlayOneButton.IsClicked(viewCoords))
                {
                    InitEntities(true);
                    m_State = GameState::Running;
                }
                else if (m_PlayTwoButton.IsClicked(viewCoords))
                {
                    InitEntities(false);
                    m_State = GameState::Running;
                }
            }
        }
    }
}

void Game::HandleRunningEvents()
{
    while (const std::optional event = m_Window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            QuitGame();
        }
    }
}

void Game::HandleWinnerEvents()
{
    while (const std::optional event = m_Window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            QuitGame();
        }
        if (const auto *mouseEvent = event->getIf<sf::Event::MouseButtonPressed>())
        {
            if (mouseEvent->button == sf::Mouse::Button::Left)
            {
                sf::Vector2f viewCoords = m_Window.mapPixelToCoords(mouseEvent->position);

                if (m_QuitButton.IsClicked(viewCoords))
                    QuitToMenu();
                else if (m_RetryButton.IsClicked(viewCoords))
                    RestartGame();
            }
        }
    }
}

void Game::HandleMenuRendering()
{
    m_PlayOneButton.Draw(m_Window);
    m_PlayTwoButton.Draw(m_Window);
    m_MainQuitButton.Draw(m_Window);
    m_Window.display();
    m_Window.clear(sf::Color::Black);
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
    m_QuitButton.Draw(m_Window);
    m_RetryButton.Draw(m_Window);
    m_Window.display();
    m_Window.clear(sf::Color::Black);
}

void Game::HandleRunningMovement(float deltaTime)
{
    m_Ball.HandleCollision(m_Player1);
    m_Ball.HandleCollision(m_Player2);

    m_Ball.Move(deltaTime);
    m_Player1.Update(deltaTime, m_Ball.getPosition());
    m_Player2.Update(deltaTime, m_Ball.getPosition());

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

void Game::QuitGame()
{
    m_IsRunning = false;
}

void Game::QuitToMenu()
{
    m_State = GameState::MainMenu;
    ScoreSystem::Initialize();
}

void Game::RestartGame()
{
    m_State = GameState::Running;

    m_Ball.Reset();

    m_Player1.move({0, m_Window.getView().getSize().y / 2.0f - m_Player1.getPosition().y});
    m_Player2.move({0, m_Window.getView().getSize().y / 2.0f - m_Player2.getPosition().y});

    ScoreSystem::Initialize();
}
