#include "CPU.h"

CPU::CPU()
{
    bus = nullptr;

    A  = 0x01;
    F  = 0xB0;
    B  = 0x00;
    C  = 0x13;
    D  = 0x00;
    E  = 0xD8;
    H  = 0x01;
    L  = 0x4D;

    SP = 0xFFFE;
    PC = 0x0100;

    halt = false;
    IME = false;
    InterruptDelayTimer = -1;
    stop = false;
}

void CPU::SetMemoryBus(MemoryBus* bus) 
{
    this->bus = bus;
}

void CPU::Step()
{
    if (bus == nullptr) return;
    if (stop) return;
    opcode = bus->Read(PC);
    PC++;
    Execute();
    HandleInterrupts();
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

        // Flag
        case 0x37: Scf(); break; // SCF
        case 0x2F: Cpl(); break; //CPL
        case 0x3F: Ccf(); break; //CCF

        // SUB r8
        case 0x90: Sub(B); break; // SUB B
        case 0x91: Sub(C); break; // SUB C
        case 0x92: Sub(D); break; // SUB D
        case 0x93: Sub(E); break; // SUB E
        case 0x94: Sub(H); break; // SUB H
        case 0x95: Sub(L); break; // SUB L
        case 0x97: Sub(A); break; // SUB A

        // SBC A,r8
        case 0x98: Sbc(B); break; // SBC B
        case 0x99: Sbc(C); break; // SBC C
        case 0x9A: Sbc(D); break; // SBC D
        case 0x9B: Sbc(E); break; // SBC E
        case 0x9C: Sbc(H); break; // SBC H
        case 0x9D: Sbc(L); break; // SBC L
        case 0x9F: Sbc(A); break; // SBC A

        // AND r8
        case 0xA0: And(B); break; // AND B
        case 0xA1: And(C); break; // AND C
        case 0xA2: And(D); break; // AND D
        case 0xA3: And(E); break; // AND E
        case 0xA4: And(H); break; // AND H
        case 0xA5: And(L); break; // AND L
        case 0xA7: And(A); break; // AND A

        // XOR r8
        case 0xA8: Xor(B); break; // XOR B
        case 0xA9: Xor(C); break; // XOR C
        case 0xAA: Xor(D); break; // XOR D
        case 0xAB: Xor(E); break; // XOR E
        case 0xAC: Xor(H); break; // XOR H
        case 0xAD: Xor(L); break; // XOR L
        case 0xAF: Xor(A); break; // XOR A
        
        // OR r8
        case 0xB0: Or(B); break; // OR B
        case 0xB1: Or(C); break; // OR C
        case 0xB2: Or(D); break; // OR D
        case 0xB3: Or(E); break; // OR E
        case 0xB4: Or(H); break; // OR H
        case 0xB5: Or(L); break; // OR L
        case 0xB7: Or(A); break; // OR A

        // CP r8
        case 0xB8: Cp(B); break; // CP B
        case 0xB9: Cp(C); break; // CP C
        case 0xBA: Cp(D); break; // CP D
        case 0xBB: Cp(E); break; // CP E
        case 0xBC: Cp(H); break; // CP H
        case 0xBD: Cp(L); break; // CP L
        case 0xBF: Cp(A); break; // CP A

        // ALU d8 functions 
        case 0xC6: Add(FetchByte()); break; //ADD A, d8
        case 0xCE: Adc(FetchByte()); break; //ADC A, d8
        case 0xD6: Sub(FetchByte()); break; //SUB A, d8
        case 0xDE: Sbc(FetchByte()); break; //SBC A, d8
        case 0xE6: And(FetchByte()); break; //AND A, d8
        case 0xEE: Xor(FetchByte()); break; //XOR A, d8
        case 0xF6: Or(FetchByte()); break; //OR A, d8
        case 0xFE: Cp(FetchByte()); break; //CP A, d8

        //LD rr, d16
        case 0x01: SetBC(FetchWord()); break; //LD BC, d16
        case 0x11: SetDE(FetchWord()); break; //LD DE, d16
        case 0x21: SetHL(FetchWord()); break; //LD HL, d16
        case 0x31: SetSP(FetchWord()); break; //LD SP, d16

        //INC rr
        case 0x03: SetBC(Inc16(GetBC())); break; //INC BC
        case 0x13: SetDE(Inc16(GetDE())); break; //INC DE
        case 0x23: SetHL(Inc16(GetHL())); break; //INC HL
        case 0x33: SetSP(Inc16(SP)); break; //INC SP
        
        //DEC rr
        case 0x0B: SetBC(Dec16(GetBC())); break; //DEC BC
        case 0x1B: SetDE(Dec16(GetDE())); break; //DEC DE
        case 0x2B: SetHL(Dec16(GetHL())); break; //DEC HL
        case 0x3B: SetSP(Dec16(SP)); break; //DEC SP

        //ADD rr, rr
        case 0x09: SetHL(Add16(GetHL(), GetBC())); break; //ADD HL, BC 
        case 0x19: SetHL(Add16(GetHL(), GetDE())); break; //ADD HL, DE 
        case 0x29: SetHL(Add16(GetHL(), GetHL())); break; //ADD HL, HL 
        case 0x39: SetHL(Add16(GetHL(), SP)); break; //ADD HL, SP 

        //Rotations
        case 0x07: Rlca(); break; //RLCA
        case 0x0F: Rrca(); break; //RRCA
        case 0x17: Rla(); break; //RLA
        case 0x1F: Rra(); break; //RRA

        //JP
        case 0xC2: Jp(!GetZeroFlag()); break; //JP NZ, a16
        case 0xC3: Jp(true); break; //JP a16
        case 0xCA: Jp(GetZeroFlag()); break; // JP Z, a16
        case 0xD2: Jp(!GetCarryFlag()); break; // JP NC, a16
        case 0xDA: Jp(GetCarryFlag()); break; // JP C, a16
        case 0xE9: PC = GetHL(); break; // JP (HL)

        //JR
        case 0x18: Jr(true); break; //JR r8
        case 0x20: Jr(!GetZeroFlag()); break; //JR NZ, r8
        case 0x28: Jr(GetZeroFlag()); break; //JR Z, r8
        case 0x30: Jr(!GetCarryFlag()); break; //JR NC, r8
        case 0x38: Jr(GetCarryFlag()); break; //JR C, r8

        //Interrupt Controls
        case 0xF3: IME = false; break; //DI

        //LD with external memory
        case 0xE0: bus->Write(0xFF00 + FetchByte(), A); break; //LDH (a8), A
        case 0xE2: bus->Write(0xFF00 + C, A); break; //LD (C), A
        case 0xEA: bus->Write(FetchWord(), A); break; //LD (a16), A
        case 0xF0: A = bus->Read(0xFF00 + FetchByte()); break; //LDH A, (a8)
        case 0xF2: A = bus->Read(0xFF00 + C); break; //LD A, (C)
        case 0xFA: A = bus->Read(FetchWord()); break; //LD A, (a16)

        //PUSH
        case 0xC5: PushWord(GetBC()); return; //PUSH BC
        case 0xD5: PushWord(GetDE()); return; //PUSH DE
        case 0xE5: PushWord(GetHL()); return; //PUSH HL
        case 0xF5: PushWord(GetAF()); return; //PUSH AF

        //POP
        case 0xC1: SetBC(PopWord()); return; //POP BC
        case 0xD1: SetDE(PopWord()); return; //POP DE
        case 0xE1: SetHL(PopWord()); return; //POP HL
        case 0xF1: SetAF(PopWord()); return; //POP AF

        //CALL
        case 0xC4: Call(!GetZeroFlag()); return; //CALL NZ, a16
        case 0xCC: Call(GetZeroFlag()); return; //CALL Z, a16
        case 0xCD: Call(true); return; //CALL a16
        case 0xD4: Call(!GetCarryFlag()); return; //CALL NC, a16
        case 0xDC: Call(GetCarryFlag()); return; //CALL C, a16

        //RET
        case 0xC0: Ret(!GetZeroFlag()); return; //RET NZ
        case 0xC8: Ret(GetZeroFlag()); return; //RET Z
        case 0xC9: Ret(true); return; //RET
        case 0xD0: Ret(!GetCarryFlag()); return; //RET NC
        case 0xD8: Ret(GetCarryFlag()); return; //RET C
        case 0xD9: Reti(); break; //RETI

        //LD between A and (HL +/-)
        case 0x22: LdHLA(true); return; //LD (HL+), A
        case 0x2A: LdAHL(true); return; //LD A, (HL+)
        case 0x32: LdHLA(false); return; //LD (HL-), A
        case 0x3A: LdHLA(false); return; //LD A, (HL-)

        //LD (HL), r
        case 0x70: bus->Write(GetHL(), B); break; //LD (HL), B
        case 0x71: bus->Write(GetHL(), C); break; //LD (HL), C
        case 0x72: bus->Write(GetHL(), D); break; //LD (HL), D
        case 0x73: bus->Write(GetHL(), E); break; //LD (HL), E
        case 0x74: bus->Write(GetHL(), H); break; //LD (HL), H
        case 0x75: bus->Write(GetHL(), L); break; //LD (HL), L
        case 0x77: bus->Write(GetHL(), A); break; //LD (HL), A

        //LD r, (HL)
        case 0x46: B = bus->Read(GetHL()); break; //LD B, (HL)
        case 0x4E: C = bus->Read(GetHL()); break; //LD C, (HL)
        case 0x56: D = bus->Read(GetHL()); break; //LD D, (HL)
        case 0x5E: E = bus->Read(GetHL()); break; //LD E, (HL)
        case 0x66: H = bus->Read(GetHL()); break; //LD H, (HL)
        case 0x6E: L = bus->Read(GetHL()); break; //LD L, (HL)
        case 0x7E: A = bus->Read(GetHL()); break; //LD A, (HL)

        //LD (rr), r & LD r, (rr)
        case 0x02: bus->Write(GetBC(), A); break; //LD (BC), A
        case 0x0A: A = bus->Read(GetBC()); break; // LD A, (BC)
        case 0x12: bus->Write(GetDE(), A); break; //LD (DE), A
        case 0x1A: A = bus->Read(GetDE()); break; // LD A, (DE)
        

        //ALU (HL) 
        case 0x86: Add(bus->Read(GetHL())); break; //ADD A, (HL)
        case 0x8E: Adc(bus->Read(GetHL())); break; //ADC A, (HL)
        case 0x96: Sub(bus->Read(GetHL())); break; //SUB (HL)
        case 0x9E: Sbc(bus->Read(GetHL())); break; //SBC A, (HL)
        case 0xA6: And(bus->Read(GetHL())); break; //AND (HL)
        case 0xAE: Xor(bus->Read(GetHL())); break; //XOR (HL)
        case 0xB6: Or(bus->Read(GetHL())); break; //OR (HL)
        case 0xBE: Cp(bus->Read(GetHL())); break; //CP (HL)

        //CB
        case 0xCB: Cb(); break;//CB

        //INC & DEC (HL)
        case 0x34: IncHL(); break; //INC (HL)
        case 0x35: DecHL(); break; //DEC (HL)

        //LD SP/HL, HL/SP+r8
        case 0xF8: LdHLSPr8(); break; //LD HL, SP+r8
        case 0xF9: SP = GetHL(); break; //LD SP, HL

        //DAA
        case 0x27: Daa(); break; //DAA

        //EI
        case 0xFB: InterruptDelayTimer = 1; break; //EI

        //RST
        case 0xC7: Rst(0x0000); break; // RST 00H
        case 0xCF: Rst(0x0008); break; // RST 08H
        case 0xD7: Rst(0x0010); break; // RST 10H
        case 0xDF: Rst(0x0018); break; // RST 18H
        case 0xE7: Rst(0x0020); break; // RST 20H
        case 0xEF: Rst(0x0028); break; // RST 28H
        case 0xF7: Rst(0x0030); break; // RST 30H
        case 0xFF: Rst(0x0038); break; // RST 38H

        //LD (HL), d8
        case 0x36: bus->Write(GetHL(), FetchByte()); break; //LD (HL), d8

        //LD (a16), SP
        case 0x08: WriteWord(FetchWord(), SP); break; //LD (a16), SP

        //ADD SP, r8
        case 0xE8: SP = AddSignedSP(FetchByte()); break; //ADD SP, r8

        //STOP
       case 0x10: Stop(); break; //STOP

        default:
            std::cout << "Unknown opcode: "
                      << std::hex
                      << (int)opcode
                      << "\n"
                      <<"At PC: "
                      << std::hex 
                      << (int)PC
                      << "\n";
            halt = true;
            break;
    }
}

