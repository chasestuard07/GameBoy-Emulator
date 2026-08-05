#pragma once
#include <cstdint>

class Joypad
{
public:
    Joypad();

    uint8_t Read();
    void Write(uint8_t value);

private:
    uint8_t joypadState;
};