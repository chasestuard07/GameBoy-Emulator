cmake_minimum_required(VERSION 3.20)

project(GameBoyEmulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(SDL2 REQUIRED)
add_subdirectory(libraries/nativefiledialog-extended-master)

add_executable(GameBoyEmulator
    src/main.cpp
    src/Emulator/Emulator.cpp
    src/MemoryBus/MemoryBus.cpp
    src/Timer/Timer.cpp
    src/Joypad/Joypad.cpp
    src/PPU/PPU.cpp
    src/Display/Display.cpp
    src/FileDialog/FileDialog.cpp

    src/Cartridge/Cartridge.cpp
    src/Cartridge/MBC1.cpp

    src/CPU/CPU.cpp
    src/CPU/CPU_Load.cpp
    src/CPU/CPU_CB.cpp
    src/CPU/CPU_Arithmetic.cpp
    src/CPU/CPU_Control.cpp
    src/CPU/CPU_Interrupts.cpp
    src/CPU/CPU_Registers.cpp
    src/CPU/CPU_Misc.cpp
)

target_include_directories(GameBoyEmulator PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)


target_link_libraries(GameBoyEmulator PRIVATE SDL2::SDL2)
target_link_libraries(GameBoyEmulator PRIVATE nfd)