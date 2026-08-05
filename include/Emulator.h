#pragma once

#include "Cartridge.h"
#include "CPU.h"
#include "MemoryBus.h"
#include "PPU.h"

class Emulator
{
public:
    Emulator(std::string romPath);

    void Step();
    void Run();

private:
    Cartridge cartridge;
    CPU cpu;
    MemoryBus memorybus;
};