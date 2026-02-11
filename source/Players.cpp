#include "Players.h"

#include <cstdlib>

///
/// Constructs a new Player based on the specified PlayerParams.
///
Player::Player(const PlayerParams& config) {
        float xPos = (config.playerOne) ? (config.size.x) : (config.viewportSize.x - config.size.x);
        float yPos = (config.viewportSize.y / 2.0f);

        this->setSize(config.size);
        this->setOrigin(config.size / 2.0f);
        this->setPosition({xPos, yPos});
        this->setOutlineColor(sf::Color::White);

        this->m_UpKey = (config.playerOne) ? (sf::Keyboard::Key::W) : (sf::Keyboard::Key::I);
        this->m_DownKey = (config.playerOne) ? (sf::Keyboard::Key::S) : (sf::Keyboard::Key::K);
        this->m_Speed = config.viewportSize.y;
        this->m_ViewportSize = config.viewportSize;
        this->m_IsAI = config.AI;
}

///
/// Updates the position of the player based on the given parameters.
/// The ballPos is needed for the potential AI, to determine the direction to move.
///
void Player::Update(float deltaTime, sf::Vector2f ballPos) { (m_IsAI) ? AIMove(deltaTime, ballPos) : Move(deltaTime); }

///
/// Moves the player based on the pressed key, and the deltaTime.
///
void Player::Move(float deltaTime) {
        if (sf::Keyboard::isKeyPressed(m_UpKey)) {
                this->move({0, -m_Speed * deltaTime});
        }
        if (sf::Keyboard::isKeyPressed(m_DownKey)) {
                this->move({0, m_Speed * deltaTime});
        }
}

///
/// Moves the player based on the position of the ball relative to the player.
/// ballPos is the position of the ball as the screen coords.
///
void Player::AIMove(float deltaTime, sf::Vector2f ballPos) {
        sf::Vector2f movementDirection(0, ballPos.y - this->getPosition().y);

        float multiplier = ((std::abs(movementDirection.y) - this->getSize().y / 2.0f) < 0)
                               ? movementDirection.y / this->getSize().y * 2.0f
                               : 1;

        if (movementDirection.lengthSquared() != 0) {
                movementDirection = movementDirection.normalized() * std::abs(multiplier);
                this->move(movementDirection * deltaTime * m_Speed);
        }
}
