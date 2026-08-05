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
    uint8_t* GetFrameBuffer();
    bool FrameReady();
    void ClearFrameReady();

private:
    Cartridge cartridge;
    CPU cpu;
    MemoryBus memorybus;
};