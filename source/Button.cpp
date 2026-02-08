#include "Button.h"

Button::Button(const ButtonConfig &config)
{
    m_Text = std::make_shared<sf::Text>(config.font, config.text, config.textSize);
    m_Text->setFillColor(sf::Color::White);

    sf::FloatRect textBounds = m_Text->getLocalBounds();
    m_Text->setOrigin(textBounds.size / 2.0f);
    m_Text->setPosition(config.location);

    m_Box.setSize(textBounds.size);
    m_Box.setOrigin(textBounds.size / 2.0f);
    m_Box.setPosition(config.location);
    m_Box.setOutlineColor(sf::Color::White);
}

void Button::Draw(sf::RenderWindow &window)
{
    window.draw(*m_Text);
    window.draw(m_Box);
}

bool Button::IsClicked(sf::Vector2f mousePos)
{
    return m_Box.getGlobalBounds().contains(mousePos);
}
