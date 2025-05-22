#ifndef CAR_H
#define CAR_H
#include <SFML/Graphics.hpp>
class Car {
private:
    sf::Sprite sprite;
    float speedX;
    const float maxSpeed = 5.f;
    const float acceleration = 0.5f;
    const float deceleration = 0.3f;
public:
    Car(const sf::Texture& texture);
    void reset();
    void update(bool left, bool right);
    void draw(sf::RenderWindow& window, sf::Vector2f shakeOffset);
    sf::Sprite& getSprite();
};
#endif
