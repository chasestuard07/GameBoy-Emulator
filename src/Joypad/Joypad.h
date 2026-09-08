#pragma once
#include <cstdint>

enum Button
    {
        RIGHT,
        LEFT,
        UP,
        DOWN,
        A,
        B,
        SELECT,
        START
    };

class Joypad
{
public:
    Joypad();

    uint8_t Read();
    void Write(uint8_t value);

    void Press(Button);
    void Release(Button);

private:
    uint8_t buttons;
    uint8_t directions;
    uint8_t select;
};