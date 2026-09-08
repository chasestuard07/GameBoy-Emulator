#include "CPU.h"

void CPU::LDrd8(uint8_t& r) 
{
    r = bus->Read(PC);
    PC++;
}

void CPU::LDrtr(uint8_t& r1 ,uint8_t& r2)
{
    r1 = r2;
}

void CPU::LdAHL(bool increment)
{
    A = bus->Read(GetHL());
    
    if (increment) SetHL(GetHL() + 1);
    else SetHL(GetHL() - 1);
}

void CPU::LdHLA(bool increment)
{
    bus->Write(GetHL(), A);

    if (increment) SetHL(GetHL() + 1);
    else SetHL(GetHL() - 1);
}

void CPU::LdHLSPr8()
{
    uint8_t rawOffset = FetchByte();
    int8_t offset = static_cast<int8_t>(rawOffset);
    uint16_t result = SP + offset;
    SetHL(result);

    SetZeroFlag(false);
    SetSubtractFlag(false);
    SetHalfCarryFlag(((SP & 0x0F) + (rawOffset & 0x0F)) > 0x0F);
    SetCarryFlag(((SP & 0xFF) + rawOffset) > 0xFF);
}