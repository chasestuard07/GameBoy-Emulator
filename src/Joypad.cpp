#include "Joypad.h"

Joypad::Joypad()
{
    // Bits 4-5 are selection lines
    // Lower 4 bits represent buttons
    joypadState = 0xFF;
}

uint8_t Joypad::Read()
{
    return joypadState;
}

void Joypad::Write(uint8_t value)
{
    joypadState = (joypadState & 0x0F) | (value & 0x30);
}