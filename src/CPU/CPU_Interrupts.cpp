#include "CPU.h"

void CPU::Di()
{
    IME = false;
    InterruptDelayTimer = -1;
}

void CPU::HandleInterrupts()
{
    uint8_t pending = bus->ie & bus->If;

    if (!IME) return;

    if (pending == 0) return; 

    if (pending & 0x01)
    {
        bus->If &= ~0x01;
        IME = false;
        halt = false;

        PushWord(PC);
        PC = 0x40;
        return;
    }
    if (pending & 0x02)
    {
        bus->If &= ~0x02;
        IME = false;
        halt = false;

        PushWord(PC);
        PC = 0x48;
        return;
    }
    if (pending & 0x04)
    {
        bus->If &= ~0x04;
        IME = false;
        halt = false;
        PushWord(PC);
        PC = 0x50;
        return;
    }
    if (pending & 0x08)
    {
        bus->If &= ~0x08;
        IME = false;
        halt = false;

        PushWord(PC);
        PC = 0x58;
        return;
    }
    if (pending & 0x10)
    {
        bus->If &= ~0x10;
        IME = false;
        halt = false;

        PushWord(PC);
        PC = 0x60;
        return;
    }
}

void CPU::HandleIE()
{
    if (InterruptDelayTimer == -1) return;

    InterruptDelayTimer--;
    
    if (InterruptDelayTimer == 0) 
    {
        IME = true;
        InterruptDelayTimer = -1;
    }
}
