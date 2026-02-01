#pragma once

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

  private:
    ScoreSystem() = default;
    ~ScoreSystem() = default;

    static int m_PlayerOneScore;
    static int m_PlayerTwoScore;
};
