#include "Emulator.h"

#include <thread>
#include <chrono>

Emulator::Emulator()
{
    memorybus.SetCartridge(&cartridge);
    cpu.SetMemoryBus(&memorybus);
}

void Emulator::Step()
{
    int cycles = cpu.Step();
    memorybus.Tick(cycles);
}

uint8_t* Emulator::GetFrameBuffer()
{
    return memorybus.GetFrameBuffer();
}

bool Emulator::FrameReady()
{
    return memorybus.FrameReady();
}

void Emulator::ClearFrameReady()
{
    memorybus.ClearFrameReady();
}

Joypad* Emulator::GetJoypad()
{
    return memorybus.GetJoypad();
}

bool Emulator::setROM(std::string romPath)
{
    if (cartridge.LoadROM(romPath)) return true;

    std::cout << "Failed to load ROM"; return false;
}

void Emulator::Reset()
{
    cpu.Reset();
    memorybus.Reset();
}