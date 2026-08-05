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
        return ppu.ReadVRAM(address);
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
        return ppu.ReadOAM(address);
    }
    else if (address >= IO.start && address <= IO.end)
    {
        switch(address)
        {
            // Serial
            case 0xFF01:
                return serialData;

            case 0xFF02:
                return 0x81;

            // Timer
            case 0xFF04:
            case 0xFF05:
            case 0xFF06:
            case 0xFF07:
                return timer.Read(address);

            // Joypad
            case 0xFF00:
                return joypad.Read();

            // PPU registers
            case 0xFF40: // LCDC
            case 0xFF41: // STAT
            case 0xFF42: // SCY
            case 0xFF43: // SCX
            case 0xFF44: // LY
            case 0xFF45: // LYC
            case 0xFF47: // BGP
            case 0xFF48: // OBP0
            case 0xFF49: // OBP1
            case 0xFF4A: // WY
            case 0xFF4B: // WX
                return ppu.ReadRegister(address);

            // DMA register
            case 0xFF46:
                return ppu.ReadDMA();

            default:
                return 0xFF;
        }
    }
    else if (address >= HRAM.start && address <= HRAM.end)
    {
        return hram[address - HRAM.start];
    }
    else if (address == 0xFFFF) // IE
    {
        return ie;
    }
    else if (address == 0xFF0F) // IF
    {
        return If;
    }
    else
    {
        return 0xFF;
    }
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
        ppu.WriteVRAM(address, value); 
    }
    else if (address >= EXT_RAM.start && address <= EXT_RAM.end)
    {  
    }
    else if (address >= WRAM.start && address <= WRAM.end) wram[address-WRAM.start] = value;
    else if (address >= ECHO_RAM.start && address <= ECHO_RAM.end) wram[address-ECHO_RAM.start] = value;
    else if (address >= OAM.start && address <= OAM.end)
    {  
        ppu.WriteOAM(address, value); 
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
        if (address >= 0xFF04 && address <= 0xFF07)
        {
            timer.Write(address, value); 
            return;
        }
        if (address >= 0xFF40 && address <= 0xFF48)
        {
            if(address == 0xFF46)
            {
                uint16_t source = value << 8;

                for(int i = 0; i < 0xA0; i++)
                {
                    ppu.DoDMA(Read(source + i), i);
                }
            }
            else 
            {
                ppu.WriteRegister(address, value);
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
    else if (address == IF.end)
    {  
        If = value;
    }
}

void MemoryBus::Tick(int cycles)
{
    timer.Tick(cycles);
    ppu.Tick(cycles);

    if (timer.GetInterrupt())
    {
        If |= 0x04;
        timer.ClearInterrupt();
    }
    if (ppu.GetVBlankInterrupt())
    {
        If |= 0x01;
        ppu.ClearVBlankInterrupt();
    }
}

uint8_t* MemoryBus::GetFrameBuffer()
{
    return ppu.GetFrameBuffer();
}

bool MemoryBus::FrameReady()
{
    return ppu.FrameReady();
}

void MemoryBus::ClearFrameReady()
{
    ppu.ClearFrameReady();
}