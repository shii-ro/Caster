#include <stdio.h>
#include "z80_op.h"
#include "z80_flags.h"

// Z80 DD-prefixed Opcode enumeration (complete set)
// Z80 DD-prefixed Opcode enumeration (complete 256 values)
enum z80_dd_opcodes
{
    // 0x00-0x0F - Most are NOP or same as unprefixed
    DD_NOP_00 = 0x00,           // NOP (same as unprefixed)
    DD_LD_BC_NN = 0x01,         // LD BC, NN (same as unprefixed)
    DD_LD_BC_A = 0x02,          // LD (BC), A (same as unprefixed)
    DD_INC_BC = 0x03,           // INC BC (same as unprefixed)
    DD_INC_B = 0x04,            // INC B (same as unprefixed)
    DD_DEC_B = 0x05,            // DEC B (same as unprefixed)
    DD_LD_B_N = 0x06,           // LD B, n (same as unprefixed)
    DD_RLCA = 0x07,             // RLCA (same as unprefixed)
    DD_EX_AF_AF = 0x08,         // EX AF, AF' (same as unprefixed)
    DD_ADD_IX_BC = 0x09,        // ADD IX, BC
    DD_LD_A_BC = 0x0A,          // LD A, (BC) (same as unprefixed)
    DD_DEC_BC = 0x0B,           // DEC BC (same as unprefixed)
    DD_INC_C = 0x0C,            // INC C (same as unprefixed)
    DD_DEC_C = 0x0D,            // DEC C (same as unprefixed)
    DD_LD_C_N = 0x0E,           // LD C, n (same as unprefixed)
    DD_RRCA = 0x0F,             // RRCA (same as unprefixed)
    
    // 0x10-0x1F
    DD_DJNZ_E = 0x10,           // DJNZ e (same as unprefixed)
    DD_LD_DE_NN = 0x11,         // LD DE, NN (same as unprefixed)
    DD_LD_DE_A = 0x12,          // LD (DE), A (same as unprefixed)
    DD_INC_DE = 0x13,           // INC DE (same as unprefixed)
    DD_INC_D = 0x14,            // INC D (same as unprefixed)
    DD_DEC_D = 0x15,            // DEC D (same as unprefixed)
    DD_LD_D_N = 0x16,           // LD D, n (same as unprefixed)
    DD_RLA = 0x17,              // RLA (same as unprefixed)
    DD_JR_E = 0x18,             // JR e (same as unprefixed)
    DD_ADD_IX_DE = 0x19,        // ADD IX, DE
    DD_LD_A_DE = 0x1A,          // LD A, (DE) (same as unprefixed)
    DD_DEC_DE = 0x1B,           // DEC DE (same as unprefixed)
    DD_INC_E = 0x1C,            // INC E (same as unprefixed)
    DD_DEC_E = 0x1D,            // DEC E (same as unprefixed)
    DD_LD_E_N = 0x1E,           // LD E, n (same as unprefixed)
    DD_RRA = 0x1F,              // RRA (same as unprefixed)
    
    // 0x20-0x2F
    DD_JR_NZ_E = 0x20,          // JR NZ, e (same as unprefixed)
    DD_LD_IX_NN = 0x21,         // LD IX, NN
    DD_LD_NN_IX = 0x22,         // LD (NN), IX
    DD_INC_IX = 0x23,           // INC IX
    DD_INC_IXH = 0x24,          // INC IXH
    DD_DEC_IXH = 0x25,          // DEC IXH
    DD_LD_IXH_N = 0x26,         // LD IXH, n
    DD_DAA = 0x27,              // DAA (same as unprefixed)
    DD_JR_Z_E = 0x28,           // JR Z, e (same as unprefixed)
    DD_ADD_IX_IX = 0x29,        // ADD IX, IX
    DD_LD_IX_NN_ind = 0x2A,     // LD IX, (NN)
    DD_DEC_IX = 0x2B,           // DEC IX
    DD_INC_IXL = 0x2C,          // INC IXL
    DD_DEC_IXL = 0x2D,          // DEC IXL
    DD_LD_IXL_N = 0x2E,         // LD IXL, n
    DD_CPL = 0x2F,              // CPL (same as unprefixed)
    
