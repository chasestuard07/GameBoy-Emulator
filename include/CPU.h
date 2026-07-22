#pragma once

#include "MemoryBus.h"
#include <cstdint>
#include <iostream>

class CPU
{
public:
    CPU();
    
    void SetMemoryBus(MemoryBus* bus);
    
    void Step();
    void Execute();

    void Debug();

private:
    MemoryBus* bus;

    uint8_t A;
    uint8_t B;
    uint8_t C;
    uint8_t D;
    uint8_t E;
    uint8_t F;
    uint8_t H;
    uint8_t L;

    uint16_t PC;
    uint16_t SP;

    uint8_t opcode;

    void LDrd8(uint8_t& r);
    void LDrtr(uint8_t& r1 ,uint8_t& r2);
};