// Step 6: Game.h
#include <vector>
#ifndef GAME_H
#define GAME_H
#include <SFML/Graphics.hpp>
#include "Car.h"
#include "Enemy.h"
class Game {
private:
    // Background graphics
   // Background visuals
    sf::RectangleShape road, leftGrass, rightGrass;
    std::vector<sf::RectangleShape> laneLines, leftCurbs, rightCurbs;


private:
    sf::RenderWindow window;
    sf::Font font;
    sf::Texture carTex, enemyTex1, enemyTex2;
    Car* car;
    Enemy* enemy1;
    Enemy* enemy2;
    sf::Text scoreText, speedText, gameOverText;
    int score;
    float speed;
    bool gameOver, gameStarted;
    sf::Clock countdownClock, speedClock;
    int countdown;
    sf::Vector2f shakeOffset;
    int shakeFrames, fadeAlpha;
    void processEvents();
    void update();
    void render();
    void reset();
public:
    Game();
    ~Game();
    void run();
};
#endif