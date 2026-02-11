#pragma once
#include <SFML/Graphics.hpp>

struct PlayerParams {
        sf::Vector2f viewportSize;
        sf::Vector2f size{20, 80};
        bool playerOne{true};
        bool AI{false};
};

class Player : public sf::RectangleShape {
       public:
        Player() = default;
        Player(const PlayerParams& config);
        ~Player() = default;

        void Update(float deltaTime, sf::Vector2f ballPos);

       private:
        sf::Keyboard::Key m_UpKey;
        sf::Keyboard::Key m_DownKey;
        sf::Vector2f m_ViewportSize;
        float m_Speed;
        bool m_IsAI;

        void Move(float deltaTime);
        void AIMove(float deltaTime, sf::Vector2f ballPos);
};
