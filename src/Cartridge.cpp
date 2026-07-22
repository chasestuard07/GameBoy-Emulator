#include "Cartridge.h"
#include <fstream>

Cartridge::Cartridge()
{
    romLoaded = false;

}
bool Cartridge::LoadROM(std::string filename)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open())
    {
        return false;
    }

    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    
    file.seekg(0, std::ios::beg);
    rom.resize(size);
    file.read(reinterpret_cast<char*>(rom.data()), size);

    romLoaded = true;

    return true;
}
uint8_t Cartridge::Read(uint16_t address) 
{
    if (address >= rom.size())
    {
        return 0xFF;
    }

    return rom[address];
}
void Cartridge::Write(uint16_t address, uint8_t value) 
{

}
