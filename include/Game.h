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

    bool m_IsRunning{false};

    void InitWindow(const GameConfig &config);
    void InitEntities();
    void Shutdown();
    void HandleEvents();
    void HandleRendering();
    void HandleMovement(float deltaTime);
    void HandleBallPoints();
    void DrawScore();
};
