#include "Ball.h"
#include "Score.h"
#include "SoundSystem.h"
#include <cstdlib>

Ball::Ball(float size, sf ::Vector2f viewportSize)
{
    this->setSize({size, size});
    this->setOrigin(this->getSize() / 2.0f);
    this->setPosition(viewportSize / 2.0f);
    this->setOutlineColor(sf::Color::White);
    this->setOutlineThickness(0.5f);

    this->m_Speed = {viewportSize.x / 1.0f, 0};
    this->m_ViewportSize = viewportSize;
}

void Ball::Move(float deltaTime)
{
    (m_Slow) ? this->move(this->m_Speed * deltaTime * 0.3f) : this->move(this->m_Speed * deltaTime * m_SpeedScale);

    if ((this->getPosition().y - this->getSize().y / 2.0f) < 0 && this->m_Speed.y < 0)
    {
        this->m_Speed.y *= -1;
        SoundSystem::PlaySound("assets/sounds/sqr_beep.wav");
    }
    if ((this->getPosition().y + this->getSize().y / 2.0f) > m_ViewportSize.y && this->m_Speed.y > 0)
    {
        this->m_Speed.y *= -1;
        SoundSystem::PlaySound("assets/sounds/sqr_beep.wav");
    }
}

void Ball::Reset()
{
    this->setPosition(m_ViewportSize / 2.0f);
    this->m_Speed.y *= 0.5;
    this->m_Slow = true;
    this->m_SpeedScale = 1.0f;
}

void Ball::HandleCollision(const Player &other)
{
    if (other.getGlobalBounds().findIntersection(this->getGlobalBounds()) &&
        m_Speed.x * (other.getPosition().x - this->getPosition().x) > 0)
    {
        this->m_Speed.x *= -1;
        this->m_Speed.y =
            (this->getPosition().y - other.getPosition().y) / other.getSize().y * std::abs(this->m_Speed.x);

        SoundSystem::PlaySound("assets/sounds/sqr_beep.wav");
        this->m_Slow = false;
        this->m_SpeedScale += 0.02;
    }
}

void Ball::HandleScore()
{
    if (this->getPosition().x < 0)
    {
        ScoreSystem::AddScore(2);
        this->Reset();
    }

    if (this->getPosition().x > m_ViewportSize.x)
    {
        ScoreSystem::AddScore(1);
        this->Reset();
    }
}
