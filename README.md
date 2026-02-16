# Pong

> A simple Pong game made with C++ using SFML. The project uses a CMake for the buildsystem.

## About
The aim of this project is to learn and have fun with C++
* **Standard:** C++23
* **Build System:** CMake 3.30+

### Features
    * Simple Pong gameplay.
    * A simple AI for singleplayer.
    * Collision sounds.

## How to build
* Clone the repository using the recursive flag.
```bash
git clone --recursive https://github.com/SebastianM0eller/Pong
cd Pong
```

* Build the game using the  appropriate cmake preset.
```bash
cmake --preset release

# If you build for linux:
cmake --build --preset release

# If you build for Windows:
cmake --build --preset windows-release
```

### How to Play
* Run the executable located in the build/release folder.
    * Select the game mode.
    * Move the player (w/s) and for 2-player (i/k).
    * First to 10 wins!