void CPU::Cb() 
{
    uint8_t code = FetchByte();

    uint8_t reg = code & 0x07;


    if (code <= 0x07) Rlc(reg); 
    else if (code <= 0x0F) Rrc(reg);
    else if (code <= 0x17) Rl(reg);
    else if (code <= 0x1F) Rr(reg);
    else if (code <= 0x27) Sla(reg);
    else if (code <= 0x2F) Sra(reg);
    else if (code <= 0x37) Swap(reg);
    else if (code <= 0x3F) Srl(reg);
    else if (code <= 0x47) Bit(0, reg);
    else if (code <= 0x4F) Bit(1, reg);
    else if (code <= 0x57) Bit(2, reg);
    else if (code <= 0x5F) Bit(3, reg);
    else if (code <= 0x67) Bit(4, reg);
    else if (code <= 0x6F) Bit(5, reg);
    else if (code <= 0x77) Bit(6, reg);
    else if (code <= 0x7F) Bit(7, reg);
    else if (code <= 0x87) Res(0, reg);
    else if (code <= 0x8F) Res(1, reg);
    else if (code <= 0x97) Res(2, reg);
    else if (code <= 0x9F) Res(3, reg);
    else if (code <= 0xA7) Res(4, reg);
    else if (code <= 0xAF) Res(5, reg);
    else if (code <= 0xB7) Res(6, reg);
    else if (code <= 0xBF) Res(7, reg);
    else if (code <= 0xC7) Set(0, reg);
    else if (code <= 0xCF) Set(1, reg);
    else if (code <= 0xD7) Set(2, reg);
    else if (code <= 0xDF) Set(3, reg);
    else if (code <= 0xE7) Set(4, reg);
    else if (code <= 0xEF) Set(5, reg);
    else if (code <= 0xF7) Set(6, reg);
    else if (code <= 0xFF) Set(7, reg);
    else {
        std::cout << "Unknown CB opcode: "
                      << std::hex
                      << (int)opcode
                      << "\n"
                      <<"At PC: "
                      << std::hex 
                      << (int)PC
                      << "\n";
            halt = true;
    }

}
uint8_t CPU::ReadCB(uint8_t reg)
{
    switch(reg)
    {
        case 0: return B;
        case 1: return C;
        case 2: return D;
        case 3: return E;
        case 4: return H;
        case 5: return L;
        case 6: return bus->Read(GetHL());
        case 7: return A;

        default: return 0xFF;
    }
}
void CPU::WriteCB(uint8_t reg, uint8_t value)
{
    switch(reg)
    {
        case 0: B = value; break;
        case 1: C = value; break;
        case 2: D = value; break;
        case 3: E = value; break;
        case 4: H = value; break;
        case 5: L = value; break;
        case 6: bus->Write(GetHL(), value); break;
        case 7: A = value; break;
    }
}
void CPU::Rlc(uint8_t reg)
{
    uint8_t value = ReadCB(reg);
    uint8_t carry = value >> 7;
    value = (value << 1) | carry;

    SetZeroFlag(value == 0);
    SetSubtractFlag(false);
    SetHalfCarryFlag(false);
    SetCarryFlag(carry);

    WriteCB(reg, value);
}
void CPU::Rrc(uint8_t reg)
{
    uint8_t value = ReadCB(reg);
    uint8_t carry = (value & 0x01);

    value = (value >> 1) | (carry << 7);

    SetZeroFlag(value == 0);
    SetSubtractFlag(false);
    SetHalfCarryFlag(false);
    SetCarryFlag(carry);

    WriteCB(reg, value);
}
void CPU::Rl(uint8_t reg)
{
    uint8_t value = ReadCB(reg);
    bool oldCarry = GetCarryFlag();
    bool newCarry = value & 0x80;

    value = value << 1;
    if (oldCarry) value = value | 1;

    SetZeroFlag(value == 0);
    SetSubtractFlag(false);
    SetHalfCarryFlag(false);
    SetCarryFlag(newCarry);

    WriteCB(reg, value);
}
void CPU::Rr(uint8_t reg)
{
    uint8_t value = ReadCB(reg);
    bool oldCarry = GetCarryFlag();
    bool newCarry = value & 0x01;

    value = value >> 1;
    if (oldCarry) value = value | 0x80;

    SetZeroFlag(value == 0);
    SetSubtractFlag(false);
    SetHalfCarryFlag(false);
    SetCarryFlag(newCarry);

    WriteCB(reg, value);
}
void CPU::Sla(uint8_t reg)
{
    uint8_t value = ReadCB(reg);
    bool carry = value & 0x80;
    value = value << 1;

    SetZeroFlag(value == 0);
    SetSubtractFlag(false);
    SetHalfCarryFlag(false);
    SetCarryFlag(carry);

    WriteCB(reg, value);
}
void CPU::Sra(uint8_t reg)
{
    uint8_t value = ReadCB(reg);
    bool carry = value & 0x01;
    bool signCarry = value & 0x80;
    value = value >> 1;
    if (signCarry) value = 0x80 | value;

    SetZeroFlag(value == 0);
    SetSubtractFlag(false);
    SetHalfCarryFlag(false);
    SetCarryFlag(carry);

    WriteCB(reg, value);
}
void CPU::Swap(uint8_t reg)
{
    uint8_t value = ReadCB(reg);

    value = (value << 4) | (value >> 4);

    SetZeroFlag(value == 0);
    SetSubtractFlag(false);
    SetHalfCarryFlag(false);
    SetCarryFlag(false);

    WriteCB(reg, value);
}
void CPU::Srl(uint8_t reg)
{
    uint8_t value = ReadCB(reg);
    bool carry = value & 0x01;
    value = value >> 1;

    SetZeroFlag(value == 0);
    SetSubtractFlag(false);
    SetHalfCarryFlag(false);
    SetCarryFlag(carry);

    WriteCB(reg, value);
}
void CPU::Bit(uint8_t bit, uint8_t reg)
{
    uint8_t value = ReadCB(reg);
    bool bitSet = value & (1 << bit);

    SetZeroFlag(!bitSet);
    SetSubtractFlag(false);
    SetHalfCarryFlag(true);
}
void CPU::Res(uint8_t bit, uint8_t reg)
{
    uint8_t value = ReadCB(reg);

    value &= ~(1 << bit);

    WriteCB(reg, value);
}
void CPU::Set(uint8_t bit, uint8_t reg)
{
    uint8_t value = ReadCB(reg);

    value |= (1 << bit);

    WriteCB(reg, value);
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
    SetCarryFlag((og + value + carry) > 0xFF);
}
void CPU::Sub(uint8_t value)
{
    uint8_t og = A;
    A -= value;

    SetZeroFlag(A == 0);
    SetSubtractFlag(true);
    SetHalfCarryFlag((og & 0x0F) < (value & 0x0F));
    SetCarryFlag(og < value);
}
void CPU::Sbc(uint8_t value)
{
    uint8_t og = A;
    uint8_t carry = GetCarryFlag() ? 1 : 0;
    A -= (value + carry);

    SetZeroFlag(A == 0);
    SetSubtractFlag(true);
    SetHalfCarryFlag((og & 0x0F) < ((value & 0x0F) + carry));
    SetCarryFlag(og < (value + carry));
}
void CPU::And(uint8_t value)
{
    A &= value;

    SetZeroFlag(A == 0);
    SetSubtractFlag(false);
    SetHalfCarryFlag(true);
    SetCarryFlag(false);
}
void CPU::Or(uint8_t value)
{
    A |= value;

    SetZeroFlag(A == 0);
    SetSubtractFlag(false);
    SetHalfCarryFlag(false);
    SetCarryFlag(false);
}
void CPU::Xor(uint8_t value)
{
    A ^= value;

    SetZeroFlag(A == 0);
    SetSubtractFlag(false);
    SetHalfCarryFlag(false);
    SetCarryFlag(false);
}
void CPU::Cp(uint8_t value)
{
    uint8_t result = A - value;

    SetZeroFlag(result == 0);
    SetSubtractFlag(true);
    SetHalfCarryFlag((A & 0x0F) < (value & 0x0F));
    SetCarryFlag(A < value);
}



