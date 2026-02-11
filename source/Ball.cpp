#include "Ball.h"

#include <cstdlib>

#include "Score.h"
#include "SoundSystem.h"

///
/// Creates a new Ball based on the specified parameters.
/// The ball is created with a position in the middle of the provided viewport.
///
Ball::Ball(float size, sf ::Vector2f viewportSize) {
        this->setSize({size, size});
        this->setOrigin({size / 2.0f, size / 2.0f});
        this->setPosition(viewportSize / 2.0f);
        this->setOutlineColor(sf::Color::White);

        this->m_Speed = {viewportSize.x, 0};
        this->m_ViewportSize = viewportSize;
}

///
/// Moves the ball based on its internal speed, and the provided deltaTime.
/// If the internal state of 'm_Slow' is true, it moves at 30% regular speed.
/// This function also checks for collision with the top and buttom of the screen
/// and if there is a collision, it plays a sound.
///
void Ball::Move(float deltaTime) {
        this->move(this->m_Speed * deltaTime * ((m_SpeedScale * !m_Slow) + (0.3f * m_Slow)));

        if ((this->getPosition().y - this->getSize().y / 2.0f) < 0 && this->m_Speed.y < 0) {
                this->m_Speed.y *= -1;
                SoundSystem::PlaySound("assets/sounds/sqr_beep.wav");
        }
        if ((this->getPosition().y + this->getSize().y / 2.0f) > m_ViewportSize.y && this->m_Speed.y > 0) {
                this->m_Speed.y *= -1;
                SoundSystem::PlaySound("assets/sounds/sqr_beep.wav");
        }
}

///
/// Checks for, and handles the collsion with the provided player.
/// A collision occurs when the ball and the player overlaps, and the ball is
/// moving toward the players x-position.
/// When a collision is detected, the 'm_SpeedScale' is increased,
/// the 'm_Slow' is set to false, and a sound is played.
/// The balls vertical speed is set, based on where it hits the player.
///
void Ball::HandleCollision(const Player& other) {
        if (other.getGlobalBounds().findIntersection(this->getGlobalBounds()) &&
            m_Speed.x * (other.getPosition().x - this->getPosition().x) > 0) {
                this->m_Speed.x *= -1;
                this->m_Speed.y =
                    (this->getPosition().y - other.getPosition().y) / other.getSize().y * std::abs(this->m_Speed.x);

                this->m_Slow = false;
                this->m_SpeedScale += 0.02;

                SoundSystem::PlaySound("assets/sounds/sqr_beep.wav");
        }
}

///
/// Checks if the ball has scored a goal.
/// If a goal is scored, the score for that player in incremented based on the ScoreSystem.
///
void Ball::HandleScore() noexcept {
        if (this->getPosition().x < 0) {
                ScoreSystem::AddScore(2);
                this->Reset();
        }

        if (this->getPosition().x > m_ViewportSize.x) {
                ScoreSystem::AddScore(1);
                this->Reset();
        }
}

///
/// Resets the ball to the middle of the screen, and the m_SpeedScale to 1.0f.
/// It also applies the internal 'm_Slow' flag to the ball, and halfs the vertical speed of the ball.
/// The function is intended to be called, when a goal is scored.
///
void Ball::Reset() noexcept {
        this->setPosition(m_ViewportSize / 2.0f);
        this->m_Speed.y *= 0.5;
        this->m_Slow = true;
        this->m_SpeedScale = 1.0f;
}
