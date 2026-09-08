#include "CPU.h"

void CPU::Inc(uint8_t& r)
{
    uint8_t og = r;

    r += 1;

    SetZeroFlag(r == 0);
    SetSubtractFlag(false);
    SetHalfCarryFlag((og & 0x0F) == 0x0F);
}

void CPU::Dec(uint8_t& r)
{
    uint8_t og = r;

    r -= 1;

    SetZeroFlag(r == 0);
    SetSubtractFlag(true);
    SetHalfCarryFlag((og & 0x0F) == 0x00);
}

void CPU::Add(uint8_t value)
{
    uint8_t og = A;
    A += value;

    SetZeroFlag(A == 0);
    SetSubtractFlag(false);
    SetHalfCarryFlag((og & 0x0F) + (value & 0x0F) > 0x0F);
    SetCarryFlag((og + value) > 0xFF);
}

void CPU::Adc(uint8_t value)
{
    uint8_t og = A;
    uint8_t carry = GetCarryFlag() ? 1 : 0;
    A += value + carry;


    SetZeroFlag(A == 0);
    SetSubtractFlag(false);
    SetHalfCarryFlag(((og & 0x0F) + (value & 0x0F) + carry) > 0x0F);
    SetCarryFlag((og + value + carry) > 0xFF);
}

void CPU::Sub(uint8_t value)
{
    uint8_t og = A;
    A -= value;

    SetZeroFlag(A == 0);
    SetSubtractFlag(true);
    SetHalfCarryFlag((og & 0x0F) < (value & 0x0F));
    SetCarryFlag(og < value);
}

void CPU::Sbc(uint8_t value)
{
    uint8_t og = A;
    uint8_t carry = GetCarryFlag() ? 1 : 0;
    A -= (value + carry);

    SetZeroFlag(A == 0);
    SetSubtractFlag(true);
    SetHalfCarryFlag((og & 0x0F) < ((value & 0x0F) + carry));
    SetCarryFlag(og < (value + carry));
}

void CPU::And(uint8_t value)
{
    A &= value;

    SetZeroFlag(A == 0);
    SetSubtractFlag(false);
    SetHalfCarryFlag(true);
    SetCarryFlag(false);
}

void CPU::Or(uint8_t value)
{
    A |= value;

    SetZeroFlag(A == 0);
    SetSubtractFlag(false);
    SetHalfCarryFlag(false);
    SetCarryFlag(false);
}

void CPU::Xor(uint8_t value)
{
    A ^= value;

    SetZeroFlag(A == 0);
    SetSubtractFlag(false);
    SetHalfCarryFlag(false);
    SetCarryFlag(false);
}

void CPU::Cp(uint8_t value)
{
    uint8_t result = A - value;

    SetZeroFlag(result == 0);
    SetSubtractFlag(true);
    SetHalfCarryFlag((A & 0x0F) < (value & 0x0F));
    SetCarryFlag(A < value);
}

uint16_t CPU::Inc16(uint16_t r)
{
    r++;
    return r;
}

uint16_t CPU::Dec16(uint16_t r)
{
    r--;
    return r;
}

uint16_t CPU::Add16(uint16_t og, uint16_t value)
{
    
    SetSubtractFlag(false);
    SetHalfCarryFlag(((og & 0x0FFF) + (value & 0x0FFF)) > 0x0FFF);
    SetCarryFlag((og + value) > 0xFFFF);
    return (og + value);
}

void CPU::IncHL()
{
    uint8_t value = bus->Read(GetHL());
    bool halfCarry = (value & 0x0F) == 0x0F;
    value++; 
    bus->Write(GetHL(), value);

    SetZeroFlag(value == 0);
    SetSubtractFlag(false);
    SetHalfCarryFlag(halfCarry);

}

void CPU::DecHL()
{
    uint8_t value = bus->Read(GetHL());
    bool halfCarry = (value & 0x0F) == 0x00;
    value--; 
    bus->Write(GetHL(), value);

    SetZeroFlag(value == 0);
    SetSubtractFlag(true);
    SetHalfCarryFlag(halfCarry);

}

uint16_t CPU::AddSignedSP(uint8_t rawOffset)
{
    int8_t offset = static_cast<int8_t>(rawOffset);

    SetZeroFlag(false);
    SetSubtractFlag(false);
    SetHalfCarryFlag(((SP & 0x0F) + (rawOffset & 0x0F)) > 0x0F);
    SetCarryFlag(((SP & 0xFF) + rawOffset) > 0xFF);

    return SP + offset;
}

void CPU::Daa()
{
    uint8_t correction = 0;
    bool carry = false;

    if (!GetSubtractFlag()) // Addition
    {
        if (GetHalfCarryFlag() || (A & 0x0F) > 9)
            correction |= 0x06;

        if (GetCarryFlag() || A > 0x99)
        {
            correction |= 0x60;
            carry = true;
        }

        A += correction;
    }
    else // Subtraction
    {
        if (GetHalfCarryFlag())
            correction |= 0x06;

        if (GetCarryFlag())
            correction |= 0x60;

        A -= correction;
    }

    SetZeroFlag(A == 0);
    SetHalfCarryFlag(false);

    if (carry)
        SetCarryFlag(true);
}

void CPU::Scf()
{
    SetCarryFlag(true);
    SetSubtractFlag(false);
    SetHalfCarryFlag(false);
}

void CPU::Cpl()
{
    A = ~A;

    SetSubtractFlag(true);
    SetHalfCarryFlag(true);
}

void CPU::Ccf()
{
    SetSubtractFlag(false);
    SetHalfCarryFlag(false);
    SetCarryFlag(!GetCarryFlag());
}

