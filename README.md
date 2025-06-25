# 🏎️ SFML Car Racing Game – Final Project (MAC125)

This is a 2D top-down car racing game that I developed using **C++** and the **SFML** graphics library. The game showcases core C++ concepts, including **object-oriented programming, encapsulation**, and **basic polymorphism**. I followed the MAC125 project prompt and was allowed to use AI tools to help with structuring, debugging, and documenting the project.

---

## 🎯 Project Objective

**"Design your own game using object-oriented programming and advanced C++ concepts."**

I chose a car racing theme to demonstrate:
- Class-based design (Car, Enemy, Game)
- Real-time decision making (player controls)
- Visual UI with score, speed, overlays

---

## ✅ Features

- ⏱️ Countdown before race starts
- 🏁 Lane lines, grass, road, curbs – full background visuals
- 🚗 Player car controlled by left/right arrows
- 🚙 Two enemy cars with random lane spawning
- ⚠️ Collision detection with hitbox shrinking
- 🟥 Red overlay, screen shake, and freeze on crash
- 🔁 Press Enter to restart after game over

---

## 🧱 Object-Oriented Design

| File         | Class     | Purpose                             |
|--------------|-----------|-------------------------------------|
| `main.cpp`   | —         | Launches game using Game class      |
| `Car.h/cpp`  | Car       | Handles player control and movement |
| `Enemy.h/cpp`| Enemy     | Enemy behavior, reset, update       |
| `Game.h/cpp` | Game      | Runs logic, rendering, collision    |

### 🔎 OOP Concepts Used

- **Encapsulation**: Data hidden in `private` sections of each class  
- **Polymorphism (basic)**: Shared method names like `draw()`, `update()` across Car and Enemy  
- **Inheritance (optional)**: Not applied directly but code is structured to support it  
- **Separation of concerns**: Each class does one job well

---

## ⚙️ How to Run the Game

1. Install **SFML 2.6.2**
2. Place the following assets in the working directory:
   - `car.png`
   - `enemy_car.png`
   - `enemy_yellow.png`
3. Compile:
```bash
g++ main.cpp Car.cpp Enemy.cpp Game.cpp -o CarGame -lsfml-graphics -lsfml-window -lsfml-system
```
4. Run:
```bash
./CarGame
```

---

## 🤖 How I Used AI

I used **ChatGPT** to:
- Help me split my procedural code into proper `.h/.cpp` class files
- Fix logic bugs and rendering glitches (like disappearing background after crash)
- Add game loop logic, hitbox fairness, freeze-after-crash
- Add helpful code comments and a professional README file

---

## 📋 Evaluation Checklist

| Criteria                        | ✅ |
|----------------------------------|----|
| Creative game design             | ✅ |
| Object-oriented implementation   | ✅ |
| Dynamic input and scoring        | ✅ |
| Organized code & separation      | ✅ |
| Clear documentation & comments   | ✅ |
| Used AI tools appropriately      | ✅ |

---

## 👤 Author

**Fayej**  
Course: MAC125 – C++ Programming  
Date: May 23, 2025  
Instructor approved AI use for this project

