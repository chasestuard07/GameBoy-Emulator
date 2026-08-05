#include "Timer.h"

Timer::Timer()
{
    dividerCounter = 0;
    timerCounter = 0;

    TIMA = 0;
    TMA = 0;
    TAC = 0;

    timerInterrupt = false;
}

void Timer::Tick(int cycles)
{
    dividerCounter += cycles;

    if(!(TAC & 0x04)) return;

    timerCounter += cycles;

    int threshold = GetClockFrequency();

    while(timerCounter >= threshold)
    {
        timerCounter -= threshold;
        TIMA++;

        if (TIMA == 0) 
        {
            TIMA = TMA;
            timerInterrupt = true;
        }
    }
}

uint8_t Timer::Read(uint16_t address)
{
    switch(address)
    {
        case 0xFF04: return dividerCounter >> 8; //DIV
        case 0xFF05: return TIMA; //TIMA
        case 0xFF06: return TMA; //TMA
        case 0xFF07: return TAC; //TAC
    }

    return 0xFF;
}

void Timer::Write(uint16_t address, uint8_t value)
{
    switch(address)
    {
        case 0xFF04: dividerCounter = 0; return; //DIV
        case 0xFF05: TIMA = value; return; //TIMA
        case 0xFF06: TMA = value; return; //TMA
        case 0xFF07: TAC = value & 0x07; return; //TAC
    }
}

int Timer::GetClockFrequency()
{
    switch(TAC & 0x03)
    {
        case 0: return 1024;
        case 1: return 16;
        case 2: return 64;
        case 3: return 256;
    }
    return 1024;
}

bool Timer::GetInterrupt()
{
    return timerInterrupt;
}

void Timer::ClearInterrupt()
{
    timerInterrupt = false;
}