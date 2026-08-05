#pragma once

#include <cstdint>


class Timer
{
public:
    Timer();

    void Tick(int cycles);
    uint8_t Read(uint16_t address);
    void Write(uint16_t address, uint8_t value);

    bool GetInterrupt();
    void ClearInterrupt();

private:
    uint16_t dividerCounter;
    uint16_t timerCounter;

    uint8_t TIMA;
    uint8_t TMA;
    uint8_t TAC;

    bool timerInterrupt;

    int GetClockFrequency();


};