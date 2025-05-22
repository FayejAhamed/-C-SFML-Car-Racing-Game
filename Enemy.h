#ifndef ENEMY_H
#define ENEMY_H
#include <SFML/Graphics.hpp>
#include <vector>
class Enemy  { 
private:
    sf::Sprite sprite;
public:
    Enemy(const sf::Texture& texture, float offset);
    void reset(const std::vector<float>& lanes);
    void update(float speed);
    void draw(sf::RenderWindow& window, sf::Vector2f shakeOffset);
    sf::Sprite& getSprite();
};
#endif
