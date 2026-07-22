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
    if (bus == nullptr) 
    {
        return;
    }
    opcode = bus->Read(PC);
    PC++;
    Execute();
}

void CPU::Execute()
{
    switch(opcode)
    {
        //NOP & Functionally NOP instructions
        case 0x00: // NOP
        case 0x40: //LD B, B
        case 0x49: //LD C, C
        case 0x52: //LD D, D
        case 0x5B: //LD E, E
        case 0x64: //LD H, H
        case 0x6D: //LD L, L
        case 0x7F: //LD A, A
            break;

        case 0x06: LDrd8(B); break; // LD B, d8
        case 0x0E: LDrd8(C); break; // LD C, d8
        case 0x16: LDrd8(D); break; // LD D, d8
        case 0x1E: LDrd8(E); break; // LD E, d8
        case 0x26: LDrd8(H); break; // LD H, d8
        case 0x2E: LDrd8(L); break; // LD L, d8
        case 0x3E: LDrd8(A); break; // LD A, d8

        
        case 0x41: LDrtr(B,C); break; //LD B, C
        case 0x42: LDrtr(B,D); break; //LD B, D
        case 0x43: LDrtr(B,E); break; //LD B, E
        case 0x44: LDrtr(B,H); break; //LD B, H
        case 0x45: LDrtr(B,L); break; //LD B, L
        case 0x47: LDrtr(B,A); break; //LD B, A

        case 0x48: LDrtr(C,B); break; //LD C, B
        case 0x4A: LDrtr(C,D); break; //LD C, D
        case 0x4B: LDrtr(C,E); break; //LD C, E
        case 0x4C: LDrtr(C,H); break; //LD C, H
        case 0x4D: LDrtr(C,L); break; //LD C, L
        case 0x4F: LDrtr(C,A); break; //LD C, A
        
        case 0x50: LDrtr(D,B); break; //LD D, B
        case 0x51: LDrtr(D,C); break; //LD D, C
        case 0x53: LDrtr(D,E); break; //LD D, E
        case 0x54: LDrtr(D,H); break; //LD D, H
        case 0x55: LDrtr(D,L); break; //LD D, L
        case 0x57: LDrtr(D,A); break; //LD D, A

        case 0x58: LDrtr(E,B); break; //LD E, B
        case 0x59: LDrtr(E,C); break; //LD E, C
        case 0x5A: LDrtr(E,D); break; //LD E, D
        case 0x5C: LDrtr(E,H); break; //LD E, H
        case 0x5D: LDrtr(E,L); break; //LD E, L
        case 0x5F: LDrtr(E,A); break; //LD E, A

        case 0x60: LDrtr(H,B); break; //LD H, B
        case 0x61: LDrtr(H,C); break; //LD H, C
        case 0x62: LDrtr(H,D); break; //LD H, D
        case 0x63: LDrtr(H,E); break; //LD H, E
        case 0x65: LDrtr(H,L); break; //LD H, L
        case 0x67: LDrtr(H,A); break; //LD H, A

        case 0x68: LDrtr(L,B); break; //LD L, B
        case 0x69: LDrtr(L,C); break; //LD L, C
        case 0x6A: LDrtr(L,D); break; //LD L, D
        case 0x6B: LDrtr(L,E); break; //LD L, E
        case 0x6C: LDrtr(L,H); break; //LD L, H
        case 0x6F: LDrtr(L,A); break; //LD L, A

        case 0x78: LDrtr(A,B); break; //LD A, B
        case 0x79: LDrtr(A,C); break; //LD A, C
        case 0x7A: LDrtr(A,D); break; //LD A, D
        case 0x7B: LDrtr(A,E); break; //LD A, E
        case 0x7C: LDrtr(A,H); break; //LD A, H
        case 0x7D: LDrtr(A,L); break; //LD A, L

        



        default:
            std::cout << "Unknown opcode: "
                      << std::hex
                      << (int)opcode
                      << "\n";
            break;
    }
}



void CPU::LDrd8(uint8_t& r) 
{
    r = bus->Read(PC);
    PC++;
}
void CPU::LDrtr(uint8_t& r1 ,uint8_t& r2)
{
    r1 = r2;
}

void CPU::Debug()
{
    std::cout << "A: " << std::hex << (int)A << "\n";
    std::cout << "B: " << std::hex << (int)B << "\n";
    std::cout << "C: " << std::hex << (int)C << "\n";
    std::cout << "D: " << std::hex << (int)D << "\n";
    std::cout << "E: " << std::hex << (int)E << "\n";
    std::cout << "F: " << std::hex << (int)F << "\n";
    std::cout << "H: " << std::hex << (int)H << "\n";
    std::cout << "L: " << std::hex << (int)L << "\n";
    std::cout << "PC: " << std::hex << (int)PC << "\n";
    std::cout << "SP: " << std::hex << (int)SP << "\n";
}