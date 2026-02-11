#pragma once
#include <SFML/Graphics.hpp>

struct PlayerParams {
        sf::Vector2f viewportSize;  // The size of the windows viewport.
        sf::Vector2f size{20, 80};  // The size of the player.
        bool playerOne{true};       // True of playerOne, and false if playerTwo.
        bool AI{false};             // True of controlled by the simple AI.
};

class Player : public sf::RectangleShape {
       public:
        Player(const PlayerParams& config);
        Player() = default;
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
