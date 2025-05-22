#include "Game.h"
#include <sstream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <cstdlib>

Game::Game() : window(sf::VideoMode(800, 600), "Car Racing Game"), score(0), speed(1.0f), gameOver(false), gameStarted(false), countdown(3), shakeFrames(0), fadeAlpha(0) {
    srand(static_cast<unsigned>(time(nullptr)));

    font.loadFromFile("C:/Windows/Fonts/arial.ttf");
    carTex.loadFromFile("car.png");
    enemyTex1.loadFromFile("enemy_car.png");
    enemyTex2.loadFromFile("enemy_yellow.png");

    car = new Car(carTex);
    enemy1 = new Enemy(enemyTex1, -150.f);
    enemy2 = new Enemy(enemyTex2, -300.f);

    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color(255, 215, 0));
    scoreText.setPosition(160.f, 10.f);

    speedText.setFont(font);
    speedText.setCharacterSize(24);
    speedText.setFillColor(sf::Color(0, 191, 255));
    speedText.setPosition(160.f, 40.f);

    gameOverText.setFont(font);
    gameOverText.setCharacterSize(32);
    gameOverText.setFillColor(sf::Color::Yellow);
    gameOverText.setPosition(150.f, 250.f);
    gameOverText.setString("Game Over! Press Enter to Restart");

    // Set up road and background
    road.setSize(sf::Vector2f(500.f, 600.f));
    road.setFillColor(sf::Color(40, 40, 40));
    road.setPosition(150.f, 0.f);

    leftGrass.setSize(sf::Vector2f(150.f, 600.f));
    leftGrass.setFillColor(sf::Color(0, 120, 0));
    leftGrass.setPosition(0.f, 0.f);

    rightGrass.setSize(sf::Vector2f(150.f, 600.f));
    rightGrass.setFillColor(sf::Color(0, 120, 0));
    rightGrass.setPosition(650.f, 0.f);

    // Dashed white lane lines
    float laneXs[] = { 275.f, 375.f, 475.f };
    for (float x : laneXs) {
        for (int i = 0; i < 5; ++i) {
            sf::RectangleShape line(sf::Vector2f(10.f, 80.f));
            line.setFillColor(sf::Color::White);
            line.setPosition(x, i * 150.f);
            laneLines.push_back(line);
        }
    }

    // Red and white curbs
    sf::Color curbColors[] = { sf::Color::Red, sf::Color::White };
    int stripeHeight = 20;
    int curbWidth = 10;
    for (int i = 0; i < 600 / stripeHeight; ++i) {
        sf::RectangleShape left(sf::Vector2f(curbWidth, stripeHeight));
        left.setFillColor(curbColors[i % 2]);
        left.setPosition(150.f - curbWidth, i * stripeHeight);
        leftCurbs.push_back(left);

        sf::RectangleShape right(sf::Vector2f(curbWidth, stripeHeight));
        right.setFillColor(curbColors[i % 2]);
        right.setPosition(650.f, i * stripeHeight);
        rightCurbs.push_back(right);
    }

    reset();
}

Game::~Game() {
    delete car;
    delete enemy1;
    delete enemy2;
}

void Game::run() {
    while (window.isOpen()) {
        processEvents();
        update();
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
    }
    if (gameOver && sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
        reset();
        gameOver = false;
        speedClock.restart();
    }
}

