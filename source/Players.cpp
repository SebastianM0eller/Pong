#include "Players.h"
#include "SFML/Window/Keyboard.hpp"

Player::Player(const PlayerParams &config)
{
    float xPos = (config.playerOne) ? (config.clearance) : (config.viewportSize.x - config.clearance);
    float yPos = (config.viewportSize.y / 2.0f);

    this->setPosition({xPos, yPos});
    this->setOutlineColor(sf::Color::White);
    this->setOutlineThickness(0.5f);
    this->setSize({config.width, config.height});
    this->setOrigin(this->getSize() / 2.0f);

    this->m_UpKey = (config.playerOne) ? (sf::Keyboard::Key::W) : (sf::Keyboard::Key::I);
    this->m_DownKey = (config.playerOne) ? (sf::Keyboard::Key::D) : (sf::Keyboard::Key::K);
    this->m_Speed = config.viewportSize.y / 3.0f;
    this->m_ViewportSize = config.viewportSize;
}

void Player::Move(float deltaTime)
{
    if (sf::Keyboard::isKeyPressed(m_UpKey))
    {
        this->move({0, m_Speed * m_ViewportSize.y * deltaTime});
    }
    if (sf::Keyboard::isKeyPressed(m_DownKey))
    {
        this->move({0, -m_Speed * m_ViewportSize.y * deltaTime});
    }
}
