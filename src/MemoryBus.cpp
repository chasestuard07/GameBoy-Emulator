#include "MemoryBus.h"

MemoryBus::MemoryBus() 
{
    CartridgePtr = nullptr;
}

void MemoryBus::SetCartridge(Cartridge* cart) 
{
    CartridgePtr = cart;
}


uint8_t MemoryBus::Read(uint16_t address)
{
    if (address <= 0x7FFF)
    {
        if (CartridgePtr == nullptr) 
        {
            return 0xff;
        }
        return CartridgePtr->Read(address);
    }
}

void MemoryBus::Write(uint16_t address, uint8_t value)
{
    if (address <= 0x7FFF)
    {
        if (CartridgePtr == nullptr) 
        {
            return;
        }
        CartridgePtr->Write(address, value);
        return;
    }
}