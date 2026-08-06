#include "Joypad.h"

#include <iostream>




Joypad::Joypad()
{
    buttons = 0x0F;
    directions = 0x0F;
    select = 0x30;
}

uint8_t Joypad::Read()
{
    uint8_t result = 0xCF;

    if(!(select & 0x10))
    {
        result &= directions;
    }

    if(!(select & 0x20))
    {
        result &= buttons;
    }

    result |= select;

    return result;
}

void Joypad::Write(uint8_t value)
{
    select = value & 0x30;
}

void Joypad::Press(Button button)
{
    switch(button)
    {
        case RIGHT:
            directions &= ~(1 << 0);
            break;

        case LEFT:
            directions &= ~(1 << 1);
            break;

        case UP:
            directions &= ~(1 << 2);
            break;

        case DOWN:
            directions &= ~(1 << 3);
            break;

        case A:
            buttons &= ~(1 << 0);
            break;

        case B:
            buttons &= ~(1 << 1);
            break;

        case SELECT:
            buttons &= ~(1 << 2);
            break;

        case START:
            buttons &= ~(1 << 3);
            break;
    }
}

void Joypad::Release(Button button)
{
    switch(button)
    {
        case RIGHT:
            directions |= (1 << 0);
            break;

        case LEFT:
            directions |= (1 << 1);
            break;

        case UP:
            directions |= (1 << 2);
            break;

        case DOWN:
            directions |= (1 << 3);
            break;

        case A:
            buttons |= (1 << 0);
            break;

        case B:
            buttons |= (1 << 1);
            break;

        case SELECT:
            buttons |= (1 << 2);
            break;

        case START:
            buttons |= (1 << 3);
            break;
    }
}