    // 0x30-0x3F
    DD_JR_NC_E = 0x30,          // JR NC, e (same as unprefixed)
    DD_LD_SP_NN = 0x31,         // LD SP, NN (same as unprefixed)
    DD_LD_NN_A = 0x32,          // LD (NN), A (same as unprefixed)
    DD_INC_SP = 0x33,           // INC SP (same as unprefixed)
    DD_INC_IX_D = 0x34,         // INC (IX+d)
    DD_DEC_IX_D = 0x35,         // DEC (IX+d)
    DD_LD_IX_D_N = 0x36,        // LD (IX+d), n
    DD_SCF = 0x37,              // SCF (same as unprefixed)
    DD_JR_C_E = 0x38,           // JR C, e (same as unprefixed)
    DD_ADD_IX_SP = 0x39,        // ADD IX, SP
    DD_LD_A_NN = 0x3A,          // LD A, (NN) (same as unprefixed)
    DD_DEC_SP = 0x3B,           // DEC SP (same as unprefixed)
    DD_INC_A = 0x3C,            // INC A (same as unprefixed)
    DD_DEC_A = 0x3D,            // DEC A (same as unprefixed)
    DD_LD_A_N = 0x3E,           // LD A, n (same as unprefixed)
    DD_CCF = 0x3F,              // CCF (same as unprefixed)
    
    // 0x40-0x4F - 8-bit load instructions
    DD_LD_B_B = 0x40,           // LD B, B (same as unprefixed)
    DD_LD_B_C = 0x41,           // LD B, C (same as unprefixed)
    DD_LD_B_D = 0x42,           // LD B, D (same as unprefixed)
    DD_LD_B_E = 0x43,           // LD B, E (same as unprefixed)
    DD_LD_B_IXH = 0x44,         // LD B, IXH
    DD_LD_B_IXL = 0x45,         // LD B, IXL
    DD_LD_B_IX_D = 0x46,        // LD B, (IX+d)
    DD_LD_B_A = 0x47,           // LD B, A (same as unprefixed)
    DD_LD_C_B = 0x48,           // LD C, B (same as unprefixed)
    DD_LD_C_C = 0x49,           // LD C, C (same as unprefixed)
    DD_LD_C_D = 0x4A,           // LD C, D (same as unprefixed)
    DD_LD_C_E = 0x4B,           // LD C, E (same as unprefixed)
    DD_LD_C_IXH = 0x4C,         // LD C, IXH
    DD_LD_C_IXL = 0x4D,         // LD C, IXL
    DD_LD_C_IX_D = 0x4E,        // LD C, (IX+d)
    DD_LD_C_A = 0x4F,           // LD C, A (same as unprefixed)
    
    // 0x50-0x5F
    DD_LD_D_B = 0x50,           // LD D, B (same as unprefixed)
    DD_LD_D_C = 0x51,           // LD D, C (same as unprefixed)
    DD_LD_D_D = 0x52,           // LD D, D (same as unprefixed)
    DD_LD_D_E = 0x53,           // LD D, E (same as unprefixed)
    DD_LD_D_IXH = 0x54,         // LD D, IXH
    DD_LD_D_IXL = 0x55,         // LD D, IXL
    DD_LD_D_IX_D = 0x56,        // LD D, (IX+d)
    DD_LD_D_A = 0x57,           // LD D, A (same as unprefixed)
    DD_LD_E_B = 0x58,           // LD E, B (same as unprefixed)
    DD_LD_E_C = 0x59,           // LD E, C (same as unprefixed)
    DD_LD_E_D = 0x5A,           // LD E, D (same as unprefixed)
    DD_LD_E_E = 0x5B,           // LD E, E (same as unprefixed)
    DD_LD_E_IXH = 0x5C,         // LD E, IXH
    DD_LD_E_IXL = 0x5D,         // LD E, IXL
    DD_LD_E_IX_D = 0x5E,        // LD E, (IX+d)
    DD_LD_E_A = 0x5F,           // LD E, A (same as unprefixed)
    
    // 0x60-0x6F
    DD_LD_IXH_B = 0x60,         // LD IXH, B
    DD_LD_IXH_C = 0x61,         // LD IXH, C
    DD_LD_IXH_D = 0x62,         // LD IXH, D
    DD_LD_IXH_E = 0x63,         // LD IXH, E
    DD_LD_IXH_IXH = 0x64,       // LD IXH, IXH
    DD_LD_IXH_IXL = 0x65,       // LD IXH, IXL
    DD_LD_H_IX_D = 0x66,        // LD H, (IX+d)
    DD_LD_IXH_A = 0x67,         // LD IXH, A
    DD_LD_IXL_B = 0x68,         // LD IXL, B
    DD_LD_IXL_C = 0x69,         // LD IXL, C
    DD_LD_IXL_D = 0x6A,         // LD IXL, D
    DD_LD_IXL_E = 0x6B,         // LD IXL, E
    DD_LD_IXL_IXH = 0x6C,       // LD IXL, IXH
    DD_LD_IXL_IXL = 0x6D,       // LD IXL, IXL
    DD_LD_L_IX_D = 0x6E,        // LD L, (IX+d)
    DD_LD_IXL_A = 0x6F,         // LD IXL, A
    
