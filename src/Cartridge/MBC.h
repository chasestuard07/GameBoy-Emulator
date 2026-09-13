#pragma once

#include <cstdint>

class MBC
{
public:
    virtual ~MBC() = default;

    virtual uint8_t Read(uint16_t address) = 0;
    virtual void Write(uint16_t address, uint8_t value) = 0;
};