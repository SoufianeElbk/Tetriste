# Tetriste

The aim of this project is to develop a small game with pieces characterized by a shape and a color, where the player tries to create repeated patterns of shape or color.

## Features

- **Dynamic Gameplay**: Incorporates both color and shape elements, adding a unique challenge beyond traditional Tetris.
- **Customizable Difficulty**: Adjustable color and shape counts to increase or decrease game complexity.
- **High Score Tracking**: Includes functionality to save and update player rankings based on high scores.
- **Save and Load Game**: Players can save their current game state and load it later to continue playing.

## Getting Started

### Prerequisites

- Windows OS
- C++17 Compiler (e.g., GCC, Clang)
- Make sure your compiler supports C++17 features and that your environment variables are set correctly.

### Installation

1. Clone the repository:
```bash
git clone https://github.com/SoufianeElbk/tetriste.git
```

2. Navigate to the cloned directory:
```bash
    cd tetriste
```
3. Compile the source code using Makefile:
```bash
   make debug
```
or 
```bash
   g++ tetriste.cpp tetriste_cli.cpp main.cpp -o tetriste
```
4. Run the game:
```bash
   make run
```
or
```bash
   ./tetriste
```