#pragma once
#include "Players.h"

class Ball : public sf::RectangleShape {
       public:
        Ball(float size, sf::Vector2f viewportSize);
        Ball() = default;
        ~Ball() = default;

        void Move(float deltaTime);
        void HandleCollision(const Player& other);
        void HandleScore() noexcept;
        void Reset() noexcept;

       private:
        sf::Vector2f m_Speed;
        sf::Vector2f m_ViewportSize;
        float m_SpeedScale{1};
        bool m_Slow{false};
};
