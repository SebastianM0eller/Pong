#pragma once
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/System/Vector2.hpp"
#include <SFML/Graphics.hpp>
#include <string>

struct ButtonConfig
{
    std::string text;      // The text displayed on the button
    sf::Font font;         // The font used to display the text
    sf::Vector2f location; // The location for the center of the button
    uint8_t textSize;      // The fontsized used
};

class Button
{
  public:
    Button(const ButtonConfig &config);
    Button() = default;
    ~Button() = default;

    void Draw(sf::RenderWindow &window);
    bool IsClicked(sf::Vector2f mousePos);

  private:
    std::shared_ptr<sf::Text> m_Text{nullptr};
    sf::RectangleShape m_Box;
};
