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

    halt = false;
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

        //LD r, d8
        case 0x06: LDrd8(B); break; // LD B, d8
        case 0x0E: LDrd8(C); break; // LD C, d8
        case 0x16: LDrd8(D); break; // LD D, d8
        case 0x1E: LDrd8(E); break; // LD E, d8
        case 0x26: LDrd8(H); break; // LD H, d8
        case 0x2E: LDrd8(L); break; // LD L, d8
        case 0x3E: LDrd8(A); break; // LD A, d8

        //LD r, r
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

        //INC r8
        case 0x3C: Inc(A); break; //INC A
        case 0x04: Inc(B); break; //INC B
        case 0x0C: Inc(C); break; //INC C
        case 0x14: Inc(D); break; //INC D
        case 0x1C: Inc(E); break; //INC E
        case 0x24: Inc(H); break; //INC H
        case 0x2C: Inc(L); break; //INC L

        //DEC r8
        case 0x3D: Dec(A); break; //DEC A
        case 0x05: Dec(B); break; //DEC B
        case 0x0D: Dec(C); break; //DEC C
        case 0x15: Dec(D); break; //DEC D
        case 0x1D: Dec(E); break; //DEC E
        case 0x25: Dec(H); break; //DEC H
        case 0x2D: Dec(L); break; //DEC L

        //HALT
        case 0x76: halt = true; break; //HALT

        //ADD r8
        case 0x80: Add(B); break; //ADD A,B
        case 0x81: Add(C); break; //ADD A,C
        case 0x82: Add(D); break; //ADD A,D
        case 0x83: Add(E); break; //ADD A,E
        case 0x84: Add(H); break; //ADD A,H
        case 0x85: Add(L); break; //ADD A,L
        case 0x87: Add(A); break; //ADD A,A

        // ADC A,r8
        case 0x88: Adc(B); break; // ADC A,B
        case 0x89: Adc(C); break; // ADC A,C
        case 0x8A: Adc(D); break; // ADC A,D
        case 0x8B: Adc(E); break; // ADC A,E
        case 0x8C: Adc(H); break; // ADC A,H
        case 0x8D: Adc(L); break; // ADC A,L
        case 0x8F: Adc(A); break; // ADC A,A

        // SCF
        case 0x37: Scf(); break; // SCF


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
void CPU::Inc(uint8_t& r)
{
    uint8_t og = r;

    r += 1;

    SetZeroFlag(r == 0);
    SetSubtractFlag(false);
    SetHalfCarryFlag((og & 0x0F) == 0x0F);
}
void CPU::Dec(uint8_t& r)
{
    uint8_t og = r;

    r -= 1;

    SetZeroFlag(r == 0);
    SetSubtractFlag(true);
    SetHalfCarryFlag((og & 0x0F) == 0x00);
}
void CPU::Add(uint8_t value)
{
    uint8_t og = A;
    A += value;

    SetZeroFlag(A == 0);
    SetSubtractFlag(false);
    SetHalfCarryFlag((og & 0x0F) + (value & 0x0F) > 0x0F);
    SetCarryFlag((og + value) > 0xFF);
}
void CPU::Adc(uint8_t value)
{
    uint8_t og = A;
    uint8_t carry = GetCarryFlag() ? 1 : 0;
    A += value + carry;


    SetZeroFlag(A == 0);
    SetSubtractFlag(false);
    SetHalfCarryFlag(((og & 0x0F) + (value & 0x0F) + carry) > 0x0F);
    SetCarryFlag(((og + value) + carry) > 0xFF);
}
void CPU::Scf()
{
    SetCarryFlag(true);
    SetSubtractFlag(false);
    SetHalfCarryFlag(false);
}



bool CPU::IsHalted()
{
    return halt;
}


void CPU::SetZeroFlag(bool b) 
{
    if (b == true) F |= 0x80;
    else F &= ~0x80;
}
void CPU::SetSubtractFlag(bool b)
{
    if (b == true) F |= 0x40;
    else F &= ~0x40;
}
void CPU::SetHalfCarryFlag(bool b)
{
    if (b == true) F |= 0x20;
    else F &= ~0x20;
}
void CPU::SetCarryFlag(bool b)
{
    if (b == true) F |= 0x10;
    else F &= ~0x10;
}
bool CPU::GetCarryFlag()
{
    return (F & 0x10);
}



void CPU::Debug()
{
    std::cout << "A: " << std::hex << (int)A << "  ";
    std::cout << "B: " << std::hex << (int)B << "  ";
    std::cout << "C: " << std::hex << (int)C << "  ";
    std::cout << "D: " << std::hex << (int)D << "  ";
    std::cout << "E: " << std::hex << (int)E << "  ";
    std::cout << "F: " << std::hex << (int)F << "  ";
    std::cout << "H: " << std::hex << (int)H << "  ";
    std::cout << "L: " << std::hex << (int)L << "  ";
    std::cout << "PC: " << std::hex << (int)PC << "  ";
    std::cout << "SP: " << std::hex << (int)SP << "\n";
}