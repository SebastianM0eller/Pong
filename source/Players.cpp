#include "Players.h"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Keyboard.hpp"
#include <complex>

Player::Player(const PlayerParams &config)
{
    float xPos = (config.playerOne) ? (config.clearance) : (config.viewportSize.x - config.clearance);
    float yPos = (config.viewportSize.y / 2.0f);

    this->setSize({config.width, config.height});
    this->setOrigin(this->getSize() / 2.0f);
    this->setPosition({xPos, yPos});
    this->setOutlineColor(sf::Color::White);
    this->setOutlineThickness(0.5f);

    this->m_UpKey = (config.playerOne) ? (sf::Keyboard::Key::W) : (sf::Keyboard::Key::I);
    this->m_DownKey = (config.playerOne) ? (sf::Keyboard::Key::S) : (sf::Keyboard::Key::K);
    this->m_Speed = config.viewportSize.y / 1.0f;
    this->m_ViewportSize = config.viewportSize;
    this->m_IsAI = config.AI;
}

void Player::Update(float deltaTime, sf::Vector2f ballPos)
{
    (m_IsAI) ? AIMove(deltaTime, ballPos) : Move(deltaTime);
}

void Player::Move(float deltaTime)
{
    if (sf::Keyboard::isKeyPressed(m_UpKey))
    {
        this->move({0, -m_Speed * deltaTime});
    }
    if (sf::Keyboard::isKeyPressed(m_DownKey))
    {
        this->move({0, m_Speed * deltaTime});
    }
}

void Player::AIMove(float deltaTime, sf::Vector2f ballPos)
{
    sf::Vector2f dir(0, ballPos.y - this->getPosition().y);

    float multiplier = ((std::norm(dir.y) - this->getSize().y / 2.0f) < 0) ? dir.y / this->getSize().y * 2.0f : 1;

    if (dir.lengthSquared() != 0)
    {
        dir = dir.normalized() * std::norm(multiplier);
        this->move(dir * deltaTime * m_Speed);
    }
}
