#include "Emulator.h"

#include <thread>
#include <chrono>

Emulator::Emulator(std::string romPath)
{
    if(!cartridge.LoadROM(romPath))
    {
        std::cout << "Failed to load ROM\n";
        return;
    }

    std::cout << "ROM loaded\n";
    
    cartridge.LoadROM(romPath); 
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
