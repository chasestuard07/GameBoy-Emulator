#include <iostream>
#include "Emulator.h"

int main()
{
    Emulator emulator;

    Cartridge cart;
    if (cart.LoadROM("../tests/test4.gb")) 
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

    for (int i = 0; i < 15; i++) {
        cpu.Step();
    }
    cpu.Debug();
    

    return 0;
    
}