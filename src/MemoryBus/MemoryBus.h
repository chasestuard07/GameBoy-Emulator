#pragma once

#include "Cartridge/Cartridge.h"
#include "Timer/Timer.h"
#include "PPU/PPU.h"
#include "Joypad/Joypad.h"

struct MemoryRegion
{
    uint16_t start;
    uint16_t end;
};

class MemoryBus
{
public:
    MemoryBus();

    void SetCartridge(Cartridge* cart);
    
    uint8_t Read(uint16_t address);
    void Write(uint16_t address, uint8_t value);

    uint8_t ie = 0;
    uint8_t If = 0;

    void Tick(int cycles);

    uint8_t* GetFrameBuffer();
    bool FrameReady();
    void ClearFrameReady();
    Joypad* GetJoypad();
    void Reset();

private:
    Cartridge* CartridgePtr;
    Timer timer;
    PPU ppu;
    Joypad joypad;

    MemoryRegion ROM       = {0x0000, 0x7FFF};
    MemoryRegion VRAM      = {0x8000, 0x9FFF};
    MemoryRegion EXT_RAM   = {0xA000, 0xBFFF};
    MemoryRegion WRAM      = {0xC000, 0xDFFF};
    MemoryRegion ECHO_RAM  = {0xE000, 0xFDFF};
    MemoryRegion OAM       = {0xFE00, 0xFE9F};
    MemoryRegion IO        = {0xFF00, 0xFF7F};
    MemoryRegion HRAM      = {0xFF80, 0xFFFE};
    MemoryRegion IF        = {0xFF0F, 0xFF0F};
    MemoryRegion IE        = {0xFFFF, 0xFFFF};

    uint8_t wram[0x2000] = {};
    uint8_t hram[0x7F] = {};

    uint8_t serialData;

};