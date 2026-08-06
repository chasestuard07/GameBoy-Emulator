#pragma once

#include <cstdint>
#include <iostream>

class PPU
{
public:
    PPU();

    uint8_t ReadRegister(uint16_t address);
    void WriteRegister(uint16_t address, uint8_t value);
    uint8_t ReadVRAM(uint16_t address);
    uint8_t ReadOAM(uint16_t address);
    void WriteVRAM(uint16_t address, uint8_t value);
    void WriteOAM(uint16_t address, uint8_t value);
    uint8_t ReadDMA();

    void Tick(int cycles);

    bool GetVBlankInterrupt();
    void ClearVBlankInterrupt();
    bool FrameReady();
    void ClearFrameReady();

    void DoDMA(uint8_t value, uint8_t index);
    uint8_t* GetFrameBuffer();

private:
    uint8_t lcdc; // FF40
    uint8_t stat; // FF41
    uint8_t scy;  // FF42
    uint8_t scx;  // FF43
    uint8_t ly;   // FF44
    uint8_t lyc;  // FF45

    uint8_t bgp;  // FF47
    uint8_t obp0; // FF48
    uint8_t obp1; // FF49
    uint8_t wy;   // FF4A
    uint8_t wx;   // FF4B

    uint8_t vram[0x2000]; // 0x8000-0x9FFF
    uint8_t oam[0xA0];    // 0xFE00-0xFE9F

    uint8_t dma;

    int lineCycles;

    bool vBlankInterrupt;
    bool frameReady;

    uint8_t mode;

    void SetMode(uint8_t newMode);

    uint8_t framebuffer[160 * 144] = {};

    uint8_t GetTilePixel(uint16_t tileAddress, int x, int y);
    void RenderTile(uint16_t tileAddress, int screenX, int screenY);
    uint8_t LocateTile(int screenX, int screenY);
    void RenderScanline();
    void RenderSprites();
    

};