    // 0x70-0x7F
    DD_LD_IX_D_B = 0x70,        // LD (IX+d), B
    DD_LD_IX_D_C = 0x71,        // LD (IX+d), C
    DD_LD_IX_D_D = 0x72,        // LD (IX+d), D
    DD_LD_IX_D_E = 0x73,        // LD (IX+d), E
    DD_LD_IX_D_H = 0x74,        // LD (IX+d), H
    DD_LD_IX_D_L = 0x75,        // LD (IX+d), L
    DD_HALT = 0x76,             // HALT (same as unprefixed)
    DD_LD_IX_D_A = 0x77,        // LD (IX+d), A
    DD_LD_A_B = 0x78,           // LD A, B (same as unprefixed)
    DD_LD_A_C = 0x79,           // LD A, C (same as unprefixed)
    DD_LD_A_D = 0x7A,           // LD A, D (same as unprefixed)
    DD_LD_A_E = 0x7B,           // LD A, E (same as unprefixed)
    DD_LD_A_IXH = 0x7C,         // LD A, IXH
    DD_LD_A_IXL = 0x7D,         // LD A, IXL
    DD_LD_A_IX_D = 0x7E,        // LD A, (IX+d)
    DD_LD_A_A = 0x7F,           // LD A, A (same as unprefixed)
    
    // 0x80-0x8F - 8-bit arithmetic ADD/ADC
    DD_ADD_A_B = 0x80,          // ADD A, B (same as unprefixed)
    DD_ADD_A_C = 0x81,          // ADD A, C (same as unprefixed)
    DD_ADD_A_D = 0x82,          // ADD A, D (same as unprefixed)
    DD_ADD_A_E = 0x83,          // ADD A, E (same as unprefixed)
    DD_ADD_A_IXH = 0x84,        // ADD A, IXH
    DD_ADD_A_IXL = 0x85,        // ADD A, IXL
    DD_ADD_A_IX_D = 0x86,       // ADD A, (IX+d)
    DD_ADD_A_A = 0x87,          // ADD A, A (same as unprefixed)
    DD_ADC_A_B = 0x88,          // ADC A, B (same as unprefixed)
    DD_ADC_A_C = 0x89,          // ADC A, C (same as unprefixed)
    DD_ADC_A_D = 0x8A,          // ADC A, D (same as unprefixed)
    DD_ADC_A_E = 0x8B,          // ADC A, E (same as unprefixed)
    DD_ADC_A_IXH = 0x8C,        // ADC A, IXH
    DD_ADC_A_IXL = 0x8D,        // ADC A, IXL
    DD_ADC_A_IX_D = 0x8E,       // ADC A, (IX+d)
    DD_ADC_A_A = 0x8F,          // ADC A, A (same as unprefixed)
    
    // 0x90-0x9F - 8-bit arithmetic SUB/SBC
    DD_SUB_A_B = 0x90,          // SUB A, B (same as unprefixed)
    DD_SUB_A_C = 0x91,          // SUB A, C (same as unprefixed)
    DD_SUB_A_D = 0x92,          // SUB A, D (same as unprefixed)
    DD_SUB_A_E = 0x93,          // SUB A, E (same as unprefixed)
    DD_SUB_A_IXH = 0x94,        // SUB A, IXH
    DD_SUB_A_IXL = 0x95,        // SUB A, IXL
    DD_SUB_A_IX_D = 0x96,       // SUB A, (IX+d)
    DD_SUB_A_A = 0x97,          // SUB A, A (same as unprefixed)
    DD_SBC_A_B = 0x98,          // SBC A, B (same as unprefixed)
    DD_SBC_A_C = 0x99,          // SBC A, C (same as unprefixed)
    DD_SBC_A_D = 0x9A,          // SBC A, D (same as unprefixed)
    DD_SBC_A_E = 0x9B,          // SBC A, E (same as unprefixed)
    DD_SBC_A_IXH = 0x9C,        // SBC A, IXH
    DD_SBC_A_IXL = 0x9D,        // SBC A, IXL
    DD_SBC_A_IX_D = 0x9E,       // SBC A, (IX+d)
    DD_SBC_A_A = 0x9F,          // SBC A, A (same as unprefixed)
    
