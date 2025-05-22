#include <SFML/Graphics.hpp> // Provides classes for handling graphics, windows, and events
#include <vector>              // Enables use of dynamic arrays like std::vector
#include <cstdlib>             // Provides functions like rand() and srand()
#include <ctime>               // Used to seed random number generator with time
#include <sstream>             // For converting numbers to strings
#include <iomanip>             // For formatting text output (like setting decimal precision)

using namespace std; // Allows usage of standard library without prefixing std::
using namespace sf;   // Allows usage of SFML classes without prefixing sf::

// Function to reset the game state to initial values (used on game start or restart)
void resetGame(Sprite& car, Sprite& enemy, Sprite& enemy2, int& score, float& speed, Text& scoreText, Text& speedText) {
    car.setPosition(370.f, 450.f); // Reset player's car to default position
    vector<float> lanePositions = { 170.f, 270.f, 370.f, 470.f }; // Possible x positions for cars (lanes)
    enemy.setPosition(lanePositions[rand() % lanePositions.size()], -150.f); // Reset enemy position randomly above screen
    enemy2.setPosition(lanePositions[rand() % lanePositions.size()], -300.f); // Reset second enemy slightly higher
    score = 0; // Reset score
    speed = 1.0f; // Reset speed
    scoreText.setString("Score: 0"); // Update score text
    speedText.setString("Speed: 20.0 km/h"); // Update speed text
}

