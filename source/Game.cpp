#include "Game.h"

#include "Score.h"
#include "SoundSystem.h"

///
///  Constructs a new Game, based on the default GameConfig.
///  Throws a runtime_error if Initialization fails.
///
Game::Game() {
        GameConfig config;
        Init(config);
}

///
/// Constructs a new Game, based on the specified GameConfig.
/// Throws a runtime_error if Initialization fails.
///
Game::Game(const GameConfig& config) { Init(config); }

///
/// Initialize the different things, needed for the game.
/// If it fails to load the font, it throws a runtime_error.
///
void Game::Init(const GameConfig& config) {
        // Load the font first, as it's used for the entities.
        if (!m_Font.openFromFile("assets/font/GameFont.ttf")) {
                throw std::runtime_error("Failed to load the font");
        }

        ScoreSystem::Initialize();
        SoundSystem::Init();
        InitWindow(config);  // We init the Window before the menu, as it depends on the window size.
        InitMenu();          // We init the menu, as it's the default state.
}

///
/// Initializes the internal SFML window, with the provided config.
/// Creates the window view based on the config, and applies the appropriate black bars.
///
void Game::InitWindow(const GameConfig& config) {
        m_Window.create(sf::VideoMode::getDesktopMode(), config.WindowName /*, sf::State::Fullscreen*/);
        sf::View view(sf::FloatRect({0, 0}, static_cast<sf::Vector2f>(config.WindowSize)));

        float screenRatio = (float)m_Window.getSize().x / m_Window.getSize().y;
        float gameRatio = (float)config.WindowSize.x / config.WindowSize.y;

        if (screenRatio >= gameRatio) {
                float width = gameRatio / screenRatio;

                view.setViewport(sf::FloatRect({(1.f - width) / 2.f, 0.f}, {width, 1.f}));
        } else {
                float height = screenRatio / gameRatio;

                view.setViewport(sf::FloatRect({0.f, (1.f - height) / 2.f}, {1.f, height}));
        }

        m_Window.setView(view);

        (config.vsync) ? m_Window.setVerticalSyncEnabled(true) : m_Window.setFramerateLimit(config.framerate);
}

///
/// Initializes the members required for the main menu.
///
void Game::InitMenu() {
        sf::Vector2f location = {1.0f / 2.0f * m_Window.getView().getSize().x,
                                 2.0f / 5.0f * m_Window.getView().getSize().y};

        uint8_t fontSize = m_Window.getView().getSize().y / 30;

        m_PlayOneButton = Button({"Single Player", m_Font, location, fontSize});
        m_PlayTwoButton = Button({"Two Player", m_Font, {location.x, location.y + fontSize * 2.0f}, fontSize});
        m_MainQuitButton = Button({"Quit Game", m_Font, {location.x, location.y + fontSize * 4.0f}, fontSize});
}

///
/// Initializes the members required for the main game.
///
void Game::InitRunning(const bool singlePlayer) {
        sf::Vector2f viewportSize = m_Window.getView().getSize();

        m_Player1 = Player({viewportSize, {20, 80}, true, false});
        m_Player2 = Player({viewportSize, {20, 80}, false, singlePlayer});

        m_Ball = Ball(20, viewportSize);
}

///
/// Initializes the members required for the winning menu.
///
void Game::InitWinner() {
        sf::Vector2f viewportSize = m_Window.getView().getSize();

        sf::Vector2f retryLocation = {1.0f / 4.0f * viewportSize.x, 2.0f / 3.0f * viewportSize.y};
        sf::Vector2f quitLocation = {3.0f / 4.0f * viewportSize.x, 2.0f / 3.0f * viewportSize.y};

        uint8_t fontSize = m_Window.getView().getSize().y / 30;

        m_RetryButton = Button({"Play Again", m_Font, retryLocation, fontSize});
        m_QuitButton = Button({"Quit to Menu", m_Font, quitLocation, fontSize});
}

