#pragma once
#include <SFML/Graphics.hpp>

struct PlayerParams
{
    sf::Vector2f viewportSize;
    float width{20};
    float height{80};
    float clearance{20};
    bool playerOne{true};
};

class Player : public sf::RectangleShape
{
  public:
    Player() = default;
    Player(const PlayerParams &config);
    ~Player() = default;

    void Move(float deltaTime);

  private:
    sf::Keyboard::Key m_UpKey;
    sf::Keyboard::Key m_DownKey;
    sf::Vector2f m_ViewportSize;
    float m_Speed;

    // Todo Fix the collision with the ball, since the location is now in the center, and not top left.
};