void CPU::Jp(bool condition)
{
    uint16_t value = FetchWord();

    if (condition) PC = value;
}
void CPU::Jr(bool condition)
{
    int8_t offset = static_cast<int8_t>(FetchByte());

    if (condition) PC += offset;
}


void CPU::Scf()
{
    SetCarryFlag(true);
    SetSubtractFlag(false);
    SetHalfCarryFlag(false);
}
void CPU::Cpl()
{
    A = ~A;

    SetSubtractFlag(true);
    SetHalfCarryFlag(true);
}
void CPU::Ccf()
{
    SetSubtractFlag(false);
    SetHalfCarryFlag(false);
    SetCarryFlag(!GetCarryFlag());
}



void CPU::Rlca()
{
    uint8_t carry = (A & 0x80) >> 7;

    A = (A << 1) | carry;

    SetZeroFlag(false);
    SetSubtractFlag(false);
    SetHalfCarryFlag(false);
    SetCarryFlag(carry);
}
void CPU::Rrca()
{
    uint8_t carry = (A & 0x01);

    A = (A >> 1) | (carry << 7);

    SetZeroFlag(false);
    SetSubtractFlag(false);
    SetHalfCarryFlag(false);
    SetCarryFlag(carry);
}
void CPU::Rla()
{
    uint8_t oldCarry = GetCarryFlag();

    uint8_t newCarry = (A & 0x80) >> 7;

    A = (A << 1) | oldCarry;

    SetZeroFlag(false);
    SetSubtractFlag(false);
    SetHalfCarryFlag(false);
    SetCarryFlag(newCarry);
}
void CPU::Rra()
{
    uint8_t oldCarry = GetCarryFlag();

    uint8_t newCarry = A & 0x01;

    A = oldCarry << 7 | (A >> 1);

    SetZeroFlag(false);
    SetSubtractFlag(false);
    SetHalfCarryFlag(false);
    SetCarryFlag(newCarry);
}