void Game::update() {
    if (!gameStarted) {
        if (countdownClock.getElapsedTime().asSeconds() >= 1.0f) {
            if (countdown > 0) {
                countdown--;
                countdownClock.restart();
            }
            else {
                gameStarted = true;
            }
        }
        return;
    }
    if (gameOver) return;

    if (speedClock.getElapsedTime().asSeconds() > 7.f && speed < 8.0f) {
        speed += 0.1f;
        speedClock.restart();
    }

    bool left = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
    bool right = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
    car->update(left, right);

    enemy1->update(speed);
    enemy2->update(speed);

    sf::FloatRect carBox = car->getSprite().getGlobalBounds();
    sf::FloatRect e1Box = enemy1->getSprite().getGlobalBounds();
    sf::FloatRect e2Box = enemy2->getSprite().getGlobalBounds();

    // Shrink hitboxes for fairer collisions
    carBox = sf::FloatRect(carBox.left + 12.f, carBox.top + 25.f, carBox.width - 24.f, carBox.height - 40.f);
    e1Box = sf::FloatRect(e1Box.left + 12.f, e1Box.top + 25.f, e1Box.width - 24.f, e1Box.height - 40.f);
    e2Box = sf::FloatRect(e2Box.left + 12.f, e2Box.top + 25.f, e2Box.width - 24.f, e2Box.height - 40.f);

    if (carBox.intersects(e1Box) || carBox.intersects(e2Box)) {
        gameOver = true;
        shakeFrames = 15;
        fadeAlpha = 150;
        return; // stop game state here
    }
    else {
        score++;
        std::stringstream ss;
        ss << "Score: " << score / 10;
        scoreText.setString(ss.str());

        std::stringstream sp;
        sp << std::fixed << std::setprecision(1) << "Speed: " << speed * 20 << " km/h";
        speedText.setString(sp.str());
    }

    if (shakeFrames > 0) {
        shakeOffset.x = static_cast<float>((rand() % 7) - 3);
        shakeOffset.y = static_cast<float>((rand() % 7) - 3);
        shakeFrames--;
    }
    else {
        shakeOffset = sf::Vector2f(0.f, 0.f);
    }
}

void Game::render() {
    window.clear();
    window.draw(leftGrass);
    window.draw(rightGrass);
    for (auto& stripe : leftCurbs) window.draw(stripe);
    for (auto& stripe : rightCurbs) window.draw(stripe);
    window.draw(road);
    for (auto& line : laneLines) {
        line.move(0.f, speed);
        if (line.getPosition().y > 600)
            line.setPosition(line.getPosition().x, -80.f);
        window.draw(line);
    }
    // window.clear(); ❌ REMOVE this second clear that erases background
    car->draw(window, shakeOffset);
    enemy1->draw(window, shakeOffset);
    enemy2->draw(window, shakeOffset);
    window.draw(scoreText);
    window.draw(speedText);

    if (gameOver) {
        gameOverText.move(shakeOffset);
        window.draw(gameOverText);
        gameOverText.move(-shakeOffset);
        if (fadeAlpha > 0) {
            sf::RectangleShape redOverlay(sf::Vector2f(500.f, 600.f));
            redOverlay.setPosition(150.f, 0.f);
            redOverlay.setFillColor(sf::Color(255, 0, 0, fadeAlpha));
            window.draw(redOverlay);
            fadeAlpha -= 2;
            if (fadeAlpha < 0) fadeAlpha = 0;
        }
    }
    else if (!gameStarted) {
        sf::Text countdownText;
        countdownText.setFont(font);
        countdownText.setCharacterSize(60);
        countdownText.setPosition(330.f, 250.f);
        countdownText.setFillColor(sf::Color::Red);
        countdownText.setString(countdown > 0 ? std::to_string(countdown) : "START");
        if (countdown == 0) countdownText.setFillColor(sf::Color::Green);
        float scale = 1.0f + 0.2f * std::sin(countdownClock.getElapsedTime().asSeconds() * 6);
        countdownText.setScale(scale, scale);
        window.draw(countdownText);
    }

    if (shakeFrames > 0) {
        for (int i = 0; i < 3; ++i) {
            sf::RectangleShape blurLayer(sf::Vector2f(500.f, 600.f));
            blurLayer.setPosition(150.f, 0.f);
            blurLayer.setFillColor(sf::Color(255, 255, 255, 20));
            window.draw(blurLayer);
        }
    }
    window.display();
}

void Game::reset() {
    car->reset();
    enemy1->reset({ 170.f, 270.f, 370.f, 470.f });
    enemy2->reset({ 170.f, 270.f, 370.f, 470.f });
    score = 0;
    speed = 1.0f;
    countdown = 3;
    countdownClock.restart();
    gameStarted = false;
    scoreText.setString("Score: 0");
    speedText.setString("Speed: 20.0 km/h");
}
