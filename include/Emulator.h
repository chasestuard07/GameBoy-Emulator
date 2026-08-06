#pragma once

#include "Cartridge.h"
#include "CPU.h"
#include "MemoryBus.h"
#include "PPU.h"

class Emulator
{
public:
    Emulator();

    void Step();
    uint8_t* GetFrameBuffer();
    bool FrameReady();
    void ClearFrameReady();
    Joypad* GetJoypad();
    bool setROM(std::string romPath);
    void Reset();

private:
    Cartridge cartridge;
    CPU cpu;
    MemoryBus memorybus;
};