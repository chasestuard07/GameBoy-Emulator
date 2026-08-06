# Game Boy Emulator

A custom Game Boy (DMG) emulator written in C++.

This project was built from scratch to explore computer architecture, emulation, graphics pipelines, and low-level systems programming. The emulator implements the core components required to run Game Boy software, including CPU execution, memory management, timing, input handling, and graphics rendering.

## Features

* Game Boy ROM loading (`.gb` files)
* LR35902 CPU emulation
* Memory bus and cartridge handling
* PPU graphics rendering
* Timer implementation
* Joypad input support
* Pause/resume emulation
* Emulator reset
* SDL-based display output

## Controls

| Key        | Game Boy Input |
| ---------- | -------------- |
| Arrow Keys | D-Pad          |
| Z          | A Button       |
| X          | B Button       |
| Enter      | Start          |
| Backspace  | Select         |
| P          | Pause/Resume   |
| R          | Reset          |

## Running the Emulator

1. Download or build the emulator executable.
2. Launch the emulator.
3. Select a Game Boy ROM (`.gb`) using the file picker.
4. The game will begin running automatically.

## Building

### Requirements

* C++ compiler with C++17 support
* CMake
* SDL2
* Native File Dialog Extended

### Build Steps

```bash
git clone <repository-url>
cd GameboyEmulator

mkdir build
cd build

cmake ..
cmake --build .
```

The executable will be generated in the build directory.

## Current Limitations

This emulator is still under active development. Some Game Boy features are not currently implemented or may not be fully compatible:

* Limited ROM compatibility (tested primarily with Tetris)
* No audio support
* No save battery support
* No Game Boy Color features
* Compatibility is still being improved

## Goals

Future improvements may include:

* Improved ROM compatibility
* Audio support
* Save states
* Debugging tools
* Additional Game Boy hardware features
