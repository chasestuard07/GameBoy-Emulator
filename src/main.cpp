#include <iostream>
#include "Emulator.h"

int main()
{
    Emulator emulator;

    Cartridge cart;
    if (cart.LoadROM("../tests/cpu_instrs.gb")) 
    {
        std::cout << "ROM loaded\n";
    }
    else
    {
        std::cout << "ROM failed to load\n";
    }

    MemoryBus bus;
    bus.SetCartridge(&cart);

    CPU cpu;
    cpu.SetMemoryBus(&bus);

    while (!cpu.IsHalted()) 
    {
        cpu.Step();
    }
    
    return 0;
    
}