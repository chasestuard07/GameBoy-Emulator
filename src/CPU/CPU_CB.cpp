# include "CPU.h"

int CPU::Cb() 
{
    uint8_t code = FetchByte();
    uint8_t reg = code & 0x07;

    if (code <= 0x07) Rlc(reg); 
    else if (code <= 0x0F) Rrc(reg);
    else if (code <= 0x17) Rl(reg);
    else if (code <= 0x1F) Rr(reg);
    else if (code <= 0x27) Sla(reg);
    else if (code <= 0x2F) Sra(reg);
    else if (code <= 0x37) Swap(reg);
    else if (code <= 0x3F) Srl(reg);
    else if (code <= 0x47) Bit(0, reg);
    else if (code <= 0x4F) Bit(1, reg);
    else if (code <= 0x57) Bit(2, reg);
    else if (code <= 0x5F) Bit(3, reg);
    else if (code <= 0x67) Bit(4, reg);
    else if (code <= 0x6F) Bit(5, reg);
    else if (code <= 0x77) Bit(6, reg);
    else if (code <= 0x7F) Bit(7, reg);
    else if (code <= 0x87) Res(0, reg);
    else if (code <= 0x8F) Res(1, reg);
    else if (code <= 0x97) Res(2, reg);
    else if (code <= 0x9F) Res(3, reg);
    else if (code <= 0xA7) Res(4, reg);
    else if (code <= 0xAF) Res(5, reg);
    else if (code <= 0xB7) Res(6, reg);
    else if (code <= 0xBF) Res(7, reg);
    else if (code <= 0xC7) Set(0, reg);
    else if (code <= 0xCF) Set(1, reg);
    else if (code <= 0xD7) Set(2, reg);
    else if (code <= 0xDF) Set(3, reg);
    else if (code <= 0xE7) Set(4, reg);
    else if (code <= 0xEF) Set(5, reg);
    else if (code <= 0xF7) Set(6, reg);
    else if (code <= 0xFF) Set(7, reg);

    bool isHL = (reg == 6);
    // BIT (HL) is special
    if (isHL && code >= 0x40 && code <= 0x7F)
        return 12;

    // All other (HL) operations
    if (isHL)
        return 16;

    return 8;
}

uint8_t CPU::ReadCB(uint8_t reg)
{
    switch(reg)
    {
        case 0: return B;
        case 1: return C;
        case 2: return D;
        case 3: return E;
        case 4: return H;
        case 5: return L;
        case 6: return bus->Read(GetHL());
        case 7: return A;

        default: return 0xFF;
    }
}

void CPU::WriteCB(uint8_t reg, uint8_t value)
{
    switch(reg)
    {
        case 0: B = value; break;
        case 1: C = value; break;
        case 2: D = value; break;
        case 3: E = value; break;
        case 4: H = value; break;
        case 5: L = value; break;
        case 6: bus->Write(GetHL(), value); break;
        case 7: A = value; break;
    }
}

void CPU::Rlc(uint8_t reg)
{
    uint8_t value = ReadCB(reg);
    uint8_t carry = value >> 7;
    value = (value << 1) | carry;

    SetZeroFlag(value == 0);
    SetSubtractFlag(false);
    SetHalfCarryFlag(false);
    SetCarryFlag(carry);

    WriteCB(reg, value);
}

void CPU::Rrc(uint8_t reg)
{
    uint8_t value = ReadCB(reg);
    uint8_t carry = (value & 0x01);

    value = (value >> 1) | (carry << 7);

    SetZeroFlag(value == 0);
    SetSubtractFlag(false);
    SetHalfCarryFlag(false);
    SetCarryFlag(carry);

    WriteCB(reg, value);
}

void CPU::Rl(uint8_t reg)
{
    uint8_t value = ReadCB(reg);
    bool oldCarry = GetCarryFlag();
    bool newCarry = value & 0x80;

    value = value << 1;
    if (oldCarry) value = value | 1;

    SetZeroFlag(value == 0);
    SetSubtractFlag(false);
    SetHalfCarryFlag(false);
    SetCarryFlag(newCarry);

    WriteCB(reg, value);
}

void CPU::Rr(uint8_t reg)
{
    uint8_t value = ReadCB(reg);
    bool oldCarry = GetCarryFlag();
    bool newCarry = value & 0x01;

    value = value >> 1;
    if (oldCarry) value = value | 0x80;

    SetZeroFlag(value == 0);
    SetSubtractFlag(false);
    SetHalfCarryFlag(false);
    SetCarryFlag(newCarry);

    WriteCB(reg, value);
}

void CPU::Sla(uint8_t reg)
{
    uint8_t value = ReadCB(reg);
    bool carry = value & 0x80;
    value = value << 1;

    SetZeroFlag(value == 0);
    SetSubtractFlag(false);
    SetHalfCarryFlag(false);
    SetCarryFlag(carry);

    WriteCB(reg, value);
}

void CPU::Sra(uint8_t reg)
{
    uint8_t value = ReadCB(reg);
    bool carry = value & 0x01;
    bool signCarry = value & 0x80;
    value = value >> 1;
    if (signCarry) value = 0x80 | value;

    SetZeroFlag(value == 0);
    SetSubtractFlag(false);
    SetHalfCarryFlag(false);
    SetCarryFlag(carry);

    WriteCB(reg, value);
}

void CPU::Swap(uint8_t reg)
{
    uint8_t value = ReadCB(reg);

    value = (value << 4) | (value >> 4);

    SetZeroFlag(value == 0);
    SetSubtractFlag(false);
    SetHalfCarryFlag(false);
    SetCarryFlag(false);

    WriteCB(reg, value);
}

void CPU::Srl(uint8_t reg)
{
    uint8_t value = ReadCB(reg);
    bool carry = value & 0x01;
    value = value >> 1;

    SetZeroFlag(value == 0);
    SetSubtractFlag(false);
    SetHalfCarryFlag(false);
    SetCarryFlag(carry);

    WriteCB(reg, value);
}

void CPU::Bit(uint8_t bit, uint8_t reg)
{
    uint8_t value = ReadCB(reg);
    bool bitSet = value & (1 << bit);

    SetZeroFlag(!bitSet);
    SetSubtractFlag(false);
    SetHalfCarryFlag(true);
}

void CPU::Res(uint8_t bit, uint8_t reg)
{
    uint8_t value = ReadCB(reg);

    value &= ~(1 << bit);

    WriteCB(reg, value);
}

void CPU::Set(uint8_t bit, uint8_t reg)
{
    uint8_t value = ReadCB(reg);

    value |= (1 << bit);

    WriteCB(reg, value);
}