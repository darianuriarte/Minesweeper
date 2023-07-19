# Minesweeper Game

This is a Minesweeper game implementation using the SFML library. The game allows players to uncover tiles on a grid while avoiding mines. The objective is to uncover all non-mine tiles without triggering any mines.

## Features

- Customizable board size and number of mines.
- Three difficulty levels: Easy, Medium, and Hard.
- Left-click to uncover a tile.
- Right-click to flag a tile as a potential mine.
- Face button to start a new game or reset the current game.
- Test boards for debugging and testing purposes.

## Requirements

- C++ compiler with C++11 support.
- SFML library (version 2.5 or higher).

## Installation

1. Clone the repository:

   ```
   git clone https://github.com/your-username/minesweeper-game.git
   ```

2. Build the game using your preferred C++ compiler. Make sure to link against the SFML library.

   Example using g++:

   ```
   g++ main.cpp -o minesweeper -lsfml-graphics -lsfml-window -lsfml-system
   ```

3. Run the game:

   ```
   ./minesweeper
   ```

## How to Play

- The game window displays a grid of covered tiles.
- Use the left mouse button to uncover tiles.
- Use the right mouse button to flag or unflag tiles as potential mines.
- The number on a revealed tile represents the number of adjacent mines.
- If you uncover a tile with a mine, the game is over.
- To win, uncover all non-mine tiles.
- Click the face button to start a new game or reset the current game.
- You can also load test boards for debugging and testing purposes.

## Customization

- The game allows customization of the board size and the number of mines.
- Modify the `numberOfRows` and `numberOfColumns` variables in the code to change the board size.
- Adjust the `numberOfMines` variable to set the desired number of mines.
- The game also provides three predefined difficulty levels: Easy, Medium, and Hard.

## Credits

This Minesweeper game implementation is developed by Darian Uriarte (https://github.com/your-username).](https://github.com/darianuriarte)

