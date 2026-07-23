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

    bool IsHalted();

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

    bool halt;

    void LDrd8(uint8_t& r);
    void LDrtr(uint8_t& r1 ,uint8_t& r2);
    void Inc(uint8_t& r);
    void Dec(uint8_t& r);
    void Add(uint8_t value);
    void Adc(uint8_t value);
    void Scf();


    void SetZeroFlag(bool b);
    void SetSubtractFlag(bool b);
    void SetHalfCarryFlag(bool b);
    void SetCarryFlag(bool b);
    bool GetCarryFlag();

};