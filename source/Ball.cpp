#include "Ball.h"
#include <cstdlib>

Ball::Ball(float size, sf ::Vector2f viewportSize)
{
    this->setSize({size, size});
    this->setOrigin(this->getSize() / 2.0f);
    this->setPosition(viewportSize / 2.0f);
    this->setOutlineColor(sf::Color::White);
    this->setOutlineThickness(0.5f);

    this->m_Speed = {viewportSize.x / 2.0f, 0};
    this->m_ViewportSize = viewportSize;
}

void Ball::Move(float deltaTime)
{
    this->move(this->m_Speed * deltaTime);

    if (this->getPosition().y < 0 && this->m_Speed.y < 0)
    {
        this->m_Speed.y *= -1;
    }
    if (this->getPosition().y > m_ViewportSize.y && this->m_Speed.y > 0)
    {
        this->m_Speed.y *= -1;
    }
}

void Ball::Reset()
{
    this->setPosition(m_ViewportSize / 2.0f);
    this->m_Speed.x *= -1;
    this->m_Speed.y *= 0.5;
}

void Ball::HandleCollision(const Player &other)
{
    if (other.getGlobalBounds().findIntersection(this->getGlobalBounds()) &&
        m_Speed.x * (other.getPosition().x - this->getPosition().x) > 0)
    {
        this->m_Speed.x *= -1;
        this->m_Speed.y =
            (this->getPosition().y - other.getPosition().y) / other.getSize().y * std::abs(this->m_Speed.x);
    }
}
