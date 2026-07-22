#include "CPU.h"

CPU::CPU()
{
    bus = nullptr;

    A = 0;
    B = 0;
    C = 0;
    D = 0;
    E = 0;
    F = 0;
    H = 0;
    L = 0;

    PC = 0;
    SP = 0;
}

void CPU::SetMemoryBus(MemoryBus* bus) 
{
    this->bus = bus;
}

void CPU::Step()
{
    opcode = bus->Read(PC);

    PC++;

    Execute();
}

void CPU::Execute()
{
    switch(opcode)
    {
        case 0x00:
            // NOP
            break;
        case 0x3E:
            // LD A, d8
            A = bus->Read(PC);
            PC++;

            std::cout << "A: "
                      << std::hex
                      << (int)A
                      << "\n";
            break;

        default:
            std::cout << "Unknown opcode: "
                      << std::hex
                      << (int)opcode
                      << "\n";
            break;
    }
}