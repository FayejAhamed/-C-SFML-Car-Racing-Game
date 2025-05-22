#include "Enemy.h"
#include <cstdlib>

Enemy::Enemy(const sf::Texture& texture, float offset) {
    sprite.setTexture(texture);
    sprite.setScale(60.f / texture.getSize().x, 120.f / texture.getSize().y);
    sprite.setPosition(270.f, offset);
}

void Enemy::reset(const std::vector<float>& lanes) {
    sprite.setPosition(lanes[rand() % lanes.size()], -150.f);
}

void Enemy::update(float speed) {
    sprite.move(0.f, speed);
    if (sprite.getPosition().y > 600)
        reset({ 170.f, 270.f, 370.f, 470.f });
}

void Enemy::draw(sf::RenderWindow& window, sf::Vector2f shakeOffset) {
    sprite.move(shakeOffset);
    window.draw(sprite);
    sprite.move(-shakeOffset);
}

sf::Sprite& Enemy::getSprite() {
    return sprite;
}