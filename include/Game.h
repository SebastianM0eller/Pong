#pragma once
#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/System/Vector2.hpp"
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
    sf::RenderWindow m_Window;
    sf::RectangleShape m_Player1;
    sf::RectangleShape m_Player2;
    sf::CircleShape m_Ball;

    bool m_IsRunning{false};

    void InitWindow(const GameConfig &config);
    void InitEntities();
    void Shutdown();
    void HandleEvents();
    void HandleRendering();

    sf::Vector2f NormalDeviceToRegular(sf::Vector2f windowSize, sf::Vector2f entitySize, sf::Vector2f location);
};
