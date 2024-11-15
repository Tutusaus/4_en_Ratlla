
# 📊 AI-Powered Board Game in C

![Game Screenshot](https://via.placeholder.com/800x400?text=Game+Screenshot) <!-- Replace with an actual screenshot if available -->

## Table of Contents
- [Introduction](#introduction)
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [How It Works](#how-it-works)
- [Future Improvements](#future-improvements)
- [Contributing](#contributing)
- [License](#license)

## Introduction
Welcome to the **AI-Powered Board Game in C**! This project is a terminal-based strategy game where you compete against a computer opponent on an 8x8 grid. The AI uses a sophisticated **minimax algorithm** with **alpha-beta pruning** to simulate challenging gameplay, ensuring a competitive experience every time you play.

## Features
- **Single-player mode**: Battle against an AI opponent with smart decision-making capabilities.
- **8x8 Board**: Classic grid-based gameplay using ASCII characters:
  - `O` for Player moves
  - `X` for Computer moves
  - ` ` (Space) for empty cells
- **AI Strategy**:
  - Minimax algorithm with a configurable depth limit for move predictions.
  - Alpha-beta pruning to optimize decision-making speed.
- **Modular code**: Organized with structs and functions for readability and maintainability.

## Installation
Follow these steps to set up the game on your local machine:

### Prerequisites
- GCC compiler (or any C compiler of your choice)
- Terminal/Command Line Interface

### Clone the Repository
`bash
git clone https://github.com/Tutusaus/4_en_Ratlla.git
cd 4_en_Ratlla
`

### Compile the Code
\`\`\`bash
gcc main.c -o boardgame
\`\`\`

## Usage
Once compiled, you can run the game by executing:

\`\`\`bash
./boardgame
\`\`\`

### Gameplay Instructions
- The game starts with an empty 8x8 board.
- Players take turns placing their marks (`O` for Player, `X` for Computer) on the board.
- The objective is to outsmart your opponent by strategically placing your marks.
- The AI uses an advanced algorithm to make decisions, providing a challenging opponent.

## How It Works
The game leverages a **tree data structure** to simulate possible game states:
- Each game state is represented by a `node` struct containing:
  - A 2D array for the board (`tauler[N][N]`)
  - A list of possible moves (children nodes)
  - A heuristic value for evaluating the game state
- The AI uses a **minimax algorithm** with **alpha-beta pruning**:
  - **Minimax** evaluates all possible moves to determine the optimal one.
  - **Alpha-beta pruning** reduces the number of nodes evaluated by the algorithm, making the AI faster and more efficient.
- You can adjust the AI difficulty by modifying the depth constant (`PROFUNDITAT`).

## Future Improvements
Here are some enhancements planned for future versions:
- **Graphical User Interface (GUI)**: Develop a visual version using libraries like SDL or OpenGL.
- **Difficulty Levels**: Add adjustable difficulty settings for the AI.
- **Multiplayer Support**: Enable two-player mode.
- **Save/Load Game**: Implement functionality to save and resume games.

## Contributing
Contributions are welcome! If you would like to contribute to this project, please follow these steps:
1. Fork the repository.
2. Create a new branch (`git checkout -b feature-branch`).
3. Commit your changes (`git commit -m 'Add new feature'`).
4. Push to the branch (`git push origin feature-branch`).
5. Open a Pull Request.

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
