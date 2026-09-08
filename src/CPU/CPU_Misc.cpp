#include "CPU.h"

void CPU::Rlca()
{
    uint8_t carry = (A & 0x80) >> 7;

    A = (A << 1) | carry;

    SetZeroFlag(false);
    SetSubtractFlag(false);
    SetHalfCarryFlag(false);
    SetCarryFlag(carry);
}

void CPU::Rrca()
{
    uint8_t carry = (A & 0x01);

    A = (A >> 1) | (carry << 7);

    SetZeroFlag(false);
    SetSubtractFlag(false);
    SetHalfCarryFlag(false);
    SetCarryFlag(carry);
}

void CPU::Rla()
{
    uint8_t oldCarry = GetCarryFlag();

    uint8_t newCarry = (A & 0x80) >> 7;

    A = (A << 1) | oldCarry;

    SetZeroFlag(false);
    SetSubtractFlag(false);
    SetHalfCarryFlag(false);
    SetCarryFlag(newCarry);
}

void CPU::Rra()
{
    uint8_t oldCarry = GetCarryFlag();

    uint8_t newCarry = A & 0x01;

    A = oldCarry << 7 | (A >> 1);

    SetZeroFlag(false);
    SetSubtractFlag(false);
    SetHalfCarryFlag(false);
    SetCarryFlag(newCarry);
}