void CPU::PushWord(uint16_t word)
{
    SP--;
    bus->Write(SP, word >> 8);

    SP--;
    bus->Write(SP, word & 0xFF);

}
uint16_t CPU::PopWord()
{
    uint8_t low = bus->Read(SP);
    SP++;

    uint8_t high = bus->Read(SP);
    SP++;

    return ((high << 8) | low);
}
void CPU::WriteWord(uint16_t address, uint16_t value)
{
    bus->Write(address, value & 0xFF);
    bus->Write(address + 1, value >> 8);
}
void CPU::Call(bool condition)
{
    uint16_t word = FetchWord();

    if (condition)
    {
        PushWord(PC);
        PC = word;
    }
}
void CPU::Ret(bool condition)
{
    if (condition) PC = PopWord();
}
void CPU::Reti()
{
    Ret(true);
    IME = true;
}
void CPU::Rst(uint16_t address)
{
    PushWord(PC);
    PC = address;
}

uint16_t CPU::GetBC()
{
    return (B << 8) | C;
}
void CPU::SetBC(uint16_t value)
{
    B = (value >> 8);
    C = (value & 0x00FF);
}
uint16_t CPU::GetDE()
{
    return (D << 8) | E;
}
void CPU::SetDE(uint16_t value)
{
    D = (value >> 8);
    E = (value & 0x00FF);
}
uint16_t CPU::GetHL()
{
    return (H << 8) | L;
}
void CPU::SetHL(uint16_t value)
{
    H = (value >> 8);
    L = (value & 0x00FF);
}
void CPU::SetSP(uint16_t value)
{
    SP = value;
}
uint16_t CPU::GetAF()
{
    return (A << 8) | F;
}
void CPU::SetAF(uint16_t value)
{
    A = (value >> 8);
    F = (value & 0x00F0);
}

