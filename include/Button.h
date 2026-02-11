#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>

struct ButtonConfig {
        std::string text;       // The text displayed on the button
        sf::Font font;          // The font used to display the text
        sf::Vector2f location;  // The location for the center of the button
        uint8_t textSize;       // The fontsized used
};

class Button {
       public:
        Button(const ButtonConfig& config);
        Button() = default;
        ~Button() = default;

        void Draw(sf::RenderWindow& window);
        bool IsClicked(sf::Vector2f mousePos);

       private:
        std::shared_ptr<sf::Text> m_Text{nullptr};
        std::shared_ptr<sf::Font> m_Font{nullptr};  // Stored as a pointer to enable copy
        sf::RectangleShape m_Box;
};
