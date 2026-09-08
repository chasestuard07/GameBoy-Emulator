#include "CPU.h"

uint16_t CPU::GetBC()
{
    return (B << 8) | C;
}

void CPU::SetBC(uint16_t value)
{
    B = (value >> 8);
    C = (value & 0x00FF);
}

uint16_t CPU::GetDE()
{
    return (D << 8) | E;
}

void CPU::SetDE(uint16_t value)
{
    D = (value >> 8);
    E = (value & 0x00FF);
}

uint16_t CPU::GetHL()
{
    return (H << 8) | L;
}

void CPU::SetHL(uint16_t value)
{
    H = (value >> 8);
    L = (value & 0x00FF);
}

void CPU::SetSP(uint16_t value)
{
    SP = value;
}

uint16_t CPU::GetAF()
{
    return (A << 8) | F;
}

void CPU::SetAF(uint16_t value)
{
    A = (value >> 8);
    F = (value & 0x00F0);
}

void CPU::SetZeroFlag(bool b) 
{
    if (b == true) F |= 0x80;
    else F &= ~0x80;
}

bool CPU::GetZeroFlag()
{
    return (F & 0x80);
}

void CPU::SetSubtractFlag(bool b)
{
    if (b == true) F |= 0x40;
    else F &= ~0x40;
}

bool CPU::GetSubtractFlag()
{
    return (F & 0x40);
}

void CPU::SetHalfCarryFlag(bool b)
{
    if (b == true) F |= 0x20;
    else F &= ~0x20;
}

bool CPU::GetHalfCarryFlag()
{
    return (F & 0x20);
}

void CPU::SetCarryFlag(bool b)
{
    if (b == true) F |= 0x10;
    else F &= ~0x10;
}

bool CPU::GetCarryFlag()
{
    return (F & 0x10);
}