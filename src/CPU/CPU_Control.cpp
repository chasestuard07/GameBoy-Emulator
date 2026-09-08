#include "CPU.h"

void CPU::PushWord(uint16_t word)
{
    SP--;
    bus->Write(SP, word >> 8);

    SP--;
    bus->Write(SP, word & 0xFF);

}

uint16_t CPU::PopWord()
{
    uint8_t low = bus->Read(SP);
    SP++;

    uint8_t high = bus->Read(SP);
    SP++;

    return ((high << 8) | low);
}

bool CPU::Call(bool condition)
{
    uint16_t word = FetchWord();

    if (condition)
    {
        PushWord(PC);
        PC = word;
        return true;
    }
    return false;
}

bool CPU::Ret(bool condition)
{
    if (condition)
    {
        uint16_t returnAddress = PopWord();
        PC = returnAddress;
        return true;
    }

    return false;
}

void CPU::Reti()
{
    Ret(true);
    IME = true;
}

void CPU::Rst(uint16_t address)
{
    PushWord(PC);
    PC = address;
}

bool CPU::Jp(bool condition)
{
    uint16_t value = FetchWord();

    if (condition) 
    {
        PC = value; 
        return true;
    }
    return false;
}

bool CPU::Jr(bool condition)
{
    int8_t offset = static_cast<int8_t>(FetchByte());
    
    if (condition) 
    {
        PC += offset; 
        return true;
    }
    return false;
}

void CPU::Stop()
{
    FetchByte(); // STOP is followed by a 0x00 byte
    stop = true;
}