///
/// Set m_IsRunning to true, and starts the main game loop.
/// The loop that runs, depends on the GameState.
/// At the end of each loop, the SoundSystem::Update() is called, to free memory held by finished sounds.
///
void Game::Run() {
        m_IsRunning = true;
        sf::Clock deltaTimer;

        while (m_IsRunning) {
                float deltaTime = deltaTimer.restart().asSeconds();
                switch (m_State) {
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

///
/// Poll and handle the events for the main menu.
/// It handles the following events:
/// - Event::Closed -> Stops the main loop by settings m_IsRunning to false.
/// - Event::MouseButtonPressed -> Checks if one of the buttons is clicked, and if so runs the appropriate methods.
///
void Game::HandleMenuEvents() {
        while (const std::optional event = m_Window.pollEvent()) {
                if (event->is<sf::Event::Closed>()) {
                        QuitGame();
                }

                if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
                        if (mouseEvent->button == sf::Mouse::Button::Left) {
                                sf::Vector2f viewCoords = m_Window.mapPixelToCoords(mouseEvent->position);

                                if (m_MainQuitButton.IsClicked(viewCoords))
                                        QuitGame();
                                else if (m_PlayOneButton.IsClicked(viewCoords)) {
                                        InitRunning(true);
                                        m_State = GameState::Running;
                                } else if (m_PlayTwoButton.IsClicked(viewCoords)) {
                                        InitRunning(false);
                                        m_State = GameState::Running;
                                }
                        }
                }
        }
}

///
/// Poll and handle the events for the main menu.
/// It handles the following events:
/// - Event::Closed -> Stops the main loop by settings m_IsRunning to false.
///
void Game::HandleRunningEvents() {
        while (const std::optional event = m_Window.pollEvent()) {
                if (event->is<sf::Event::Closed>()) {
                        QuitGame();
                }
        }
}

///
/// Poll and handle the events for the main menu.
/// It handles the following events:
/// - Event::Closed -> Stops the main loop by settings m_IsRunning to false.
/// - Event::MouseButtonPressed -> Checks if one of the buttons is clicked, and if so runs the appropriate methods.
///
void Game::HandleWinnerEvents() {
        while (const std::optional event = m_Window.pollEvent()) {
                if (event->is<sf::Event::Closed>()) {
                        QuitGame();
                }
                if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
                        if (mouseEvent->button == sf::Mouse::Button::Left) {
                                sf::Vector2f viewCoords = m_Window.mapPixelToCoords(mouseEvent->position);

                                if (m_QuitButton.IsClicked(viewCoords))
                                        QuitToMenu();
                                else if (m_RetryButton.IsClicked(viewCoords))
                                        RestartGame();
                        }
                }
        }
}

///
/// Draws the entities in the menu, to the RenderWindow.
/// Display the entities, and clears the buffer with the color black.
///
void Game::HandleMenuRendering() {
        m_PlayOneButton.Draw(m_Window);
        m_PlayTwoButton.Draw(m_Window);
        m_MainQuitButton.Draw(m_Window);
        m_Window.display();
        m_Window.clear(sf::Color::Black);
}

///
/// Draws the entities in the main game to the RenderWindow.
/// Swaps buffers, and clears the buffer with the color black.
///
void Game::HandleRunningRendering() {
        ScoreSystem::DrawScore(m_Window, m_Font);
        m_Window.draw(m_Player1);
        m_Window.draw(m_Player2);
        m_Window.draw(m_Ball);
        m_Window.display();
        m_Window.clear(sf::Color::Black);
}

///
/// Draws the entities and the text from the winning screen to the RenderWindow.
/// Swaps the buffer for the window, and clears the buffer with the color black.
///
void Game::HandleWinnerRendering() {
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

///
/// Calls the update methods for the entities in the main game.
/// Also handle the collision checks.
///
void Game::HandleRunningMovement(float deltaTime) {
        m_Ball.HandleCollision(m_Player1);
        m_Ball.HandleCollision(m_Player2);

        m_Ball.Move(deltaTime);
        m_Player1.Update(deltaTime, m_Ball.getPosition());
        m_Player2.Update(deltaTime, m_Ball.getPosition());
}

///
/// Handles the score for the game.
/// If the score is large enough for a player to win, the m_State is changed to Winner, and we set the appropriate
/// m_Winner.
/// When there is a winner, the Winner entities are also Initialized.
///
void Game::CheckForWinner() {
        m_Ball.HandleScore();

        if (ScoreSystem::GetScore(1) >= 10) {
                m_State = GameState::Winner;
                m_Winner = 1;
                InitWinner();
        }

        if (ScoreSystem::GetScore(2) >= 10) {
                m_State = GameState::Winner;
                m_Winner = 2;
                InitWinner();
        }
}

///
/// Sets m_IsRunning to false
///
void Game::QuitGame() { m_IsRunning = false; }

///
/// Sets the GameState to MainMenu, and resets the score.
///
void Game::QuitToMenu() {
        m_State = GameState::MainMenu;
        ScoreSystem::Initialize();
}

///
/// Set the GameState to Running, resets the score, and places the ball and players back to their initial state.
///
void Game::RestartGame() {
        m_State = GameState::Running;

        m_Ball.Reset();

        m_Player1.move({0, m_Window.getView().getSize().y / 2.0f - m_Player1.getPosition().y});
        m_Player2.move({0, m_Window.getView().getSize().y / 2.0f - m_Player2.getPosition().y});

        ScoreSystem::Initialize();
}
