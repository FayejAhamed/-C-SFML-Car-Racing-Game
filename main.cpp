// Refactored version of the full game using OOP principles
// File: main.cpp

#include <SFML/Graphics.hpp>
#include "Car.h"
#include "Enemy.h"
#include "Game.h"

int main() {
    Game game;
    game.run();
    return 0;
}