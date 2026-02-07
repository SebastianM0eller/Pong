#pragma once
#include "Players.h"
#include <SFML/Graphics.hpp>

class Ball : public sf::RectangleShape
{
  public:
    Ball() = default;
    Ball(float size, sf::Vector2f viewportSize);
    ~Ball() = default;

    void Move(float deltaTime);
    void Reset();
    void HandleCollision(const Player &other);
    void HandleScore();

  private:
    sf::Vector2f m_Speed;
    sf::Vector2f m_ViewportSize;
};