    // 0xA0-0xAF - 8-bit logical AND/XOR
    DD_AND_A_B = 0xA0,          // AND A, B (same as unprefixed)
    DD_AND_A_C = 0xA1,          // AND A, C (same as unprefixed)
    DD_AND_A_D = 0xA2,          // AND A, D (same as unprefixed)
    DD_AND_A_E = 0xA3,          // AND A, E (same as unprefixed)
    DD_AND_A_IXH = 0xA4,        // AND A, IXH
    DD_AND_A_IXL = 0xA5,        // AND A, IXL
    DD_AND_A_IX_D = 0xA6,       // AND A, (IX+d)
    DD_AND_A_A = 0xA7,          // AND A, A (same as unprefixed)
    DD_XOR_A_B = 0xA8,          // XOR A, B (same as unprefixed)
    DD_XOR_A_C = 0xA9,          // XOR A, C (same as unprefixed)
    DD_XOR_A_D = 0xAA,          // XOR A, D (same as unprefixed)
    DD_XOR_A_E = 0xAB,          // XOR A, E (same as unprefixed)
    DD_XOR_A_IXH = 0xAC,        // XOR A, IXH
    DD_XOR_A_IXL = 0xAD,        // XOR A, IXL
    DD_XOR_A_IX_D = 0xAE,       // XOR A, (IX+d)
    DD_XOR_A_A = 0xAF,          // XOR A, A (same as unprefixed)
    
    // 0xB0-0xBF - 8-bit logical OR/CP
    DD_OR_A_B = 0xB0,           // OR A, B (same as unprefixed)
    DD_OR_A_C = 0xB1,           // OR A, C (same as unprefixed)
    DD_OR_A_D = 0xB2,           // OR A, D (same as unprefixed)
    DD_OR_A_E = 0xB3,           // OR A, E (same as unprefixed)
    DD_OR_A_IXH = 0xB4,         // OR A, IXH
    DD_OR_A_IXL = 0xB5,         // OR A, IXL
    DD_OR_A_IX_D = 0xB6,        // OR A, (IX+d)
    DD_OR_A_A = 0xB7,           // OR A, A (same as unprefixed)
    DD_CP_A_B = 0xB8,           // CP A, B (same as unprefixed)
    DD_CP_A_C = 0xB9,           // CP A, C (same as unprefixed)
    DD_CP_A_D = 0xBA,           // CP A, D (same as unprefixed)
    DD_CP_A_E = 0xBB,           // CP A, E (same as unprefixed)
    DD_CP_A_IXH = 0xBC,         // CP A, IXH
    DD_CP_A_IXL = 0xBD,         // CP A, IXL
    DD_CP_A_IX_D = 0xBE,        // CP A, (IX+d)
    DD_CP_A_A = 0xBF,           // CP A, A (same as unprefixed)
    
    // 0xC0-0xCF - Conditional returns, jumps, calls
    DD_RET_NZ = 0xC0,           // RET NZ (same as unprefixed)
    DD_POP_BC = 0xC1,           // POP BC (same as unprefixed)
    DD_JP_NZ_NN = 0xC2,         // JP NZ, NN (same as unprefixed)
    DD_JP_NN = 0xC3,            // JP NN (same as unprefixed)
    DD_CALL_NZ_NN = 0xC4,       // CALL NZ, NN (same as unprefixed)
    DD_PUSH_BC = 0xC5,          // PUSH BC (same as unprefixed)
    DD_ADD_A_N = 0xC6,          // ADD A, n (same as unprefixed)
    DD_RST_00 = 0xC7,           // RST 00 (same as unprefixed)
    DD_RET_Z = 0xC8,            // RET Z (same as unprefixed)
    DD_RET = 0xC9,              // RET (same as unprefixed)
    DD_JP_Z_NN = 0xCA,          // JP Z, NN (same as unprefixed)
    DD_PREFIX_CB = 0xCB,        // DD CB prefix (bit operations on (IX+d))
    DD_CALL_Z_NN = 0xCC,        // CALL Z, NN (same as unprefixed)
    DD_CALL_NN = 0xCD,          // CALL NN (same as unprefixed)
    DD_ADC_A_N = 0xCE,          // ADC A, n (same as unprefixed)
    DD_RST_08 = 0xCF,           // RST 08 (same as unprefixed)
    
