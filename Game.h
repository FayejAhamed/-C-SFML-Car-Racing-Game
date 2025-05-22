#ifndef GAME_H
#define GAME_H
#include <SFML/Graphics.hpp>
#include "Car.h"
#include "Enemy.h"
#include <vector>

class Game { // CLASS: Manages the full game state and logic (encapsulation used)
private:
    sf::RenderWindow window;

    // Game objects
    Car* car; // Polymorphism: using base class pointer to draw any drawable entity
    Enemy* enemy1;; // Polymorphism: enemy1 treated as a generic drawable
    Enemy* enemy2;; // Polymorphism: enemy2 treated as a generic drawable

    // Background elements
    sf::RectangleShape road, leftGrass, rightGrass;
    std::vector<sf::RectangleShape> laneLines, leftCurbs, rightCurbs;

    // Text and fonts
    sf::Font font;
    sf::Text scoreText, speedText, gameOverText;

    // Assets
    sf::Texture carTex, enemyTex1, enemyTex2;

    // Game state
    int score;
    float speed;
    bool gameOver;
    bool gameStarted;

    sf::Clock countdownClock, speedClock;
    int countdown;

    sf::Vector2f shakeOffset;
    int shakeFrames;
    int fadeAlpha;

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