#pragma once

#include "Cartridge.h"

class MemoryBus
{
public:
    MemoryBus();

    void SetCartridge(Cartridge* cart);
    
    uint8_t Read(uint16_t address);
    void Write(uint16_t address, uint8_t value);

private:
    Cartridge* CartridgePtr;
};