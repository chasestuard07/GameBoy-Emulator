cmake_minimum_required(VERSION 3.20)

project(GameBoyEmulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

add_executable(GameBoyEmulator
    src/main.cpp
    src/Emulator.cpp
    src/CPU.cpp
    src/Cartridge.cpp
    src/MemoryBus.cpp
    src/Timer.cpp
    src/Joypad.cpp
    src/PPU.cpp
)