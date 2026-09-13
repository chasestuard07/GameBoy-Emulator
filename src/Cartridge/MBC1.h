#pragma once

#include "MBC.h"
#include <vector>

class MBC1 : public MBC
{
public:
    MBC1(std::vector<uint8_t>& rom);

    uint8_t Read(uint16_t address) override;
    void Write(uint16_t address, uint8_t value) override;


private:
    std::vector<uint8_t>& rom;
    
    uint8_t romBank;
    uint8_t romBankUpper;
    uint8_t romBankLower;
    uint8_t bankingMode;

    std::vector<uint8_t> ram;
    bool ramEnabled;
    uint8_t ramBank;
};