int main() {
    RenderWindow window(VideoMode(800, 600), "Car Racing Game"); // Create a game window of 800x600 pixels with title

    // --- Load and scale player's car texture and sprite ---
    Texture carTexture;
    if (!carTexture.loadFromFile("car.png")) return -1; // Load car image; exit if failed
    Sprite car(carTexture); // Create car sprite
    car.setScale(60.f / carTexture.getSize().x, 120.f / carTexture.getSize().y); // Resize car to desired dimensions
    car.setPosition(370.f, 450.f); // Initial car position

    // --- Load and setup enemy car sprites ---
    Texture enemyTexture;
    if (!enemyTexture.loadFromFile("enemy_car.png")) return -1;
    Sprite enemy(enemyTexture);
    enemy.setScale(60.f / enemyTexture.getSize().x, 120.f / enemyTexture.getSize().y);
    enemy.setPosition(150.f, -150.f); // Start above screen

    Texture enemyYellowTexture;
    if (!enemyYellowTexture.loadFromFile("enemy_yellow.png")) return -1;
    Sprite enemy2(enemyYellowTexture);
    enemy2.setScale(60.f / enemyYellowTexture.getSize().x, 120.f / enemyYellowTexture.getSize().y);
    enemy2.setPosition(270.f, -300.f); // Start higher than the first enemy

    // --- Create left and right grass sidebars ---
    RectangleShape leftGrass(Vector2f(150.f, 600.f));
    leftGrass.setFillColor(Color(0, 120, 0)); // Green color
    leftGrass.setPosition(0.f, 0.f);

    RectangleShape rightGrass(Vector2f(150.f, 600.f));
    rightGrass.setFillColor(Color(0, 120, 0)); // Green color
    rightGrass.setPosition(650.f, 0.f);

    // --- Add red and white curbs along road edges ---
    vector<RectangleShape> leftCurbs, rightCurbs;
    Color curbColors[] = { Color::Red, Color::White }; // Alternating curb colors
    int stripeHeight = 20; // Height of each curb stripe
    int curbWidth = 10; // Width of each curb stripe
    for (int i = 0; i < 600 / stripeHeight; ++i) {
        RectangleShape leftStripe(Vector2f(curbWidth, stripeHeight));
        leftStripe.setFillColor(curbColors[i % 2]);
        leftStripe.setPosition(150.f - curbWidth, i * stripeHeight); // Align on left edge
        leftCurbs.push_back(leftStripe);

        RectangleShape rightStripe(Vector2f(curbWidth, stripeHeight));
        rightStripe.setFillColor(curbColors[i % 2]);
        rightStripe.setPosition(650.f, i * stripeHeight); // Align on right edge
        rightCurbs.push_back(rightStripe);
    }

    // --- Create center road area ---
    RectangleShape road(Vector2f(500.f, 600.f));
    road.setFillColor(Color(40, 40, 40)); // Dark gray road color
    road.setPosition(150.f, 0.f);

    // --- Create dashed white lines to separate lanes ---
    vector<RectangleShape> laneLines;
    vector<float> laneX = { 275.f, 375.f, 475.f }; // X positions of lane dividers
    for (float x : laneX) {
        for (int i = 0; i < 5; i++) {
            RectangleShape line(Vector2f(10.f, 80.f)); // Line dimensions
            line.setFillColor(Color::White); // Lane line color
            line.setPosition(x, i * 150.f); // Stack lines vertically
            laneLines.push_back(line);
        }
    }

    // --- Load font and setup on-screen texts ---
    Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) return -1; // Load system font

    Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(Color(255, 215, 0)); // Yellow-ish score text
    scoreText.setPosition(160.f, 10.f);

    Text speedText;
    speedText.setFont(font);
    speedText.setCharacterSize(24);
    speedText.setFillColor(Color(0, 191, 255)); // Blue speed text
    speedText.setPosition(160.f, 40.f);

    Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(32);
    gameOverText.setFillColor(Color::Yellow);
    gameOverText.setPosition(150.f, 250.f);
    gameOverText.setString("Game Over! Press Enter to Restart");

    // --- Initialize game state variables ---
    srand(static_cast<unsigned>(time(nullptr))); // Seed for randomness
    int score = 0;           // Tracks player's score
    float speed = 1.0f;      // Current enemy movement speed
    const float maxGameSpeed = 8.0f; // Cap the max speed
    bool gameOver = false;   // Game over state flag
    bool gameStarted = false; // Whether game has started after countdown
    Clock countdownClock;    // Timer for countdown animation
    int countdown = 3;       // Countdown timer before game starts
    Vector2f shakeOffset(0.f, 0.f); // Shake offset for screen effects
    int shakeFrames = 0;     // Frames left to apply screen shake
    float carSpeedX = 0.f;   // Lateral car movement
    int fadeAlpha = 0;       // Transparency of red crash overlay
    const float maxSpeed = 5.f; // Maximum side speed of car
    const float acceleration = 0.5f; // How quickly car accelerates left/right
    const float deceleration = 0.3f; // How quickly car stops moving side to side
    Clock speedClock;        // Clock to control automatic speed increase

    // Main game loop begins below...
