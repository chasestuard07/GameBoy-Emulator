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
    bool IME;
    int InterruptDelayTimer;
    bool stop;

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

    void Cb();
    uint8_t ReadCB(uint8_t reg);
    void WriteCB(uint8_t reg, uint8_t value);
    void Rlc(uint8_t reg);
    void Rrc(uint8_t reg);
    void Rl(uint8_t reg);
    void Rr(uint8_t reg);
    void Sla(uint8_t reg);
    void Sra(uint8_t reg);
    void Swap(uint8_t reg);
    void Srl(uint8_t reg);
    void Bit(uint8_t bit, uint8_t reg);
    void Res(uint8_t bit, uint8_t reg);
    void Set(uint8_t bit, uint8_t reg);

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

    void Jp(bool condition);
    void Jr(bool condition);

    void Scf();
    void Cpl();
    void Ccf();



    void Rlca();
    void Rrca();
    void Rla();
    void Rra();

    void PushWord(uint16_t word);
    uint16_t PopWord();
    void WriteWord(uint16_t address, uint16_t value);



    uint8_t FetchByte();
    uint16_t FetchWord();
    void Call(bool condition);
    void Ret(bool condition);
    void Reti();
    void Rst(uint16_t address);


    uint16_t GetBC();
    void SetBC(uint16_t value);
    uint16_t GetDE();
    void SetDE(uint16_t value);
    uint16_t GetHL();
    void SetHL(uint16_t value);
    void SetSP(uint16_t value);
    uint16_t GetAF();
    void SetAF(uint16_t value);


    uint16_t Inc16(uint16_t r);
    uint16_t Dec16(uint16_t r);
    uint16_t Add16(uint16_t r1, uint16_t r2);

    void LdAHL(bool increment);
    void LdHLA(bool increment);
    void IncHL();
    void DecHL();
    void LdHLSPr8();
    void Daa();

    uint16_t AddSignedSP(uint8_t rawOffset);

    void SetZeroFlag(bool b);
    bool GetZeroFlag();
    void SetSubtractFlag(bool b);
    bool GetSubtractFlag();
    void SetHalfCarryFlag(bool b);
    bool GetHalfCarryFlag();
    void SetCarryFlag(bool b);
    bool GetCarryFlag();

    void HandleInterrupts();
    void Stop();

};