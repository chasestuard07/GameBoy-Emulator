#include "MemoryBus.h"

#include <iostream>

MemoryBus::MemoryBus() 
{
    CartridgePtr = nullptr;
    serialData = 0;
}

void MemoryBus::SetCartridge(Cartridge* cart) 
{
    CartridgePtr = cart;
}


uint8_t MemoryBus::Read(uint16_t address)
{
    if (address >= ROM.start && address <= ROM.end)
    {
        if (CartridgePtr == nullptr) return 0xff;
        return CartridgePtr->Read(address);
    }
    else if (address >= VRAM.start && address <= VRAM.end)
    {  
        return 0xFF;
    }
    else if (address >= EXT_RAM.start && address <= EXT_RAM.end)
    {  
        return 0xFF;
    }
    else if (address >= WRAM.start && address <= WRAM.end)
    {  
        return wram[address - WRAM.start];
    }
    else if (address >= ECHO_RAM.start && address <= ECHO_RAM.end)
    {  
        return wram[address - ECHO_RAM.start];
    }
    else if (address >= OAM.start && address <= OAM.end)
    {  
        return 0xFF;
    }
    else if (address >= IO.start && address <= IO.end)
    {  
        if (address == 0xFF01) return serialData;
        if (address == 0xFF02) return 0x81;
        return 0xFF;
    }
    else if (address >= HRAM.start && address <= HRAM.end)
    {  
        return hram[address - HRAM.start];
    }
    else if (address == IE.end)
    {  
        return ie;
    }
    else return 0xFF;
}

void MemoryBus::Write(uint16_t address, uint8_t value)
{
    if (address >= ROM.start && address <= ROM.end)
    {
        if (CartridgePtr == nullptr) return;
        CartridgePtr->Write(address, value);
    }
    else if (address >= VRAM.start && address <= VRAM.end)
    {  
    }
    else if (address >= EXT_RAM.start && address <= EXT_RAM.end)
    {  
    }
    else if (address >= WRAM.start && address <= WRAM.end)
    {  
        wram[address-WRAM.start] = value;
    }
    else if (address >= ECHO_RAM.start && address <= ECHO_RAM.end)
    {  
        wram[address-ECHO_RAM.start] = value;
    }
    else if (address >= OAM.start && address <= OAM.end)
    {  
    }
    else if (address >= IO.start && address <= IO.end)
    {  
        if (address == 0xFF01) 
        {
            serialData = value; 
            return;
        }
        if (address == 0xFF02)
        {
            if (value == 0x81)
            {
                std::cout << static_cast<char>(serialData);
            }
            return;
        }
        
    }
    else if (address >= HRAM.start && address <= HRAM.end)
    {  
        hram[address-HRAM.start] = value;
    }
    else if (address == IE.end)
    {  
        ie = value;
    }
}