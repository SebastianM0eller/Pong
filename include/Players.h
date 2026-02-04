#include <SFML/Graphics.hpp>

struct PlayerParams
{
    float width{20};
    float height{80};
    float clearance{20};
    bool playerOne{true};
    sf::Vector2u viewportSize;
};

class Player : public sf::RectangleShape
{
  public:
    Player(const PlayerParams &config);
    ~Player() = default;

    void Move(float deltaTime);

  private:
    sf::Keyboard::Key m_UpKey;
    sf::Keyboard::Key m_DownKey;
    sf::Vector2u m_ViewportSize;
    float m_Speed;
};
