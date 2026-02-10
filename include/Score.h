#pragma once
#include <SFML/Graphics.hpp>

class ScoreSystem {
       public:
        ///
        /// Sets the score of the players to 0.
        ///
        static void Initialize() noexcept {
                m_PlayerOneScore = 0;
                m_PlayerTwoScore = 0;
        }

        ///
        /// Increments the score of the specified playerNumber.
        /// The playerNumber must be either '1' or '2'.
        /// If the playerNumber is invalid, an assert is triggered in **Debug** for
        /// **Release** nothing happens.
        ///
        static void AddScore(const int playerNumer) noexcept {
                assert(playerNumer == 1 || playerNumer == 2);

                if (playerNumer == 1) {
                        m_PlayerOneScore++;
                        return;
                }
                if (playerNumer == 2) {
                        m_PlayerTwoScore++;
                        return;
                }
        }

        ///
        /// Returns the score for the specified playerNumber.
        /// The playerNumber must be either '1' or '2'.
        /// If the playerNumber is invalid, an assert is triggered in **Debug** for
        /// **Release** the function returns -1.
        ///
        static int GetScore(const int playerNumber) noexcept {
                assert(playerNumber == 1 || playerNumber == 2);

                if (playerNumber == 1) return m_PlayerOneScore;
                if (playerNumber == 2) return m_PlayerTwoScore;

                return -1;
        }

        ///
        /// Draws the score to the top-center of the provided RenderWindow using the
        /// provided font.
        /// The text is displayed using a FontSize of 30.
        /// The color for the text is white.
        ///
        /// Todo: Make the FontSize a parameter.
        /// Todo: Make the color a parameter.
        ///
        static void DrawScore(sf::RenderWindow& window, const sf::Font& font) {
                sf::Text text(font, std::format("{}     {}", m_PlayerOneScore, m_PlayerTwoScore), 30);

                text.setFillColor(sf::Color::White);
                text.setOrigin(text.getLocalBounds().size / 2.0f);
                text.setPosition({window.getView().getSize().x / 2.0f, 40});

                window.draw(text);
        }

       private:
        ScoreSystem() = default;
        ~ScoreSystem() = default;

        static inline int m_PlayerOneScore{0};
        static inline int m_PlayerTwoScore{0};
};
