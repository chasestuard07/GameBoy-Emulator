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

    switch(mode)
    {
        case 2: // OAM
            if(lineCycles >= 80)
            {
                lineCycles -= 80;
                SetMode(3);
            }
            break;

        case 3: // Pixel transfer
            if(lineCycles >= 172)
            {
                lineCycles -= 172;

                RenderScanline();
                RenderSprites();

                SetMode(0);
            }
            break;

        case 0: // HBlank
            if(lineCycles >= 204)
            {
                lineCycles -= 204;

                ly++;

                if(ly == 144)
                {
                    SetMode(1);
                    vBlankInterrupt = true;
                    frameReady = true;
                }
                else
                {
                    SetMode(2);
                }
            }
            break;

        case 1: // VBlank
            if(lineCycles >= 456)
            {
                lineCycles -= 456;

                ly++;

                if(ly > 153)
                {
                    ly = 0;
                    SetMode(2);
                }
            }
            break;
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

bool PPU::FrameReady()
{
    return frameReady;
}

void PPU::ClearFrameReady()
{
    frameReady = false;
}



void PPU::SetMode(uint8_t newMode)
{
    mode = newMode;

    stat &= 0xFC;
    stat |= mode;
}

void PPU::DoDMA(uint8_t value, uint8_t index)
{
    oam[index] = value;
}

uint8_t PPU::GetTilePixel(uint16_t tileAddress, int x, int y)
{
    // Each row takes 2 bytes
    uint16_t rowAddress = tileAddress + (y * 2);

    uint8_t lowByte = vram[rowAddress - 0x8000];
    uint8_t highByte = vram[rowAddress - 0x8000 + 1];

    // Pixels are stored left-to-right in bits 7-0
    int bit = 7 - x;

    uint8_t lowBit = (lowByte >> bit) & 1;
    uint8_t highBit = (highByte >> bit) & 1;

    return (highBit << 1) | lowBit;
}
void PPU::RenderTile(uint16_t tileAddress, int screenX, int screenY)
{
    for(int y = 0; y < 8; y++)
    {
        for(int x = 0; x < 8; x++)
        {
            uint8_t color = GetTilePixel(tileAddress, x, y);

            framebuffer[(screenY + y) * 160 + (screenX + x)] = color;
        }
    }
}
uint8_t PPU::LocateTile(int screenX, int screenY)
{
    int tileX = screenX / 8;
    int tileY = screenY / 8;

    uint16_t tileMapAddress = 0x9800 + (tileY * 32) + tileX;
    uint8_t tileNumber = vram[tileMapAddress - 0x8000];
    return tileNumber;
}

void PPU::RenderScanline()
{
    uint8_t bgY = ly + scy;

    for (int x = 0; x < 160; x++)
    {
        uint8_t bgX = x + scx;

        uint8_t tileNumber = LocateTile(bgX, bgY);
        uint16_t tileAddress = 0x8000 + (tileNumber * 16);

        int pixelX = bgX % 8;
        int pixelY = bgY % 8;

        uint8_t color = GetTilePixel(tileAddress, pixelX, pixelY);

        framebuffer[ly * 160 + x] = color;
    }
}

void PPU::RenderSprites()
{
    for(int i = 0; i < 40; i++)
    {
        uint8_t index = i * 4; 
        uint8_t spriteY = oam[index];
        uint8_t spriteX = oam[index + 1];
        uint8_t tile = oam[index + 2];
        uint8_t attributes = oam[index + 3];

        int screenY = spriteY - 16;
        int screenX = spriteX - 8;

        if (ly >= screenY && ly < screenY + 8)
        {
            uint8_t currentRow = ly - screenY;
            uint16_t tileAddress = 0x8000 + (tile * 16);
            uint16_t address = tileAddress + (currentRow * 2);
    
            for (int x = 0; x < 8; x++)
            {
                int bit = 7 - x;

                uint8_t lowByte = vram[address - 0x8000];
                uint8_t highByte = vram[address - 0x8000 + 1];
                uint8_t lowBit = (lowByte >> bit) & 1;
                uint8_t highBit = (highByte >> bit) & 1;

                int color = (highBit << 1) | lowBit;
                if(color != 0)
                {
                    framebuffer[ly * 160 + (screenX + x)] = color;
                }
            }
            
        }

    }   
}


uint8_t* PPU::GetFrameBuffer()
{
    return framebuffer;
}

void PPU::Reset()
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