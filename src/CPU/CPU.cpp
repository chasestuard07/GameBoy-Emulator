#include "CPU/CPU.h"

CPU::CPU()
{
    bus = nullptr;

    A  = 0x01;
    F  = 0xB0;
    B  = 0;
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
    debugMode = false;
}

void CPU::SetMemoryBus(MemoryBus* bus) 
{
    this->bus = bus;
}

int CPU::Step()
{
    if (bus == nullptr) return 0;
    if (halt)
    {
        // HALT stops instruction execution, but hardware continues ticking.
        if (bus->If & bus->ie)
        {
            halt = false;
        }
        else
        {
            return 4;
        }
    }
    if (stop) return 0;
    if(debugMode) Debug();
    opcode = bus->Read(PC);
    PC++;
    int cycles = Execute();
    HandleIE();
    HandleInterrupts();
    return cycles;
}

int CPU::Execute()
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
            return 4;

        //LD r, d8
        case 0x06: LDrd8(B); return 8; // LD B, d8
        case 0x0E: LDrd8(C); return 8; // LD C, d8
        case 0x16: LDrd8(D); return 8; // LD D, d8
        case 0x1E: LDrd8(E); return 8; // LD E, d8
        case 0x26: LDrd8(H); return 8; // LD H, d8
        case 0x2E: LDrd8(L); return 8; // LD L, d8
        case 0x3E: LDrd8(A); return 8; // LD A, d8

        //LD r, r
        case 0x41: LDrtr(B,C); return 4; //LD B, C
        case 0x42: LDrtr(B,D); return 4; //LD B, D
        case 0x43: LDrtr(B,E); return 4; //LD B, E
        case 0x44: LDrtr(B,H); return 4; //LD B, H
        case 0x45: LDrtr(B,L); return 4; //LD B, L
        case 0x47: LDrtr(B,A); return 4; //LD B, A
        case 0x48: LDrtr(C,B); return 4; //LD C, B
        case 0x4A: LDrtr(C,D); return 4; //LD C, D
        case 0x4B: LDrtr(C,E); return 4; //LD C, E
        case 0x4C: LDrtr(C,H); return 4; //LD C, H
        case 0x4D: LDrtr(C,L); return 4; //LD C, L
        case 0x4F: LDrtr(C,A); return 4; //LD C, A
        case 0x50: LDrtr(D,B); return 4; //LD D, B
        case 0x51: LDrtr(D,C); return 4; //LD D, C
        case 0x53: LDrtr(D,E); return 4; //LD D, E
        case 0x54: LDrtr(D,H); return 4; //LD D, H
        case 0x55: LDrtr(D,L); return 4; //LD D, L
        case 0x57: LDrtr(D,A); return 4; //LD D, A
        case 0x58: LDrtr(E,B); return 4; //LD E, B
        case 0x59: LDrtr(E,C); return 4; //LD E, C
        case 0x5A: LDrtr(E,D); return 4; //LD E, D
        case 0x5C: LDrtr(E,H); return 4; //LD E, H
        case 0x5D: LDrtr(E,L); return 4; //LD E, L
        case 0x5F: LDrtr(E,A); return 4; //LD E, A
        case 0x60: LDrtr(H,B); return 4; //LD H, B
        case 0x61: LDrtr(H,C); return 4; //LD H, C
        case 0x62: LDrtr(H,D); return 4; //LD H, D
        case 0x63: LDrtr(H,E); return 4; //LD H, E
        case 0x65: LDrtr(H,L); return 4; //LD H, L
        case 0x67: LDrtr(H,A); return 4; //LD H, A
        case 0x68: LDrtr(L,B); return 4; //LD L, B
        case 0x69: LDrtr(L,C); return 4; //LD L, C
        case 0x6A: LDrtr(L,D); return 4; //LD L, D
        case 0x6B: LDrtr(L,E); return 4; //LD L, E
        case 0x6C: LDrtr(L,H); return 4; //LD L, H
        case 0x6F: LDrtr(L,A); return 4; //LD L, A
        case 0x78: LDrtr(A,B); return 4; //LD A, B
        case 0x79: LDrtr(A,C); return 4; //LD A, C
        case 0x7A: LDrtr(A,D); return 4; //LD A, D
        case 0x7B: LDrtr(A,E); return 4; //LD A, E
        case 0x7C: LDrtr(A,H); return 4; //LD A, H
        case 0x7D: LDrtr(A,L); return 4; //LD A, L

        //INC r8
        case 0x3C: Inc(A); return 4; //INC A
        case 0x04: Inc(B); return 4; //INC B
        case 0x0C: Inc(C); return 4; //INC C
        case 0x14: Inc(D); return 4; //INC D
        case 0x1C: Inc(E); return 4; //INC E
        case 0x24: Inc(H); return 4; //INC H
        case 0x2C: Inc(L); return 4; //INC L

        //DEC r8
        case 0x3D: Dec(A); return 4; //DEC A
        case 0x05: Dec(B); return 4; //DEC B
        case 0x0D: Dec(C); return 4; //DEC C
        case 0x15: Dec(D); return 4; //DEC D
        case 0x1D: Dec(E); return 4; //DEC E
        case 0x25: Dec(H); return 4; //DEC H
        case 0x2D: Dec(L); return 4; //DEC L

        //HALT
        case 0x76: halt = true; return 4; //HALT

        //ADD r8
        case 0x80: Add(B); return 4; //ADD A,B
        case 0x81: Add(C); return 4; //ADD A,C
        case 0x82: Add(D); return 4; //ADD A,D
        case 0x83: Add(E); return 4; //ADD A,E
        case 0x84: Add(H); return 4; //ADD A,H
        case 0x85: Add(L); return 4; //ADD A,L
        case 0x87: Add(A); return 4; //ADD A,A

        // ADC A,r8
        case 0x88: Adc(B); return 4; // ADC A,B
        case 0x89: Adc(C); return 4; // ADC A,C
        case 0x8A: Adc(D); return 4; // ADC A,D
        case 0x8B: Adc(E); return 4; // ADC A,E
        case 0x8C: Adc(H); return 4; // ADC A,H
        case 0x8D: Adc(L); return 4; // ADC A,L
        case 0x8F: Adc(A); return 4; // ADC A,A

        // Flag
        case 0x37: Scf(); return 4; // SCF
        case 0x2F: Cpl(); return 4; //CPL
        case 0x3F: Ccf(); return 4; //CCF

        // SUB r8
        case 0x90: Sub(B); return 4; // SUB B
        case 0x91: Sub(C); return 4; // SUB C
        case 0x92: Sub(D); return 4; // SUB D
        case 0x93: Sub(E); return 4; // SUB E
        case 0x94: Sub(H); return 4; // SUB H
        case 0x95: Sub(L); return 4; // SUB L
        case 0x97: Sub(A); return 4; // SUB A

        // SBC A,r8
        case 0x98: Sbc(B); return 4; // SBC B
        case 0x99: Sbc(C); return 4; // SBC C
        case 0x9A: Sbc(D); return 4; // SBC D
        case 0x9B: Sbc(E); return 4; // SBC E
        case 0x9C: Sbc(H); return 4; // SBC H
        case 0x9D: Sbc(L); return 4; // SBC L
        case 0x9F: Sbc(A); return 4; // SBC A

        // AND r8
        case 0xA0: And(B); return 4; // AND B
        case 0xA1: And(C); return 4; // AND C
        case 0xA2: And(D); return 4; // AND D
        case 0xA3: And(E); return 4; // AND E
        case 0xA4: And(H); return 4; // AND H
        case 0xA5: And(L); return 4; // AND L
        case 0xA7: And(A); return 4; // AND A

        // XOR r8
        case 0xA8: Xor(B); return 4; // XOR B
        case 0xA9: Xor(C); return 4; // XOR C
        case 0xAA: Xor(D); return 4; // XOR D
        case 0xAB: Xor(E); return 4; // XOR E
        case 0xAC: Xor(H); return 4; // XOR H
        case 0xAD: Xor(L); return 4; // XOR L
        case 0xAF: Xor(A); return 4; // XOR A
        
        // OR r8
        case 0xB0: Or(B); return 4; // OR B
        case 0xB1: Or(C); return 4; // OR C
        case 0xB2: Or(D); return 4; // OR D
        case 0xB3: Or(E); return 4; // OR E
        case 0xB4: Or(H); return 4; // OR H
        case 0xB5: Or(L); return 4; // OR L
        case 0xB7: Or(A); return 4; // OR A

        // CP r8
        case 0xB8: Cp(B); return 4; // CP B
        case 0xB9: Cp(C); return 4; // CP C
        case 0xBA: Cp(D); return 4; // CP D
        case 0xBB: Cp(E); return 4; // CP E
        case 0xBC: Cp(H); return 4; // CP H
        case 0xBD: Cp(L); return 4; // CP L
        case 0xBF: Cp(A); return 4; // CP A

        // ALU d8 functions 
        case 0xC6: Add(FetchByte()); return 8; //ADD A, d8
        case 0xCE: Adc(FetchByte()); return 8; //ADC A, d8
        case 0xD6: Sub(FetchByte()); return 8; //SUB A, d8
        case 0xDE: Sbc(FetchByte()); return 8; //SBC A, d8
        case 0xE6: And(FetchByte()); return 8; //AND A, d8
        case 0xEE: Xor(FetchByte()); return 8; //XOR A, d8
        case 0xF6: Or(FetchByte()); return 8; //OR A, d8
        case 0xFE: Cp(FetchByte()); return 8; //CP A, d8

        //LD rr, d16
        case 0x01: SetBC(FetchWord()); return 12; //LD BC, d16
        case 0x11: SetDE(FetchWord()); return 12; //LD DE, d16
        case 0x21: SetHL(FetchWord()); return 12; //LD HL, d16
        case 0x31: SetSP(FetchWord()); return 12; //LD SP, d16

        //INC rr
        case 0x03: SetBC(Inc16(GetBC())); return 8; //INC BC
        case 0x13: SetDE(Inc16(GetDE())); return 8; //INC DE
        case 0x23: SetHL(Inc16(GetHL())); return 8; //INC HL
        case 0x33: SetSP(Inc16(SP)); return 8; //INC SP
        
        //DEC rr
        case 0x0B: SetBC(Dec16(GetBC())); return 8; //DEC BC
        case 0x1B: SetDE(Dec16(GetDE())); return 8; //DEC DE
        case 0x2B: SetHL(Dec16(GetHL())); return 8; //DEC HL
        case 0x3B: SetSP(Dec16(SP)); return 8; //DEC SP

        //ADD rr, rr
        case 0x09: SetHL(Add16(GetHL(), GetBC())); return 8; //ADD HL, BC 
        case 0x19: SetHL(Add16(GetHL(), GetDE())); return 8; //ADD HL, DE 
        case 0x29: SetHL(Add16(GetHL(), GetHL())); return 8; //ADD HL, HL 
        case 0x39: SetHL(Add16(GetHL(), SP)); return 8; //ADD HL, SP 

        //Rotations
        case 0x07: Rlca(); return 4; //RLCA
        case 0x0F: Rrca(); return 4; //RRCA
        case 0x17: Rla(); return 4; //RLA
        case 0x1F: Rra(); return 4; //RRA

        //JP
        case 0xC2: return Jp(!GetZeroFlag()) ? 16 : 12; //JP NZ, a16
        case 0xC3: Jp(true); return 16; //JP a16
        case 0xCA: return Jp(GetZeroFlag()) ? 16 : 12; // JP Z, a16
        case 0xD2: return Jp(!GetCarryFlag()) ? 16 : 12; // JP NC, a16
        case 0xDA: return Jp(GetCarryFlag()) ? 16 : 12; // JP C, a16
        case 0xE9: PC = GetHL(); return 4; // JP (HL)

        //JR
        case 0x18: Jr(true); return 12; // JR r8
        case 0x20: return Jr(!GetZeroFlag()) ? 12 : 8; // JR NZ,r8
        case 0x28: return Jr(GetZeroFlag()) ? 12 : 8;  // JR Z,r8
        case 0x30: return Jr(!GetCarryFlag()) ? 12 : 8; // JR NC,r8
        case 0x38: return Jr(GetCarryFlag()) ? 12 : 8;  // JR C,r8

        //Interrupt Controls
        case 0xF3: Di(); return 4; //DI

        //LD with external memory
        case 0xE0: bus->Write(0xFF00 + FetchByte(), A); return 12; //LDH (a8), A
        case 0xE2: bus->Write(0xFF00 + C, A); return 8; //LD (C), A
        case 0xEA: bus->Write(FetchWord(), A); return 16; //LD (a16), A
        case 0xF0: A = bus->Read(0xFF00 + FetchByte()); return 12; //LDH A, (a8)
        case 0xF2: A = bus->Read(0xFF00 + C); return 8; //LD A, (C)
        case 0xFA: A = bus->Read(FetchWord()); return 16; //LD A, (a16)

        //PUSH
        case 0xC5: PushWord(GetBC()); return 16; //PUSH BC
        case 0xD5: PushWord(GetDE()); return 16; //PUSH DE
        case 0xE5: PushWord(GetHL()); return 16; //PUSH HL
        case 0xF5: PushWord(GetAF()); return 16; //PUSH AF

        //POP
        case 0xC1: SetBC(PopWord()); return 12; //POP BC
        case 0xD1: SetDE(PopWord()); return 12; //POP DE
        case 0xE1: SetHL(PopWord()); return 12; //POP HL
        case 0xF1: SetAF(PopWord()); return 12; //POP AF

        //CALL
        case 0xC4: return Call(!GetZeroFlag()) ? 24 : 12; //CALL NZ, a16
        case 0xCC: return Call(GetZeroFlag()) ? 24 : 12; //CALL Z, a16
        case 0xCD: Call(true); return 24; //CALL a16
        case 0xD4: return Call(!GetCarryFlag()) ? 24 : 12; //CALL NC, a16
        case 0xDC: return Call(GetCarryFlag()) ? 24 : 12; //CALL C, a16

        //RET
        case 0xC0: return Ret(!GetZeroFlag()) ? 20 : 8; //RET NZ
        case 0xC8: return Ret(GetZeroFlag()) ? 20 : 8; //RET Z
        case 0xC9: Ret(true); return 16; //RET
        case 0xD0: return Ret(!GetCarryFlag()) ? 20 : 8; //RET NC
        case 0xD8: return Ret(GetCarryFlag()) ? 20 : 8; //RET C
        case 0xD9: Reti(); return 16; //RETI

        //LD between A and (HL +/-)
        case 0x22: LdHLA(true); return 8; //LD (HL+), A
        case 0x2A: LdAHL(true); return 8; //LD A, (HL+)
        case 0x32: LdHLA(false); return 8; //LD (HL-), A
        case 0x3A: LdAHL(false); return 8; //LD A, (HL-)

        //LD (HL), r
        case 0x70: bus->Write(GetHL(), B); return 8; //LD (HL), B
        case 0x71: bus->Write(GetHL(), C); return 8; //LD (HL), C
        case 0x72: bus->Write(GetHL(), D); return 8; //LD (HL), D
        case 0x73: bus->Write(GetHL(), E); return 8; //LD (HL), E
        case 0x74: bus->Write(GetHL(), H); return 8; //LD (HL), H
        case 0x75: bus->Write(GetHL(), L); return 8; //LD (HL), L
        case 0x77: bus->Write(GetHL(), A); return 8; //LD (HL), A

        //LD r, (HL)
        case 0x46: B = bus->Read(GetHL()); return 8; //LD B, (HL)
        case 0x4E: C = bus->Read(GetHL()); return 8; //LD C, (HL)
        case 0x56: D = bus->Read(GetHL()); return 8; //LD D, (HL)
        case 0x5E: E = bus->Read(GetHL()); return 8; //LD E, (HL)
        case 0x66: H = bus->Read(GetHL()); return 8; //LD H, (HL)
        case 0x6E: L = bus->Read(GetHL()); return 8; //LD L, (HL)
        case 0x7E: A = bus->Read(GetHL()); return 8; //LD A, (HL)

        //LD (rr), r & LD r, (rr)
        case 0x02: bus->Write(GetBC(), A); return 8; //LD (BC), A
        case 0x0A: A = bus->Read(GetBC()); return 8; // LD A, (BC)
        case 0x12: bus->Write(GetDE(), A); return 8; //LD (DE), A
        case 0x1A: A = bus->Read(GetDE()); return 8; // LD A, (DE)
        

        //ALU (HL) 
        case 0x86: Add(bus->Read(GetHL())); return 8; //ADD A, (HL)
        case 0x8E: Adc(bus->Read(GetHL())); return 8; //ADC A, (HL)
        case 0x96: Sub(bus->Read(GetHL())); return 8; //SUB (HL)
        case 0x9E: Sbc(bus->Read(GetHL())); return 8; //SBC A, (HL)
        case 0xA6: And(bus->Read(GetHL())); return 8; //AND (HL)
        case 0xAE: Xor(bus->Read(GetHL())); return 8; //XOR (HL)
        case 0xB6: Or(bus->Read(GetHL())); return 8; //OR (HL)
        case 0xBE: Cp(bus->Read(GetHL())); return 8; //CP (HL)

        //CB
        case 0xCB: return Cb(); //CB

        //INC & DEC (HL)
        case 0x34: IncHL(); return 12; //INC (HL)
        case 0x35: DecHL(); return 12; //DEC (HL)

        //LD SP/HL, HL/SP+r8
        case 0xF8: LdHLSPr8(); return 12; //LD HL, SP+r8
        case 0xF9: SP = GetHL(); return 8; //LD SP, HL

        //DAA
        case 0x27: Daa(); return 4; //DAA

        //EI
        case 0xFB: InterruptDelayTimer = 2; return 4; //EI

        //RST
        case 0xC7: Rst(0x0000); return 16; // RST 00H
        case 0xCF: Rst(0x0008); return 16; // RST 08H
        case 0xD7: Rst(0x0010); return 16; // RST 10H
        case 0xDF: Rst(0x0018); return 16; // RST 18H
        case 0xE7: Rst(0x0020); return 16; // RST 20H
        case 0xEF: Rst(0x0028); return 16; // RST 28H
        case 0xF7: Rst(0x0030); return 16; // RST 30H
        case 0xFF: Rst(0x0038); return 16; // RST 38H

        //LD (HL), d8
        case 0x36: bus->Write(GetHL(), FetchByte()); return 12; //LD (HL), d8

        //LD (a16), SP
        case 0x08: WriteWord(FetchWord(), SP); return 20; //LD (a16), SP

        //ADD SP, r8
        case 0xE8: SP = AddSignedSP(FetchByte()); return 16; //ADD SP, r8

        //STOP
        case 0x10: Stop(); return 4; //STOP
    }

    return 0; //Just to stop the compiler from complaining, shouldn't affect performance :)
}

bool CPU::IsHalted()
{
    return halt;
}

uint16_t CPU::Debug()
{
    return PC;
}

void CPU::Reset()
{
    A  = 0x01;
    F  = 0xB0;
    B  = 0;
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

void CPU::WriteWord(uint16_t address, uint16_t value)
{
    bus->Write(address, value & 0xFF);
    bus->Write(address + 1, value >> 8);
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