// Main game loop begins below...
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close(); // Close the game window if the user clicks the X button
        }

        // Countdown before the game starts
        static int previousCountdown = countdown;
        if (!gameStarted) {
            if (countdownClock.getElapsedTime().asSeconds() >= 1.0f) {
                if (countdown > 0) {
                    countdown--; // Decrease countdown every second
                    countdownClock.restart(); // Restart clock after each countdown tick
                    previousCountdown = countdown;
                }
                else {
                    gameStarted = true; // Start the game after countdown ends
                }
            }
        }

        // Game logic runs only if game has started and not over
        if (!gameOver && gameStarted) {
            // Gradually increase the game speed every 7 seconds
            if (speedClock.getElapsedTime().asSeconds() > 7.f && speed < maxGameSpeed) {
                speed += 0.1f;
                speedClock.restart();
            }

            // Handle keyboard input for moving car left or right
            bool movingLeft = false, movingRight = false;
            if (Keyboard::isKeyPressed(Keyboard::Left)) movingLeft = true;
            if (Keyboard::isKeyPressed(Keyboard::Right)) movingRight = true;

            // Optional: handle touch input for mobile devices
            for (int i = 0; i < 10; ++i) {
                if (Touch::isDown(i)) {
                    Vector2f touchPos = window.mapPixelToCoords(Touch::getPosition(i, window));
                    if (touchPos.x < 400)
                        movingLeft = true;
                    else
                        movingRight = true;
                }
            }

            // Update car speed left/right based on input
            if (movingLeft) {
                carSpeedX -= acceleration;
                if (carSpeedX < -maxSpeed) carSpeedX = -maxSpeed;
            }
            else if (movingRight) {
                carSpeedX += acceleration;
                if (carSpeedX > maxSpeed) carSpeedX = maxSpeed;
            }
            else {
                // If no input, gradually slow the car down
                if (carSpeedX > 0) {
                    carSpeedX -= deceleration;
                    if (carSpeedX < 0) carSpeedX = 0;
                }
                else if (carSpeedX < 0) {
                    carSpeedX += deceleration;
                    if (carSpeedX > 0) carSpeedX = 0;
                }
            }

            car.move(carSpeedX, 0.f); // Move the car based on calculated speed

            // Keep car within the road boundaries (150 - 650)
            if (car.getPosition().x < 150.f)
                car.setPosition(150.f, car.getPosition().y);
            if (car.getPosition().x + car.getGlobalBounds().width > 650.f)
                car.setPosition(650.f - car.getGlobalBounds().width, car.getPosition().y);

            // Move the white dashed lane lines downwards to simulate road movement
            for (auto& line : laneLines) {
                line.move(0.f, speed);
                if (line.getPosition().y > 600)
                    line.setPosition(line.getPosition().x, -80.f); // Reset line to top
            }

            // Move enemies downward and reset when they go off screen
            vector<float> lanePositions = { 170.f, 270.f, 370.f, 470.f };
            enemy.move(0.f, speed);
            if (enemy.getPosition().y > 600) {
                enemy.setPosition(lanePositions[rand() % lanePositions.size()], -150.f);
                score++; // Increase score each time enemy resets
                if (score % 10 == 0 && speed < maxGameSpeed) speed += 0.2f; // Increase speed every 10 points
            }

            enemy2.move(0.f, speed);
            if (enemy2.getPosition().y > 600) {
                enemy2.setPosition(lanePositions[rand() % lanePositions.size()], -150.f);
                score++;
                if (score % 10 == 0 && speed < maxGameSpeed) speed += 0.2f;
            }

            // Define hitboxes (slightly smaller for fairness)
            FloatRect carHitbox(car.getPosition().x + 12.f, car.getPosition().y + 25.f,
                car.getGlobalBounds().width - 24.f, car.getGlobalBounds().height - 40.f);
            FloatRect enemyHitbox(enemy.getPosition().x + 12.f, enemy.getPosition().y + 25.f,
                enemy.getGlobalBounds().width - 24.f, enemy.getGlobalBounds().height - 40.f);
            FloatRect enemyHitbox2(enemy2.getPosition().x + 12.f, enemy2.getPosition().y + 25.f,
                enemy2.getGlobalBounds().width - 24.f, enemy2.getGlobalBounds().height - 40.f);

            // Check collision: if player's car touches any enemy
            if (carHitbox.intersects(enemyHitbox) || carHitbox.intersects(enemyHitbox2)) {
                gameOver = true; // End the game
                shakeFrames = 15; // Trigger screen shake effect
                fadeAlpha = 150;  // Start red overlay fade-in effect
            }
            else {
                // Update score display
                stringstream ss;
                ss << "Score: " << score;
                scoreText.setString(ss.str());

                // Update speed display (speed x 20 to show in km/h)
                stringstream sp;
                sp << fixed << setprecision(1) << "Speed: " << speed * 20 << " km/h";
                speedText.setString(sp.str());
            }
        }
        else {
            // Game is over, wait for Enter key to restart
            if (Keyboard::isKeyPressed(Keyboard::Enter)) {
                resetGame(car, enemy, enemy2, score, speed, scoreText, speedText); // Reset game state
                gameOver = false; // Clear game over flag
                speedClock.restart(); // Restart the speed timer
            }
        }
        // (continued from game logic)
                // Screen shake logic (shakeOffset changes every frame for dramatic effect)
                if (shakeFrames > 0) {
                    float xOffset = static_cast<float>((rand() % 7) - 3); // Horizontal shake range: -3 to +3
                    float yOffset = static_cast<float>((rand() % 7) - 3); // Vertical shake range: -3 to +3
                    shakeOffset = Vector2f(xOffset, yOffset);
                    shakeFrames--; // Countdown shake duration
                }
                else {
                    shakeOffset = Vector2f(0.f, 0.f); // Stop shaking
                }

                // --- Drawing Section ---
                window.clear(); // Clear the previous frame

                window.draw(leftGrass);  // Draw left grass
                window.draw(rightGrass); // Draw right grass

                for (auto& stripe : leftCurbs) window.draw(stripe);  // Draw left curb stripes
                for (auto& stripe : rightCurbs) window.draw(stripe); // Draw right curb stripes

                window.draw(road); // Draw the main road

                for (auto& line : laneLines) window.draw(line); // Draw lane dividers

                car.move(shakeOffset);       // Apply shake to car before drawing
                window.draw(car);            // Draw player's car
                car.move(-shakeOffset);      // Undo shake after drawing

                enemy.move(shakeOffset);     // Apply shake to enemy
                window.draw(enemy);          // Draw enemy car
                enemy.move(-shakeOffset);    // Undo shake

                enemy2.move(shakeOffset);    // Apply shake to second enemy
                window.draw(enemy2);         // Draw second enemy car
                enemy2.move(-shakeOffset);   // Undo shake

                window.draw(scoreText);      // Display current score
                window.draw(speedText);      // Display current speed

                // Show game over message if game ended
                if (gameOver) {
                    gameOverText.move(shakeOffset);  // Apply shake
                    window.draw(gameOverText);
                    gameOverText.move(-shakeOffset); // Reset position
                }
                // Show countdown before game starts
                else if (!gameStarted) {
                    Text countdownText;
                    countdownText.setFont(font);
                    countdownText.setCharacterSize(60);
                    countdownText.setFillColor(Color::Red);
                    countdownText.setPosition(330.f, 250.f);

                    if (countdown > 0)
                        countdownText.setString(to_string(countdown)); // Show countdown number
                    else {
                        countdownText.setString("START"); // Show START message
                        countdownText.setFillColor(Color::Green);
                    }

                    // Scale text for pulse animation
                    countdownText.setScale(
                        1.0f + 0.2f * sin(countdownClock.getElapsedTime().asSeconds() * 6),
                        1.0f + 0.2f * sin(countdownClock.getElapsedTime().asSeconds() * 6)
                    );
                    window.draw(countdownText); // Draw countdown or START
                }

                // Red fade overlay after crash
                if (gameOver && fadeAlpha > 0) {
                    RectangleShape redOverlay(Vector2f(500.f, 600.f));
                    redOverlay.setPosition(150.f, 0.f);
                    redOverlay.setFillColor(Color(255, 0, 0, fadeAlpha)); // Transparent red layer
                    window.draw(redOverlay);
                    fadeAlpha -= 2; // Fade effect decreases over time
                    if (fadeAlpha < 0) fadeAlpha = 0;
                }

                // White blur overlay during shake effect for visual impact
                if (shakeFrames > 0) {
                    for (int i = 0; i < 3; ++i) {
                        RectangleShape blurLayer(Vector2f(500.f, 600.f));
                        blurLayer.setPosition(150.f, 0.f);
                        blurLayer.setFillColor(Color(255, 255, 255, 20)); // Light transparent white overlay
                        window.draw(blurLayer);
                    }
                }

                window.display(); // Display everything on the screen
    } // End of game loop

    return 0; // Exit the program
}