    // 0xD0-0xDF
    DD_RET_NC = 0xD0,           // RET NC (same as unprefixed)
    DD_POP_DE = 0xD1,           // POP DE (same as unprefixed)
    DD_JP_NC_NN = 0xD2,         // JP NC, NN (same as unprefixed)
    DD_OUT_N_A = 0xD3,          // OUT (n), A (same as unprefixed)
    DD_CALL_NC_NN = 0xD4,       // CALL NC, NN (same as unprefixed)
    DD_PUSH_DE = 0xD5,          // PUSH DE (same as unprefixed)
    DD_SUB_A_N = 0xD6,          // SUB A, n (same as unprefixed)
    DD_RST_10 = 0xD7,           // RST 10 (same as unprefixed)
    DD_RET_C = 0xD8,            // RET C (same as unprefixed)
    DD_EXX = 0xD9,              // EXX (same as unprefixed)
    DD_JP_C_NN = 0xDA,          // JP C, NN (same as unprefixed)
    DD_IN_A_N = 0xDB,           // IN A, (n) (same as unprefixed)
    DD_CALL_C_NN = 0xDC,        // CALL C, NN (same as unprefixed)
    DD_INVALID_DD = 0xDD,       // DD DD - Invalid/undefined
    DD_SBC_A_N = 0xDE,          // SBC A, n (same as unprefixed)
    DD_RST_18 = 0xDF,           // RST 18 (same as unprefixed)
    
    // 0xE0-0xEF
    DD_RET_PO = 0xE0,           // RET PO (same as unprefixed)
    DD_POP_IX = 0xE1,           // POP IX
    DD_JP_PO_NN = 0xE2,         // JP PO, NN (same as unprefixed)
    DD_EX_SP_IX = 0xE3,         // EX (SP), IX
    DD_CALL_PO_NN = 0xE4,       // CALL PO, NN (same as unprefixed)
    DD_PUSH_IX = 0xE5,          // PUSH IX
    DD_AND_A_N = 0xE6,          // AND A, n (same as unprefixed)
    DD_RST_20 = 0xE7,           // RST 20 (same as unprefixed)
    DD_RET_PE = 0xE8,           // RET PE (same as unprefixed)
    DD_JP_IX = 0xE9,            // JP (IX)
    DD_JP_PE_NN = 0xEA,         // JP PE, NN (same as unprefixed)
    DD_EX_DE_HL = 0xEB,         // EX DE, HL (same as unprefixed)
    DD_CALL_PE_NN = 0xEC,       // CALL PE, NN (same as unprefixed)
    DD_PREFIX_ED = 0xED,        // ED prefix (same as unprefixed)
    DD_XOR_A_N = 0xEE,          // XOR A, n (same as unprefixed)
    DD_RST_28 = 0xEF,           // RST 28 (same as unprefixed)
    
    // 0xF0-0xFF
    DD_RET_P = 0xF0,            // RET P (same as unprefixed)
    DD_POP_AF = 0xF1,           // POP AF (same as unprefixed)
    DD_JP_P_NN = 0xF2,          // JP P, NN (same as unprefixed)
    DD_DI = 0xF3,               // DI (same as unprefixed)
    DD_CALL_P_NN = 0xF4,        // CALL P, NN (same as unprefixed)
    DD_PUSH_AF = 0xF5,          // PUSH AF (same as unprefixed)
    DD_OR_A_N = 0xF6,           // OR A, n (same as unprefixed)
    DD_RST_30 = 0xF7,           // RST 30 (same as unprefixed)
    DD_RET_M = 0xF8,            // RET M (same as unprefixed)
    DD_LD_SP_IX = 0xF9,         // LD SP, IX
    DD_JP_M_NN = 0xFA,          // JP M, NN (same as unprefixed)
    DD_EI = 0xFB,               // EI (same as unprefixed)
    DD_CALL_M_NN = 0xFC,        // CALL M, NN (same as unprefixed)
    DD_PREFIX_FD = 0xFD,        // FD prefix (same as unprefixed)
    DD_CP_A_N = 0xFE,           // CP A, n (same as unprefixed)
    DD_RST_38 = 0xFF            // RST 38 (same as unprefixed)
};

