# Harry Potter Themed Game 🧙‍♂️

A 2D platformer game developed for NYU CS3113 (Intro to Game Programming) featuring custom collision detection, multiple AI behaviors, and multi-level progression.

![Game Screenshot](assets/screenshot.png)
*Screenshot coming soon - game features tile-based maps, animated sprites, and enemy AI*

## 🎮 About

Players control Harry Potter navigating through three levels of a magical library, collecting books while avoiding enemies with different AI behaviors. The game demonstrates advanced game programming concepts including custom physics, AI state machines, and scene management.
![Uploading Screenshot 2026-01-08 at 3.00.26 AM.png…]()

## ✨ Features

### Core Gameplay
- **3 Complete Levels**: Progressively challenging library-themed maps
- **Lives System**: 3 lives per playthrough, death on enemy collision
- **Collectible System**: Find 5 magical books per level to unlock exit
- **Win Condition**: Collect all items and reach the exit door

### Technical Implementation
- **Custom Collision Detection**: Probe-based tile collision with separate X/Y axis resolution
- **Enemy AI (3 Types)**:
  - **Mr. Flinch (WANDERER)**: Patrols randomly across the map
  - **Cat (FOLLOWER)**: Chases player when within 250-unit detection radius
  - **Flyer AI**: Direction-based movement with wall collision response
- **Animation System**: 4-directional sprite atlas (DOWN, LEFT, RIGHT, UP) with frame timing
- **Camera System**: Smooth following camera with panning interpolation
- **Audio Integration**: Background music, pickup sounds, proximity-based detection alerts
- **Scene Management**: Menu, gameplay levels, win/lose screens with state transitions

### Code Architecture
- **Entity System**: Base entity class with inheritance for Player, Enemy, NPC types
- **Map Rendering**: Tile-based map system with collision data
- **Effects System**: Visual effects and screen transitions
- **Object-Oriented Design**: ~930 lines of C++ across 13 files with clean separation of concerns

## 🛠️ Tech Stack

- **Language**: C++ (C++17)
- **Framework**: Raylib
- **Tools**: Visual Studio Code, Git
- **Concepts**: Custom Physics, AI State Machines, Sprite Animation, Object-Oriented Design

## 🚀 How to Run

### Prerequisites
- C++ compiler (g++, clang++, or MSVC)
- Raylib library ([Installation Guide](https://www.raylib.com/))

### Build and Run (macOS/Linux)
```bash
# Clone the repository
git clone https://github.com/ska7048-cmd/homework-five.git
cd homework-five

# Compile
g++ -std=c++17 *.cpp -o game -lraylib -framework OpenGL -framework Cocoa -framework IOKit

# Run
./game
