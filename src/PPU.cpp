#include "PPU.h"

PPU::PPU()
{
    lcdc = 0x91;
    stat = 0x85;

    scy = 0x00;
    scx = 0x00;

    ly = 0x00;
    lyc = 0x00;

    bgp = 0xFC;
    obp0 = 0xFF;
    obp1 = 0xFF;

    wy = 0x00;
    wx = 0x00;

    dma = 0;

    lineCycles = 0;

    vBlankInterrupt = false;

    mode = 2;

    for (int i = 0; i < 0x2000; i++)
        vram[i] = 0;

    for (int i = 0; i < 0xA0; i++)
        oam[i] = 0;
}

uint8_t PPU::ReadRegister(uint16_t address)
{
    switch(address)
    {
        case 0xFF40: return lcdc;
        case 0xFF41: return stat;
        case 0xFF42: return scy;
        case 0xFF43: return scx;
        case 0xFF44: return ly;
        case 0xFF45: return lyc;

        case 0xFF47: return bgp;
        case 0xFF48: return obp0;
        case 0xFF49: return obp1;

        case 0xFF4A: return wy;
        case 0xFF4B: return wx;

        default:
            return 0xFF;
    }
}
void PPU::WriteRegister(uint16_t address, uint8_t value)
{
    switch(address)
    {
        case 0xFF40: lcdc = value; return;
        case 0xFF41: stat = value; return;
        case 0xFF42: scy = value; return;
        case 0xFF43: scx = value; return;
        case 0xFF44: ly = value; return;
        case 0xFF45: lyc = value; return;

        case 0xFF47: bgp = value; return;
        case 0xFF48: obp0 = value; return;
        case 0xFF49: obp1 = value; return;

        case 0xFF4A: wy = value; return;
        case 0xFF4B: wx = value; return;

        default:
            return;
    }
}

uint8_t PPU::ReadVRAM(uint16_t address)
{
    return vram[address - 0x8000];
}
uint8_t PPU::ReadOAM(uint16_t address)
{
    return oam[address - 0xFE00];
}
void PPU::WriteVRAM(uint16_t address, uint8_t value)
{
    vram[address - 0x8000] = value;
}
void PPU::WriteOAM(uint16_t address, uint8_t value)
{
    oam[address - 0xFE00] = value;
}


uint8_t PPU::ReadDMA()
{
    return dma;
}

void PPU::Tick(int cycles)
{
    lineCycles += cycles;

    while (lineCycles >= 456)
    {
        lineCycles -= 456;

        ly++;

        if (ly == 144)
        {
            vBlankInterrupt = true;
            SetMode(1);
        }
        else if (ly > 153)
        {
            ly = 0;
            SetMode(2);
        }
    }
}

bool PPU::GetVBlankInterrupt()
{
    return vBlankInterrupt;
}


void PPU::ClearVBlankInterrupt()
{
    vBlankInterrupt = false;
}

void PPU::SetMode(uint8_t newMode)
{
    mode = newMode;

    stat &= 0xFC;
    stat |= mode;
}

void PPU::DoDMA(uint8_t value, uint8_t index)
{
        std::cout << "OAM[" 
              <<  static_cast<int>(index) 
              << "] = "
              <<  static_cast<int>(value)
              << std::endl;
    oam[index] = value;
}