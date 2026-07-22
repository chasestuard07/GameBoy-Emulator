#include <iostream>
#include "Emulator.h"

int main()
{
    Emulator emulator;

    Cartridge cart;
    if (cart.LoadROM("../tests/test1.gb")) 
    {
        std::cout << "ROM loaded\n";
    }
    else
    {
        std::cout << "ROM failed to load\n";
    }

    MemoryBus bus;
    bus.SetCartridge(&cart);


    std::cout   << std::hex
                << (int)bus.Read(0)
                << "\n";

    return 0;
    
}