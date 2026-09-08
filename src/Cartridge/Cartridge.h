#pragma once

#include <cstdint>
#include <vector>
#include <string>

class Cartridge
{
public:
    Cartridge();
    
    bool LoadROM(std::string filename);
    
    uint8_t Read(uint16_t address);
    void Write(uint16_t address, uint8_t value);

private: 
    std::vector<uint8_t> rom;
    bool romLoaded;
};