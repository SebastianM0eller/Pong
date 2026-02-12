#include "Button.h"

#include <memory>

#include "SFML/System/Vector2.hpp"

///
/// Creates a new button, based on the specified ButtonConfig.
///
Button::Button(const ButtonConfig& config) {
        m_Font = std::make_unique<sf::Font>(config.font);

        m_Text = std::make_unique<sf::Text>(*m_Font.get(), config.text, config.textSize);
        m_Text->setFillColor(sf::Color::White);

        sf::FloatRect ButtonSize = m_Text->getLocalBounds();
        m_Text->setOrigin(ButtonSize.size / 2.0f);
        m_Text->setPosition(config.location);

        sf::Vector2f boxBuffer(2, 2);
        m_Box.setSize(ButtonSize.size + boxBuffer);
        m_Box.setOrigin((ButtonSize.size + boxBuffer) / 2.0f);
        m_Box.setPosition(config.location);
        m_Box.setFillColor(sf::Color::Transparent);
        m_Box.setOutlineColor(sf::Color::White);
        m_Box.setOutlineThickness(1.0f);
}

///
/// Draws the button to the specified RenderWindow.
///
void Button::Draw(sf::RenderWindow& window) {
        window.draw(*m_Text);
        window.draw(m_Box);
}

///
/// Returns wether the provided location is inside the button.
///
bool Button::IsClicked(sf::Vector2f mousePos) { return m_Box.getGlobalBounds().contains(mousePos); }
