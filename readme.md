
# Snake Game

A classic Snake game implementation using SDL3 (Simple DirectMedia Layer 3) in C.

## Description

This is a simple Snake game where the player controls a snake moving on a grid. The snake wraps around the screen edges, allowing continuous movement in any direction.

## Features

- Grid-based gameplay on an 800x800 pixel window
- Smooth snake movement with keyboard controls
- Screen wrapping (snake reappears on opposite side when hitting edges)
- Clean graphics with white grid lines on black background

## Prerequisites

- SDL3 library installed on your system
- C compiler (GCC, Clang, etc.)
- Make (optional, for build automation)

## Installation

### Installing SDL3

#### Linux (Ubuntu/Debian):
```bash
# SDL3 may need to be built from source
# Check SDL3 documentation for latest installation instructions
```

#### macOS:
```bash
brew install sdl3
```

#### Windows:
Download SDL3 development libraries from the official SDL website.

## Building

```bash
gcc snake.c -o snake $(sdl3-config --cflags --libs)
```

Or with pkg-config:
```bash
gcc snake.c -o snake $(pkg-config --cflags --libs sdl3)
```

## Controls

- **Arrow Keys**: Control snake direction
  - `→` (Right Arrow): Move right
  - `←` (Left Arrow): Move left
  - `↑` (Up Arrow): Move up
  - `↓` (Down Arrow): Move down

> **Note**: The snake cannot reverse direction (e.g., cannot move left while moving right).

## How to Play

1. Run the compiled executable
2. Use arrow keys to control the snake's movement
3. The snake automatically wraps around screen edges
4. Close the window or press the window close button to exit

## Game Specifications

| Specification | Value |
|--------------|-------|
| Window Size | 900x900 pixels |
| Grid Cell Size | 30x30 pixels |
| Grid Dimensions | 30x30 cells |
| Movement Speed | 250ms delay |

## Screenshots

![Gameplay Screenshot](assets/Screenshot_20260211_135322.png)

![Snake Wrapping Around Edge]()

## Current Limitations

- No apple/food collection implemented yet
- Snake doesn't grow in length
- No collision detection with self
- No score tracking
- No game over condition

## Future Enhancements

- [ ] Add apple spawning and collection
- [ ] Implement snake growth mechanism
- [ ] Add self-collision detection
- [ ] Implement score system
- [ ] Add difficulty levels (speed adjustment)
- [ ] Game over screen and restart functionality
- [ ] Sound effects and background music
- [ ] High score tracking

## Code Structure

- **`snakeElement`**: Linked list structure for snake body segments
- **`Apple`**: Structure for food position
- **`Direction`**: Structure for movement direction
- **`draw_grid()`**: Renders the game grid
- **`draw_snake()`**: Renders the snake and handles edge wrapping
- **`move_snake()`**: Updates snake position
- **`clear_screen()`**: Clears the display

## License

This project is open source and available for educational purposes.

## Acknowledgments

Built with [SDL3](https://www.libsdl.org/) - Simple DirectMedia Layer 3

---

**Enjoy the game!** 🐍