void z80_execute_dd_instruction(struct z80_t *cpu, uint8_t opcode)
{
    switch (opcode)
    {
    case DD_ADD_IX_BC: cpu->registers.IX = z80_op_add16(cpu, cpu->registers.IX, cpu->registers.BC); break;
    case DD_ADD_IX_DE: cpu->registers.IX = z80_op_add16(cpu, cpu->registers.IX, cpu->registers.DE); break;
    case DD_ADD_IX_IX: cpu->registers.IX = z80_op_add16(cpu, cpu->registers.IX, cpu->registers.IX); break;
    case DD_ADD_IX_SP: cpu->registers.IX = z80_op_add16(cpu, cpu->registers.IX, cpu->registers.SP); break;
    case DD_LD_IX_NN:  cpu->registers.IX = z80_fetch16(cpu); break;
    case DD_INC_IX: cpu->registers.IX++; break;
    case DD_DEC_IX: cpu->registers.IX--; break;
    case DD_DEC_IXH: cpu->registers.IXH = z80_op_dec8(cpu, cpu->registers.IXH); break;
    case DD_DEC_IXL: cpu->registers.IXL = z80_op_dec8(cpu, cpu->registers.IXL); break;
    case DD_INC_IXH: cpu->registers.IXH = z80_op_inc8(cpu, cpu->registers.IXH); break;
    case DD_INC_IXL: cpu->registers.IXL = z80_op_inc8(cpu, cpu->registers.IXL); break;
    case DD_INC_IX_D:
    {
        int8_t offset = (int8_t)z80_fetch8(cpu);
        uint16_t address = cpu->registers.IX + offset;
        uint8_t original = z80_read8(cpu, address);
        uint8_t result = z80_op_inc8(cpu, original);
        cpu->cycle_count++;
        z80_write8(cpu, address, result);
        break;
    }
    case DD_DEC_IX_D:
    {
        int8_t offset = (int8_t)z80_fetch8(cpu);
        uint16_t address = cpu->registers.IX + offset;
        uint8_t original = z80_read8(cpu, address);
        uint8_t result = z80_op_dec8(cpu, original);
        cpu->cycle_count++;
        z80_write8(cpu, address, result);
        break;
    }
    case DD_LD_IX_D_N:
    {
        int8_t d = (int8_t)z80_fetch8(cpu); // displacement
        uint8_t n = z80_fetch8(cpu);        // immediate value
        z80_write8(cpu, cpu->registers.IX + d, n);
        break;
    }
    case DD_LD_B_B:    cpu->registers.B = cpu->registers.B; break;
    case DD_LD_B_C:    cpu->registers.B = cpu->registers.C; break;
    case DD_LD_B_D:    cpu->registers.B = cpu->registers.D; break;
    case DD_LD_B_E:    cpu->registers.B = cpu->registers.E; break;
    case DD_LD_B_IXH:  cpu->registers.B = cpu->registers.IXH; break;
    case DD_LD_B_IXL:  cpu->registers.B = cpu->registers.IXL; break;
    case DD_LD_B_A:    cpu->registers.B = cpu->registers.A; break;
    case DD_LD_C_B:    cpu->registers.C = cpu->registers.B; break;
    case DD_LD_C_C:    cpu->registers.C = cpu->registers.C; break;
    case DD_LD_C_D:    cpu->registers.C = cpu->registers.D; break;
    case DD_LD_C_E:    cpu->registers.C = cpu->registers.E; break;
    case DD_LD_C_IXH:  cpu->registers.C = cpu->registers.IXH; break;
    case DD_LD_C_IXL:  cpu->registers.C = cpu->registers.IXL; break;
    case DD_LD_C_A:    cpu->registers.C = cpu->registers.A; break;
    case DD_LD_D_B:    cpu->registers.D = cpu->registers.B; break;
    case DD_LD_D_C:    cpu->registers.D = cpu->registers.C; break;
    case DD_LD_D_D:    cpu->registers.D = cpu->registers.D; break;
    case DD_LD_D_E:    cpu->registers.D = cpu->registers.E; break;
    case DD_LD_D_IXH:  cpu->registers.D = cpu->registers.IXH; break;
    case DD_LD_D_IXL:  cpu->registers.D = cpu->registers.IXL; break;
    case DD_LD_D_A:    cpu->registers.D = cpu->registers.A; break;
    case DD_LD_E_B:    cpu->registers.E = cpu->registers.B; break;
    case DD_LD_E_C:    cpu->registers.E = cpu->registers.C; break;
    case DD_LD_E_D:    cpu->registers.E = cpu->registers.D; break;
    case DD_LD_E_E:    cpu->registers.E = cpu->registers.E; break;
    case DD_LD_E_IXH:  cpu->registers.E = cpu->registers.IXH; break;
    case DD_LD_E_IXL:  cpu->registers.E = cpu->registers.IXL; break;
    case DD_LD_E_A:    cpu->registers.E = cpu->registers.A; break;
    case DD_LD_IXH_B:  cpu->registers.IXH = cpu->registers.B; break;
    case DD_LD_IXH_C:  cpu->registers.IXH = cpu->registers.C; break;
    case DD_LD_IXH_D:  cpu->registers.IXH = cpu->registers.D; break;
    case DD_LD_IXH_E:  cpu->registers.IXH = cpu->registers.E; break;
    case DD_LD_IXH_IXH: cpu->registers.IXH = cpu->registers.IXH; break;
    case DD_LD_IXH_IXL: cpu->registers.IXH = cpu->registers.IXL; break;
    case DD_LD_IXH_A:  cpu->registers.IXH = cpu->registers.A; break;
    case DD_LD_IXL_B:  cpu->registers.IXL = cpu->registers.B; break;
    case DD_LD_IXL_C:  cpu->registers.IXL = cpu->registers.C; break;
    case DD_LD_IXL_D:  cpu->registers.IXL = cpu->registers.D; break;
    case DD_LD_IXL_E:  cpu->registers.IXL = cpu->registers.E; break;
    case DD_LD_IXL_IXH: cpu->registers.IXL = cpu->registers.IXH; break;
    case DD_LD_IXL_IXL: cpu->registers.IXL = cpu->registers.IXL; break;
    case DD_LD_IXL_A:  cpu->registers.IXL = cpu->registers.A; break;
    case DD_LD_IX_D_B: z80_write8(cpu, cpu->registers.IX + (int8_t)z80_fetch8(cpu), cpu->registers.B); break;
    case DD_LD_IX_D_C: z80_write8(cpu, cpu->registers.IX + (int8_t)z80_fetch8(cpu), cpu->registers.C); break;
    case DD_LD_IX_D_D: z80_write8(cpu, cpu->registers.IX + (int8_t)z80_fetch8(cpu), cpu->registers.D); break;
    case DD_LD_IX_D_E: z80_write8(cpu, cpu->registers.IX + (int8_t)z80_fetch8(cpu), cpu->registers.E); break;
    case DD_LD_IX_D_H: z80_write8(cpu, cpu->registers.IX + (int8_t)z80_fetch8(cpu), cpu->registers.H); break;
    case DD_LD_IX_D_L: z80_write8(cpu, cpu->registers.IX + (int8_t)z80_fetch8(cpu), cpu->registers.L); break;
    case DD_LD_IX_D_A: z80_write8(cpu, cpu->registers.IX + (int8_t)z80_fetch8(cpu), cpu->registers.A); break;
    case DD_LD_A_B:    cpu->registers.A = cpu->registers.B; break;
    case DD_LD_A_C:    cpu->registers.A = cpu->registers.C; break;
    case DD_LD_A_D:    cpu->registers.A = cpu->registers.D; break;
    case DD_LD_A_E:    cpu->registers.A = cpu->registers.E; break;
    case DD_LD_A_IXH:  cpu->registers.A = cpu->registers.IXH; break;
    case DD_LD_A_IXL:  cpu->registers.A = cpu->registers.IXL; break;
    case DD_LD_A_A:    cpu->registers.A = cpu->registers.A; break;
    case DD_LD_IXH_N:  cpu->registers.IXH  = z80_fetch8(cpu); break;
    case DD_LD_IXL_N:  cpu->registers.IXL = z80_fetch8(cpu); break;
    case DD_LD_B_IX_D: cpu->registers.B = z80_read8(cpu, (cpu->registers.IX + (int8_t)z80_fetch8(cpu))); break;
    case DD_LD_C_IX_D: cpu->registers.C = z80_read8(cpu, (cpu->registers.IX + (int8_t)z80_fetch8(cpu))); break;
    case DD_LD_D_IX_D: cpu->registers.D = z80_read8(cpu, (cpu->registers.IX + (int8_t)z80_fetch8(cpu))); break;
    case DD_LD_E_IX_D: cpu->registers.E = z80_read8(cpu, (cpu->registers.IX + (int8_t)z80_fetch8(cpu))); break;
    case DD_LD_H_IX_D: cpu->registers.H = z80_read8(cpu, (cpu->registers.IX + (int8_t)z80_fetch8(cpu))); break;
    case DD_LD_L_IX_D: cpu->registers.L = z80_read8(cpu, (cpu->registers.IX + (int8_t)z80_fetch8(cpu))); break;
    case DD_LD_NN_IX: z80_write16(cpu, z80_fetch16(cpu), cpu->registers.IX); break;
    case DD_LD_IX_NN_ind: cpu->registers.IX = z80_read16(cpu, z80_fetch16(cpu)); break;
    case DD_LD_A_IX_D: cpu->registers.A = z80_read8(cpu, (cpu->registers.IX + (int8_t)z80_fetch8(cpu))); break;
    case DD_POP_IX: cpu->registers.IX = z80_stack_pop16(cpu); break;
    case DD_PUSH_IX: z80_stack_push16(cpu, cpu->registers.IX); break;
    case DD_JP_IX: cpu->registers.PC = cpu->registers.IX; break;
    case DD_ADD_A_IXH:     z80_op_add8(cpu, cpu->registers.IXH); break;
    case DD_ADD_A_IXL:     z80_op_add8(cpu, cpu->registers.IXL); break;
    case DD_ADD_A_IX_D:    z80_op_add8(cpu, z80_read8(cpu, cpu->registers.IX + (int8_t)z80_fetch8(cpu))); break;
    case DD_ADC_A_IXH:     z80_op_adc8(cpu, cpu->registers.IXH); break;
    case DD_ADC_A_IXL:     z80_op_adc8(cpu, cpu->registers.IXL); break;
    case DD_ADC_A_IX_D:    z80_op_adc8(cpu, z80_read8(cpu, cpu->registers.IX + (int8_t)z80_fetch8(cpu))); break;
    case DD_SUB_A_IXH:     z80_op_sub8(cpu, cpu->registers.IXH); break;
    case DD_SUB_A_IXL:     z80_op_sub8(cpu, cpu->registers.IXL); break;
    case DD_SUB_A_IX_D:    z80_op_sub8(cpu, z80_read8(cpu, cpu->registers.IX + (int8_t)z80_fetch8(cpu))); break;
    case DD_SBC_A_IXH:     z80_op_sbc8(cpu, cpu->registers.IXH); break;
    case DD_SBC_A_IXL:     z80_op_sbc8(cpu, cpu->registers.IXL); break;
    case DD_SBC_A_IX_D:    z80_op_sbc8(cpu, z80_read8(cpu, cpu->registers.IX + (int8_t)z80_fetch8(cpu))); break;
    case DD_AND_A_IXH:     z80_op_and(cpu, cpu->registers.IXH); break;
    case DD_AND_A_IXL:     z80_op_and(cpu, cpu->registers.IXL); break;
    case DD_AND_A_IX_D:    z80_op_and(cpu, z80_read8(cpu, cpu->registers.IX + (int8_t)z80_fetch8(cpu))); break;
    case DD_XOR_A_IXH:     z80_op_xor(cpu, cpu->registers.IXH); break;
    case DD_XOR_A_IXL:     z80_op_xor(cpu, cpu->registers.IXL); break;
    case DD_XOR_A_IX_D:    z80_op_xor(cpu, z80_read8(cpu, cpu->registers.IX + (int8_t)z80_fetch8(cpu))); break;
    case DD_OR_A_IXH:      z80_op_or(cpu, cpu->registers.IXH); break;
    case DD_OR_A_IXL:      z80_op_or(cpu, cpu->registers.IXL); break;
    case DD_OR_A_IX_D:     z80_op_or(cpu, z80_read8(cpu, cpu->registers.IX + (int8_t)z80_fetch8(cpu))); break;
    case DD_CP_A_IXH:      z80_op_cp(cpu, cpu->registers.A, cpu->registers.IXH); break;
    case DD_CP_A_IXL:      z80_op_cp(cpu, cpu->registers.A, cpu->registers.IXL); break;
    case DD_CP_A_IX_D:     z80_op_cp(cpu, cpu->registers.A, z80_read8(cpu, cpu->registers.IX + (int8_t)z80_fetch8(cpu))); break;
    case DD_PREFIX_CB:     z80_execute_dd_cb_instruction(cpu, z80_fetch8(cpu)); break;
    default:
        printf("Unimplemented 0xDD Instruction: 0x%02X\n", opcode);
        cpu->running = false;
        cpu->halted = true;
        break;
    }
}