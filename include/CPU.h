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
    uint8_t opcode;
    bool halt;

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



    void LDrd8(uint8_t& r);
    void LDrtr(uint8_t& r1 ,uint8_t& r2);

    void Inc(uint8_t& r);
    void Dec(uint8_t& r);
    void Add(uint8_t value);
    void Adc(uint8_t value);
    void Sub(uint8_t value);
    void Sbc(uint8_t value);
    void And(uint8_t value);
    void Or(uint8_t value);
    void Xor(uint8_t value);
    void Cp(uint8_t value);



    void Scf();
    uint8_t FetchByte();
    uint16_t FetchWord();



    uint16_t GetBC();
    void SetBC(uint16_t value);
    uint16_t GetDE();
    void SetDE(uint16_t value);
    uint16_t GetHL();
    void SetHL(uint16_t value);
    void SetSP(uint16_t value);
    uint16_t Inc16(uint16_t r);
    uint16_t Dec16(uint16_t r);
    uint16_t Add16(uint16_t r1, uint16_t r2);


    void SetZeroFlag(bool b);
    void SetSubtractFlag(bool b);
    void SetHalfCarryFlag(bool b);
    void SetCarryFlag(bool b);
    bool GetCarryFlag();

};