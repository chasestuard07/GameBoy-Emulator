#include "MBC1.h"

MBC1::MBC1(std::vector<uint8_t>& rom)
    : rom(rom)
{
    romBank = 1;
    bankingMode = 0;
    ram.resize(0x8000);
    ramEnabled = false;
    ramBank = 0;
}

uint8_t MBC1::Read(uint16_t address)
{
    switch (bankingMode)
    {
        case 0:
            if (address < 0x4000)
                return rom[address];

            else if (address >= 0xA000 && address <= 0xBFFF)
            {
                if (!ramEnabled)
                    return 0xFF;

                return ram[address - 0xA000];
            }

            else
            {
                size_t location = (romBank * 0x4000) + (address - 0x4000);
                return rom[location];
            }

        case 1:
            if (address < 0x4000)
            {
                size_t location = ((romBankUpper >> 5) * 0x4000) + address;
                return rom[location];
            }

            else if (address >= 0xA000 && address <= 0xBFFF)
            {
                if (!ramEnabled)
                    return 0xFF;

                size_t location = (ramBank * 0x2000) + (address - 0xA000);
                return ram[location];
            }

            else
            {
                size_t location = (romBank * 0x4000) + (address - 0x4000);
                return rom[location];
            }

        default:
            return 0xFF;
    }
}

void MBC1::Write(uint16_t address, uint8_t value)
{
    if (address < 0x2000)
    {
        ramEnabled = ((value & 0x0F) == 0x0A);
        return;
    }

    if (address <= 0x3FFF)
    {
        romBankLower = value & 0x1F;
    }

    else if (address <= 0x5FFF)
    {
        if (bankingMode == 0)
            romBankUpper = value & 0x60;
        else
            ramBank = (value & 0x60) >> 5;
    }

    else if (address <= 0x7FFF)
    {
        bankingMode = value & 0x01;
    }

    else if (address >= 0xA000 && address <= 0xBFFF)
    {
        if (!ramEnabled)
            return;

        size_t location = (ramBank * 0x2000) + (address - 0xA000);
        ram[location] = value;
    }

    romBank = romBankLower | romBankUpper;

    if (romBank == 0)
        romBank = 1;
}