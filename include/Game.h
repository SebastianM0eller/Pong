#pragma once
#include "Ball.h"
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
    sf::RenderWindow m_Window;
    sf::Font m_Font;
    Player m_Player1;
    Player m_Player2;
    Ball m_Ball;
    GameState m_State{GameState::Running}; // Todo: Should start in the menu when implemented.
    uint8_t m_Winner;
    bool m_IsRunning{false};

    // Init functions
    void Init(const GameConfig &config);
    void InitWindow(const GameConfig &config);
    void InitEntities();

    // Options functions
    // Todo:

    // Running functions
    void HandleRunningEvents();
    void HandleRunningRendering();
    void HandleRunningMovement(float deltaTime);
    void CheckForWinner();

    // Winner functions
    void HandleWinnerRendering();
    void HandleWinnerEvents();

    // Todo: Add buttons for quit and go again
};
