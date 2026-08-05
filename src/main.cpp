#include <iostream>
#include "Emulator.h"

#include <filesystem>

int main()
{
    Emulator emulator("../tests/02-interrupts.gb");
    emulator.Run();
    return 0;
}