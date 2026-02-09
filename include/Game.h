#pragma once
#include "Ball.h"
#include "Button.h"
#include "Players.h"

struct GameConfig
{
    const char *WindowName{"SFML Window"};
    uint16_t width{800};
    uint16_t height{600};
    uint16_t framerate{60};
    bool vsync{true};
};

enum class GameState
{
    MainMenu,
    Running,
    Winner
};

class Game
{
  public:
    Game();
    Game(const GameConfig &config);
    ~Game();

    void Run();

  private:
    // Members needed for all states:
    sf::RenderWindow m_Window;
    sf::Font m_Font;
    GameState m_State{GameState::MainMenu};
    bool m_IsRunning{false};

    // Members needed for the menu.
    Button m_PlayOneButton;
    Button m_PlayTwoButton;
    Button m_MainQuitButton;

    // Members needed for Running:
    Player m_Player1;
    Player m_Player2;
    Ball m_Ball;

    // Members needed for Winner:
    uint8_t m_Winner;
    Button m_RetryButton;
    Button m_QuitButton;

    // Init functions
    void Init(const GameConfig &config);
    void InitWindow(const GameConfig &config);
    void InitEntities(bool SinglePlayer);

    void QuitGame();

    // Options functions
    void StartGame();
    void HandleMenuEvents();
    void HandleMenuRendering();

    // Running functions
    void HandleRunningEvents();
    void HandleRunningRendering();
    void HandleRunningMovement(float deltaTime);
    void CheckForWinner();

    // Winner functions
    void HandleWinnerRendering();
    void HandleWinnerEvents();
    void RestartGame();
    void QuitToMenu();

    // Todo: Add buttons for quit and go again
};