uint16_t CPU::Inc16(uint16_t r)
{
    r++;
    return r;
}
uint16_t CPU::Dec16(uint16_t r)
{
    r--;
    return r;
}
uint16_t CPU::Add16(uint16_t og, uint16_t value)
{
    
    SetSubtractFlag(false);
    SetHalfCarryFlag(((og & 0x0FFF) + (value & 0x0FFF)) > 0x0FFF);
    SetCarryFlag((og + value) > 0xFFFF);
    return (og + value);
}
void CPU::LdAHL(bool increment)
{
    
    A = bus->Read(GetHL());
    
    if (increment) SetHL(GetHL() + 1);
    else SetHL(GetHL() - 1);
}
void CPU::LdHLA(bool increment)
{
    bus->Write(GetHL(), A);

    if (increment) SetHL(GetHL() + 1);
    else SetHL(GetHL() - 1);
}
void CPU::IncHL()
{
    uint8_t value = bus->Read(GetHL());
    bool halfCarry = (value & 0x0F) == 0x0F;
    value++; 
    bus->Write(GetHL(), value);

    SetZeroFlag(value == 0);
    SetSubtractFlag(false);
    SetHalfCarryFlag(halfCarry);

}
void CPU::DecHL()
{
    uint8_t value = bus->Read(GetHL());
    bool halfCarry = (value & 0x0F) == 0x00;
    value--; 
    bus->Write(GetHL(), value);

    SetZeroFlag(value == 0);
    SetSubtractFlag(true);
    SetHalfCarryFlag(halfCarry);

}
void CPU::LdHLSPr8()
{
    uint8_t rawOffset = FetchByte();
    int8_t offset = static_cast<int8_t>(rawOffset);
    uint16_t result = SP + offset;
    SetHL(result);

    SetZeroFlag(false);
    SetSubtractFlag(false);
    SetHalfCarryFlag(((SP & 0x0F) + (rawOffset & 0x0F)) > 0x0F);
    SetCarryFlag(((SP & 0xFF) + rawOffset) > 0xFF);

}

