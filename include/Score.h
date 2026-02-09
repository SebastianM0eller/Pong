#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class ScoreSystem
{
  public:
    static void Initialize()
    {
        m_PlayerOneScore = 0;
        m_PlayerTwoScore = 0;
    }
    static void AddScore(const int playerNumer)
    {
        if (playerNumer == 1)
            m_PlayerOneScore++;
        if (playerNumer == 2)
            m_PlayerTwoScore++;
    }

    static int GetScore(const int playerNumber)
    {
        if (playerNumber == 1)
            return m_PlayerOneScore;
        if (playerNumber == 2)
            return m_PlayerTwoScore;

        std::cerr << "Invalid player number: " << playerNumber << "\n";
        return 0;
    }

    static void DrawScore(sf::RenderWindow &window, sf::Font font)
    {
        sf::Text text(font, std::format("{}           {}", m_PlayerOneScore, m_PlayerTwoScore), 30);
        text.setFillColor(sf::Color::White);

        sf::FloatRect textArea = text.getLocalBounds();
        text.setOrigin({textArea.size.x / 2.0f, textArea.size.y / 2.0f});
        text.setPosition({window.getView().getSize().x / 2.0f, 40});

        window.draw(text);
    }

  private:
    ScoreSystem() = default;
    ~ScoreSystem() = default;

    static inline int m_PlayerOneScore;
    static inline int m_PlayerTwoScore;
};
