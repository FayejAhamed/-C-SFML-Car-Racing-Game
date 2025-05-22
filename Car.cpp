#include "Car.h"
Car::Car(const sf::Texture& texture) : speedX(0.f) {
    sprite.setTexture(texture);
    sprite.setScale(60.f / texture.getSize().x, 120.f / texture.getSize().y);
    reset();
}
void Car::reset() {
    sprite.setPosition(370.f, 450.f);
    speedX = 0.f;
}
void Car::update(bool left, bool right) {
    if (left) {
        speedX -= acceleration;
        if (speedX < -maxSpeed) speedX = -maxSpeed;
    }
    else if (right) {
        speedX += acceleration;
        if (speedX > maxSpeed) speedX = maxSpeed;
    }
    else {
        if (speedX > 0) speedX -= deceleration;
        else if (speedX < 0) speedX += deceleration;
        if (abs(speedX) < 0.1f) speedX = 0;
    }
    sprite.move(speedX, 0);
    if (sprite.getPosition().x < 150)
        sprite.setPosition(150, sprite.getPosition().y);
    else if (sprite.getPosition().x + sprite.getGlobalBounds().width > 650)
        sprite.setPosition(650 - sprite.getGlobalBounds().width, sprite.getPosition().y);
}
void Car::draw(sf::RenderWindow& window, sf::Vector2f shakeOffset) {
    sprite.move(shakeOffset);
    window.draw(sprite);
    sprite.move(-shakeOffset);
}
sf::Sprite& Car::getSprite() { return sprite; }