void CPU::Daa()
{
    uint8_t correction = 0;
    bool carry = false;

    if (!GetSubtractFlag()) // Addition
    {
        if (GetHalfCarryFlag() || (A & 0x0F) > 9)
            correction |= 0x06;

        if (GetCarryFlag() || A > 0x99)
        {
            correction |= 0x60;
            carry = true;
        }

        A += correction;
    }
    else // Subtraction
    {
        if (GetHalfCarryFlag())
            correction |= 0x06;

        if (GetCarryFlag())
            correction |= 0x60;

        A -= correction;
    }

    SetZeroFlag(A == 0);
    SetHalfCarryFlag(false);

    if (carry)
        SetCarryFlag(true);
}

uint16_t CPU::AddSignedSP(uint8_t rawOffset)
{
    int8_t offset = static_cast<int8_t>(rawOffset);

    SetZeroFlag(false);
    SetSubtractFlag(false);
    SetHalfCarryFlag(((SP & 0x0F) + (rawOffset & 0x0F)) > 0x0F);
    SetCarryFlag(((SP & 0xFF) + rawOffset) > 0xFF);

    return SP + offset;
}


bool CPU::IsHalted()
{
    return halt;
}
uint8_t CPU::FetchByte()
{
    uint8_t value = bus->Read(PC);
    PC++;
    return value;
}
uint16_t CPU::FetchWord()
{
    uint8_t low = bus->Read(PC);
    PC++;

    uint8_t high = bus->Read(PC);
    PC++;

    return low | (high << 8);
}



void CPU::SetZeroFlag(bool b) 
{
    if (b == true) F |= 0x80;
    else F &= ~0x80;
}
bool CPU::GetZeroFlag()
{
    return (F & 0x80);
}
void CPU::SetSubtractFlag(bool b)
{
    if (b == true) F |= 0x40;
    else F &= ~0x40;
}
bool CPU::GetSubtractFlag()
{
    return (F & 0x40);
}
void CPU::SetHalfCarryFlag(bool b)
{
    if (b == true) F |= 0x20;
    else F &= ~0x20;
}
bool CPU::GetHalfCarryFlag()
{
    return (F & 0x20);
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

void CPU::HandleInterrupts()
{
    if (InterruptDelayTimer == -1) return;
    if (InterruptDelayTimer == 0) 
    {
        IME = true;
        InterruptDelayTimer = -1;
    }
    else InterruptDelayTimer--;
}
void CPU::Stop()
{
    FetchByte(); // STOP is followed by a 0x00 byte
    stop = true;
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