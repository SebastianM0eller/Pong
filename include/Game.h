#pragma once
#include <SFML/Graphics.hpp>
#include <cstdint>

struct GameConfig
{
    const char *WindowName{"SFML Window"};
    uint16_t width{800};
    uint16_t height{600};
    uint16_t framerate{60};
    bool vsync{false};
};

class Game
{
  public:
    Game();
    Game(const GameConfig &config);
    ~Game();

    void Run();

  private:
    sf::Window m_Window;
    bool m_IsRunning{false};

    void InitWindow(const GameConfig &config);
    void Shutdown();
    void HandleEvents();
};
