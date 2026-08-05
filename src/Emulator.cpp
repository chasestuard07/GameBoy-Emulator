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

void Emulator::Run()
{
    while(true) Step();
}

void Emulator::Step()
{
    int cycles = cpu.Step();
    memorybus.Tick(cycles);
}
