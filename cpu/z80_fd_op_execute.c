#include <stdio.h>
#include "z80_op.h"
#include "z80_flags.h"

// Z80 FD (IY) prefiY instruction opcodes
enum z80_fd_opcodes
{
    // 0x00-0x0F - Most are NOP or same as unprefiYed
    FD_NOP_00 = 0x00,           // NOP (same as unprefiYed)
    FD_LD_BC_NN = 0x01,         // LD BC, NN (same as unprefiYed)
    FD_LD_BC_A = 0x02,          // LD (BC), A (same as unprefiYed)
    FD_INC_BC = 0x03,           // INC BC (same as unprefiYed)
    FD_INC_B = 0x04,            // INC B (same as unprefiYed)
    FD_DEC_B = 0x05,            // DEC B (same as unprefiYed)
    FD_LD_B_N = 0x06,           // LD B, n (same as unprefiYed)
    FD_RLCA = 0x07,             // RLCA (same as unprefiYed)
    FD_EX_AF_AF = 0x08,         // EX AF, AF' (same as unprefiYed)
    FD_ADD_IY_BC = 0x09,        // ADD IY, BC
    FD_LD_A_BC = 0x0A,          // LD A, (BC) (same as unprefiYed)
    FD_DEC_BC = 0x0B,           // DEC BC (same as unprefiYed)
    FD_INC_C = 0x0C,            // INC C (same as unprefiYed)
    FD_DEC_C = 0x0D,            // DEC C (same as unprefiYed)
    FD_LD_C_N = 0x0E,           // LD C, n (same as unprefiYed)
    FD_RRCA = 0x0F,             // RRCA (same as unprefiYed)
    
    // 0x10-0x1F
    FD_DJNZ_E = 0x10,           // DJNZ e (same as unprefiYed)
    FD_LD_DE_NN = 0x11,         // LD DE, NN (same as unprefiYed)
    FD_LD_DE_A = 0x12,          // LD (DE), A (same as unprefiYed)
    FD_INC_DE = 0x13,           // INC DE (same as unprefiYed)
    FD_INC_D = 0x14,            // INC D (same as unprefiYed)
    FD_DEC_D = 0x15,            // DEC D (same as unprefiYed)
    FD_LD_D_N = 0x16,           // LD D, n (same as unprefiYed)
    FD_RLA = 0x17,              // RLA (same as unprefiYed)
    FD_JR_E = 0x18,             // JR e (same as unprefiYed)
    FD_ADD_IY_DE = 0x19,        // ADD IY, DE
    FD_LD_A_DE = 0x1A,          // LD A, (DE) (same as unprefiYed)
    FD_DEC_DE = 0x1B,           // DEC DE (same as unprefiYed)
    FD_INC_E = 0x1C,            // INC E (same as unprefiYed)
    FD_DEC_E = 0x1D,            // DEC E (same as unprefiYed)
    FD_LD_E_N = 0x1E,           // LD E, n (same as unprefiYed)
    FD_RRA = 0x1F,              // RRA (same as unprefiYed)
    
    // 0x20-0x2F
    FD_JR_NZ_E = 0x20,          // JR NZ, e (same as unprefiYed)
    FD_LD_IY_NN = 0x21,         // LD IY, NN
    FD_LD_NN_IY = 0x22,         // LD (NN), IY
    FD_INC_IY = 0x23,           // INC IY
    FD_INC_IYH = 0x24,          // INC IYH
    FD_DEC_IYH = 0x25,          // DEC IYH
    FD_LD_IYH_N = 0x26,         // LD IYH, n
    FD_DAA = 0x27,              // DAA (same as unprefiYed)
    FD_JR_Z_E = 0x28,           // JR Z, e (same as unprefiYed)
    FD_ADD_IY_IY = 0x29,        // ADD IY, IY
    FD_LD_IY_NN_ind = 0x2A,     // LD IY, (NN)
    FD_DEC_IY = 0x2B,           // DEC IY
    FD_INC_IYL = 0x2C,          // INC IYL
    FD_DEC_IYL = 0x2D,          // DEC IYL
    FD_LD_IYL_N = 0x2E,         // LD IYL, n
    FD_CPL = 0x2F,              // CPL (same as unprefiYed)
    
    // 0x30-0x3F
    FD_JR_NC_E = 0x30,          // JR NC, e (same as unprefiYed)
    FD_LD_SP_NN = 0x31,         // LD SP, NN (same as unprefiYed)
    FD_LD_NN_A = 0x32,          // LD (NN), A (same as unprefiYed)
    FD_INC_SP = 0x33,           // INC SP (same as unprefiYed)
    FD_INC_IY_D = 0x34,         // INC (IY+d)
    FD_DEC_IY_D = 0x35,         // DEC (IY+d)
    FD_LD_IY_D_N = 0x36,        // LD (IY+d), n
    FD_SCF = 0x37,              // SCF (same as unprefiYed)
    FD_JR_C_E = 0x38,           // JR C, e (same as unprefiYed)
    FD_ADD_IY_SP = 0x39,        // ADD IY, SP
    FD_LD_A_NN = 0x3A,          // LD A, (NN) (same as unprefiYed)
    FD_DEC_SP = 0x3B,           // DEC SP (same as unprefiYed)
    FD_INC_A = 0x3C,            // INC A (same as unprefiYed)
    FD_DEC_A = 0x3D,            // DEC A (same as unprefiYed)
    FD_LD_A_N = 0x3E,           // LD A, n (same as unprefiYed)
    FD_CCF = 0x3F,              // CCF (same as unprefiYed)
    
    // 0x40-0x4F - 8-bit load instructions
    FD_LD_B_B = 0x40,           // LD B, B (same as unprefiYed)
    FD_LD_B_C = 0x41,           // LD B, C (same as unprefiYed)
    FD_LD_B_D = 0x42,           // LD B, D (same as unprefiYed)
    FD_LD_B_E = 0x43,           // LD B, E (same as unprefiYed)
    FD_LD_B_IYH = 0x44,         // LD B, IYH
    FD_LD_B_IYL = 0x45,         // LD B, IYL
    FD_LD_B_IY_D = 0x46,        // LD B, (IY+d)
    FD_LD_B_A = 0x47,           // LD B, A (same as unprefiYed)
    FD_LD_C_B = 0x48,           // LD C, B (same as unprefiYed)
    FD_LD_C_C = 0x49,           // LD C, C (same as unprefiYed)
    FD_LD_C_D = 0x4A,           // LD C, D (same as unprefiYed)
    FD_LD_C_E = 0x4B,           // LD C, E (same as unprefiYed)
    FD_LD_C_IYH = 0x4C,         // LD C, IYH
    FD_LD_C_IYL = 0x4D,         // LD C, IYL
    FD_LD_C_IY_D = 0x4E,        // LD C, (IY+d)
    FD_LD_C_A = 0x4F,           // LD C, A (same as unprefiYed)
    
    // 0x50-0x5F
    FD_LD_D_B = 0x50,           // LD D, B (same as unprefiYed)
    FD_LD_D_C = 0x51,           // LD D, C (same as unprefiYed)
    FD_LD_D_D = 0x52,           // LD D, D (same as unprefiYed)
    FD_LD_D_E = 0x53,           // LD D, E (same as unprefiYed)
    FD_LD_D_IYH = 0x54,         // LD D, IYH
    FD_LD_D_IYL = 0x55,         // LD D, IYL
    FD_LD_D_IY_D = 0x56,        // LD D, (IY+d)
    FD_LD_D_A = 0x57,           // LD D, A (same as unprefiYed)
    FD_LD_E_B = 0x58,           // LD E, B (same as unprefiYed)
    FD_LD_E_C = 0x59,           // LD E, C (same as unprefiYed)
    FD_LD_E_D = 0x5A,           // LD E, D (same as unprefiYed)
    FD_LD_E_E = 0x5B,           // LD E, E (same as unprefiYed)
    FD_LD_E_IYH = 0x5C,         // LD E, IYH
    FD_LD_E_IYL = 0x5D,         // LD E, IYL
    FD_LD_E_IY_D = 0x5E,        // LD E, (IY+d)
    FD_LD_E_A = 0x5F,           // LD E, A (same as unprefiYed)
    
    // 0x60-0x6F
    FD_LD_IYH_B = 0x60,         // LD IYH, B
    FD_LD_IYH_C = 0x61,         // LD IYH, C
    FD_LD_IYH_D = 0x62,         // LD IYH, D
    FD_LD_IYH_E = 0x63,         // LD IYH, E
    FD_LD_IYH_IYH = 0x64,       // LD IYH, IYH
    FD_LD_IYH_IYL = 0x65,       // LD IYH, IYL
    FD_LD_H_IY_D = 0x66,        // LD H, (IY+d)
    FD_LD_IYH_A = 0x67,         // LD IYH, A
    FD_LD_IYL_B = 0x68,         // LD IYL, B
    FD_LD_IYL_C = 0x69,         // LD IYL, C
    FD_LD_IYL_D = 0x6A,         // LD IYL, D
    FD_LD_IYL_E = 0x6B,         // LD IYL, E
    FD_LD_IYL_IYH = 0x6C,       // LD IYL, IYH
    FD_LD_IYL_IYL = 0x6D,       // LD IYL, IYL
    FD_LD_L_IY_D = 0x6E,        // LD L, (IY+d)
    FD_LD_IYL_A = 0x6F,         // LD IYL, A
    
    // 0x70-0x7F
    FD_LD_IY_D_B = 0x70,        // LD (IY+d), B
    FD_LD_IY_D_C = 0x71,        // LD (IY+d), C
    FD_LD_IY_D_D = 0x72,        // LD (IY+d), D
    FD_LD_IY_D_E = 0x73,        // LD (IY+d), E
    FD_LD_IY_D_H = 0x74,        // LD (IY+d), H
    FD_LD_IY_D_L = 0x75,        // LD (IY+d), L
    FD_HALT = 0x76,             // HALT (same as unprefiYed)
    FD_LD_IY_D_A = 0x77,        // LD (IY+d), A
    FD_LD_A_B = 0x78,           // LD A, B (same as unprefiYed)
    FD_LD_A_C = 0x79,           // LD A, C (same as unprefiYed)
    FD_LD_A_D = 0x7A,           // LD A, D (same as unprefiYed)
    FD_LD_A_E = 0x7B,           // LD A, E (same as unprefiYed)
    FD_LD_A_IYH = 0x7C,         // LD A, IYH
    FD_LD_A_IYL = 0x7D,         // LD A, IYL
    FD_LD_A_IY_D = 0x7E,        // LD A, (IY+d)
    FD_LD_A_A = 0x7F,           // LD A, A (same as unprefiYed)
    
    // 0x80-0x8F - 8-bit arithmetic ADD/ADC
    FD_ADD_A_B = 0x80,          // ADD A, B (same as unprefiYed)
    FD_ADD_A_C = 0x81,          // ADD A, C (same as unprefiYed)
    FD_ADD_A_D = 0x82,          // ADD A, D (same as unprefiYed)
    FD_ADD_A_E = 0x83,          // ADD A, E (same as unprefiYed)
    FD_ADD_A_IYH = 0x84,        // ADD A, IYH
    FD_ADD_A_IYL = 0x85,        // ADD A, IYL
    FD_ADD_A_IY_D = 0x86,       // ADD A, (IY+d)
    FD_ADD_A_A = 0x87,          // ADD A, A (same as unprefiYed)
    FD_ADC_A_B = 0x88,          // ADC A, B (same as unprefiYed)
    FD_ADC_A_C = 0x89,          // ADC A, C (same as unprefiYed)
    FD_ADC_A_D = 0x8A,          // ADC A, D (same as unprefiYed)
    FD_ADC_A_E = 0x8B,          // ADC A, E (same as unprefiYed)
    FD_ADC_A_IYH = 0x8C,        // ADC A, IYH
    FD_ADC_A_IYL = 0x8D,        // ADC A, IYL
    FD_ADC_A_IY_D = 0x8E,       // ADC A, (IY+d)
    FD_ADC_A_A = 0x8F,          // ADC A, A (same as unprefiYed)
    
    // 0x90-0x9F - 8-bit arithmetic SUB/SBC
    FD_SUB_A_B = 0x90,          // SUB A, B (same as unprefiYed)
    FD_SUB_A_C = 0x91,          // SUB A, C (same as unprefiYed)
    FD_SUB_A_D = 0x92,          // SUB A, D (same as unprefiYed)
    FD_SUB_A_E = 0x93,          // SUB A, E (same as unprefiYed)
    FD_SUB_A_IYH = 0x94,        // SUB A, IYH
    FD_SUB_A_IYL = 0x95,        // SUB A, IYL
    FD_SUB_A_IY_D = 0x96,       // SUB A, (IY+d)
    FD_SUB_A_A = 0x97,          // SUB A, A (same as unprefiYed)
    FD_SBC_A_B = 0x98,          // SBC A, B (same as unprefiYed)
    FD_SBC_A_C = 0x99,          // SBC A, C (same as unprefiYed)
    FD_SBC_A_D = 0x9A,          // SBC A, D (same as unprefiYed)
    FD_SBC_A_E = 0x9B,          // SBC A, E (same as unprefiYed)
    FD_SBC_A_IYH = 0x9C,        // SBC A, IYH
    FD_SBC_A_IYL = 0x9D,        // SBC A, IYL
    FD_SBC_A_IY_D = 0x9E,       // SBC A, (IY+d)
    FD_SBC_A_A = 0x9F,          // SBC A, A (same as unprefiYed)
    
    // 0xA0-0xAF - 8-bit logical AND/XOR
    FD_AND_A_B = 0xA0,          // AND A, B (same as unprefiYed)
    FD_AND_A_C = 0xA1,          // AND A, C (same as unprefiYed)
    FD_AND_A_D = 0xA2,          // AND A, D (same as unprefiYed)
    FD_AND_A_E = 0xA3,          // AND A, E (same as unprefiYed)
    FD_AND_A_IYH = 0xA4,        // AND A, IYH
    FD_AND_A_IYL = 0xA5,        // AND A, IYL
    FD_AND_A_IY_D = 0xA6,       // AND A, (IY+d)
    FD_AND_A_A = 0xA7,          // AND A, A (same as unprefiYed)
    FD_XOR_A_B = 0xA8,          // XOR A, B (same as unprefiYed)
    FD_XOR_A_C = 0xA9,          // XOR A, C (same as unprefiYed)
    FD_XOR_A_D = 0xAA,          // XOR A, D (same as unprefiYed)
    FD_XOR_A_E = 0xAB,          // XOR A, E (same as unprefiYed)
    FD_XOR_A_IYH = 0xAC,        // XOR A, IYH
    FD_XOR_A_IYL = 0xAD,        // XOR A, IYL
    FD_XOR_A_IY_D = 0xAE,       // XOR A, (IY+d)
    FD_XOR_A_A = 0xAF,          // XOR A, A (same as unprefiYed)
    
    // 0xB0-0xBF - 8-bit logical OR/CP
    FD_OR_A_B = 0xB0,           // OR A, B (same as unprefiYed)
    FD_OR_A_C = 0xB1,           // OR A, C (same as unprefiYed)
    FD_OR_A_D = 0xB2,           // OR A, D (same as unprefiYed)
    FD_OR_A_E = 0xB3,           // OR A, E (same as unprefiYed)
    FD_OR_A_IYH = 0xB4,         // OR A, IYH
    FD_OR_A_IYL = 0xB5,         // OR A, IYL
    FD_OR_A_IY_D = 0xB6,        // OR A, (IY+d)
    FD_OR_A_A = 0xB7,           // OR A, A (same as unprefiYed)
    FD_CP_A_B = 0xB8,           // CP A, B (same as unprefiYed)
    FD_CP_A_C = 0xB9,           // CP A, C (same as unprefiYed)
    FD_CP_A_D = 0xBA,           // CP A, D (same as unprefiYed)
    FD_CP_A_E = 0xBB,           // CP A, E (same as unprefiYed)
    FD_CP_A_IYH = 0xBC,         // CP A, IYH
    FD_CP_A_IYL = 0xBD,         // CP A, IYL
    FD_CP_A_IY_D = 0xBE,        // CP A, (IY+d)
    FD_CP_A_A = 0xBF,           // CP A, A (same as unprefiYed)
    
    // 0xC0-0xCF - Conditional returns, jumps, calls
    FD_RET_NZ = 0xC0,           // RET NZ (same as unprefiYed)
    FD_POP_BC = 0xC1,           // POP BC (same as unprefiYed)
    FD_JP_NZ_NN = 0xC2,         // JP NZ, NN (same as unprefiYed)
    FD_JP_NN = 0xC3,            // JP NN (same as unprefiYed)
    FD_CALL_NZ_NN = 0xC4,       // CALL NZ, NN (same as unprefiYed)
    FD_PUSH_BC = 0xC5,          // PUSH BC (same as unprefiYed)
    FD_ADD_A_N = 0xC6,          // ADD A, n (same as unprefiYed)
    FD_RST_00 = 0xC7,           // RST 00 (same as unprefiYed)
    FD_RET_Z = 0xC8,            // RET Z (same as unprefiYed)
    FD_RET = 0xC9,              // RET (same as unprefiYed)
    FD_JP_Z_NN = 0xCA,          // JP Z, NN (same as unprefiYed)
    FD_PREFIY_CB = 0xCB,        // FD CB prefiY (bit operations on (IY+d))
    FD_CALL_Z_NN = 0xCC,        // CALL Z, NN (same as unprefiYed)
    FD_CALL_NN = 0xCD,          // CALL NN (same as unprefiYed)
    FD_ADC_A_N = 0xCE,          // ADC A, n (same as unprefiYed)
    FD_RST_08 = 0xCF,           // RST 08 (same as unprefiYed)
    
    // 0xD0-0xDF
    FD_RET_NC = 0xD0,           // RET NC (same as unprefiYed)
    FD_POP_DE = 0xD1,           // POP DE (same as unprefiYed)
    FD_JP_NC_NN = 0xD2,         // JP NC, NN (same as unprefiYed)
    FD_OUT_N_A = 0xD3,          // OUT (n), A (same as unprefiYed)
    FD_CALL_NC_NN = 0xD4,       // CALL NC, NN (same as unprefiYed)
    FD_PUSH_DE = 0xD5,          // PUSH DE (same as unprefiYed)
    FD_SUB_A_N = 0xD6,          // SUB A, n (same as unprefiYed)
    FD_RST_10 = 0xD7,           // RST 10 (same as unprefiYed)
    FD_RET_C = 0xD8,            // RET C (same as unprefiYed)
    FD_EXX = 0xD9,              // EXX (same as unprefiYed)
    FD_JP_C_NN = 0xDA,          // JP C, NN (same as unprefiYed)
    FD_IN_A_N = 0xDB,           // IN A, (n) (same as unprefiYed)
    FD_CALL_C_NN = 0xDC,        // CALL C, NN (same as unprefiYed)
    FD_INVALID_DD = 0xDD,       // FD FD - Invalid/undefined
    FD_SBC_A_N = 0xDE,          // SBC A, n (same as unprefiYed)
    FD_RST_18 = 0xDF,           // RST 18 (same as unprefiYed)
    
    // 0xE0-0xEF
    FD_RET_PO = 0xE0,           // RET PO (same as unprefiYed)
    FD_POP_IY = 0xE1,           // POP IY
    FD_JP_PO_NN = 0xE2,         // JP PO, NN (same as unprefiYed)
    FD_EX_SP_IY = 0xE3,         // EX (SP), IY
    FD_CALL_PO_NN = 0xE4,       // CALL PO, NN (same as unprefiYed)
    FD_PUSH_IY = 0xE5,          // PUSH IY
    FD_AND_A_N = 0xE6,          // AND A, n (same as unprefiYed)
    FD_RST_20 = 0xE7,           // RST 20 (same as unprefiYed)
    FD_RET_PE = 0xE8,           // RET PE (same as unprefiYed)
    FD_JP_IY = 0xE9,            // JP (IY)
    FD_JP_PE_NN = 0xEA,         // JP PE, NN (same as unprefiYed)
    FD_EX_DE_HL = 0xEB,         // EX DE, HL (same as unprefiYed)
    FD_CALL_PE_NN = 0xEC,       // CALL PE, NN (same as unprefiYed)
    FD_PREFIY_ED = 0xED,        // ED prefiY (same as unprefiYed)
    FD_XOR_A_N = 0xEE,          // XOR A, n (same as unprefiYed)
    FD_RST_28 = 0xEF,           // RST 28 (same as unprefiYed)
    
    // 0xF0-0xFF
    FD_RET_P = 0xF0,            // RET P (same as unprefiYed)
    FD_POP_AF = 0xF1,           // POP AF (same as unprefiYed)
    FD_JP_P_NN = 0xF2,          // JP P, NN (same as unprefiYed)
    FD_DI = 0xF3,               // DI (same as unprefiYed)
    FD_CALL_P_NN = 0xF4,        // CALL P, NN (same as unprefiYed)
    FD_PUSH_AF = 0xF5,          // PUSH AF (same as unprefiYed)
    FD_OR_A_N = 0xF6,           // OR A, n (same as unprefiYed)
    FD_RST_30 = 0xF7,           // RST 30 (same as unprefiYed)
    FD_RET_M = 0xF8,            // RET M (same as unprefiYed)
    FD_LD_SP_IY = 0xF9,         // LD SP, IY
    FD_JP_M_NN = 0xFA,          // JP M, NN (same as unprefiYed)
    FD_EI = 0xFB,               // EI (same as unprefiYed)
    FD_CALL_M_NN = 0xFC,        // CALL M, NN (same as unprefiYed)
    FD_PREFIY_FD = 0xFD,        // FD prefiY (same as unprefiYed)
    FD_CP_A_N = 0xFE,           // CP A, n (same as unprefiYed)
    FD_RST_38 = 0xFF            // RST 38 (same as unprefiYed)
};

void z80_execute_fd_instruction(struct z80_t *cpu, uint8_t opcode)
{
    switch (opcode)
    {
        case FD_ADD_IY_BC: cpu->registers.IY = z80_op_add16(cpu, cpu->registers.IY, cpu->registers.BC); break;
        case FD_ADD_IY_DE: cpu->registers.IY = z80_op_add16(cpu, cpu->registers.IY, cpu->registers.DE); break;
        case FD_ADD_IY_IY: cpu->registers.IY = z80_op_add16(cpu, cpu->registers.IY, cpu->registers.IY); break;
        case FD_ADD_IY_SP: cpu->registers.IY = z80_op_add16(cpu, cpu->registers.IY, cpu->registers.SP); break;
        case FD_LD_IY_NN:  cpu->registers.IY = z80_fetch16(cpu); break;
        case FD_INC_IY: cpu->registers.IY++; break;
        case FD_DEC_IY: cpu->registers.IY--; break;
        case FD_DEC_IYH: cpu->registers.IYH = z80_op_dec8(cpu, cpu->registers.IYH); break;
        case FD_DEC_IYL: cpu->registers.IYL = z80_op_dec8(cpu, cpu->registers.IYL); break;
        case FD_INC_IYH: cpu->registers.IYH = z80_op_inc8(cpu, cpu->registers.IYH); break;
        case FD_INC_IYL: cpu->registers.IYL = z80_op_inc8(cpu, cpu->registers.IYL); break;
        case FD_INC_IY_D:
        {
            int8_t offset = (int8_t)z80_fetch8(cpu);
            uint16_t address = cpu->registers.IY + offset;
            uint8_t original = z80_read8(cpu, address);
            uint8_t result = z80_op_inc8(cpu, original);
            cpu->cycle_count++;
            z80_write8(cpu, address, result);
            break;
        }
        case FD_DEC_IY_D:
        {
            int8_t offset = (int8_t)z80_fetch8(cpu);
            uint16_t address = cpu->registers.IY + offset;
            uint8_t original = z80_read8(cpu, address);
            uint8_t result = z80_op_dec8(cpu, original);
            cpu->cycle_count++;
            z80_write8(cpu, address, result);
            break;
        }
        case FD_LD_IY_D_N:
        {
            int8_t d = (int8_t)z80_fetch8(cpu); // displacement
            uint8_t n = z80_fetch8(cpu);        // immediate value
            z80_write8(cpu, cpu->registers.IY + d, n);
            break;
        }
        case FD_LD_B_B:    cpu->registers.B = cpu->registers.B; break;
        case FD_LD_B_C:    cpu->registers.B = cpu->registers.C; break;
        case FD_LD_B_D:    cpu->registers.B = cpu->registers.D; break;
        case FD_LD_B_E:    cpu->registers.B = cpu->registers.E; break;
        case FD_LD_B_IYH:  cpu->registers.B = cpu->registers.IYH; break;
        case FD_LD_B_IYL:  cpu->registers.B = cpu->registers.IYL; break;
        case FD_LD_B_A:    cpu->registers.B = cpu->registers.A; break;
        case FD_LD_C_B:    cpu->registers.C = cpu->registers.B; break;
        case FD_LD_C_C:    cpu->registers.C = cpu->registers.C; break;
        case FD_LD_C_D:    cpu->registers.C = cpu->registers.D; break;
        case FD_LD_C_E:    cpu->registers.C = cpu->registers.E; break;
        case FD_LD_C_IYH:  cpu->registers.C = cpu->registers.IYH; break;
        case FD_LD_C_IYL:  cpu->registers.C = cpu->registers.IYL; break;
        case FD_LD_C_A:    cpu->registers.C = cpu->registers.A; break;
        case FD_LD_D_B:    cpu->registers.D = cpu->registers.B; break;
        case FD_LD_D_C:    cpu->registers.D = cpu->registers.C; break;
        case FD_LD_D_D:    cpu->registers.D = cpu->registers.D; break;
        case FD_LD_D_E:    cpu->registers.D = cpu->registers.E; break;
        case FD_LD_D_IYH:  cpu->registers.D = cpu->registers.IYH; break;
        case FD_LD_D_IYL:  cpu->registers.D = cpu->registers.IYL; break;
        case FD_LD_D_A:    cpu->registers.D = cpu->registers.A; break;
        case FD_LD_E_B:    cpu->registers.E = cpu->registers.B; break;
        case FD_LD_E_C:    cpu->registers.E = cpu->registers.C; break;
        case FD_LD_E_D:    cpu->registers.E = cpu->registers.D; break;
        case FD_LD_E_E:    cpu->registers.E = cpu->registers.E; break;
        case FD_LD_E_IYH:  cpu->registers.E = cpu->registers.IYH; break;
        case FD_LD_E_IYL:  cpu->registers.E = cpu->registers.IYL; break;
        case FD_LD_E_A:    cpu->registers.E = cpu->registers.A; break;
        case FD_LD_IYH_B:  cpu->registers.IYH = cpu->registers.B; break;
        case FD_LD_IYH_C:  cpu->registers.IYH = cpu->registers.C; break;
        case FD_LD_IYH_D:  cpu->registers.IYH = cpu->registers.D; break;
        case FD_LD_IYH_E:  cpu->registers.IYH = cpu->registers.E; break;
        case FD_LD_IYH_IYH: cpu->registers.IYH = cpu->registers.IYH; break;
        case FD_LD_IYH_IYL: cpu->registers.IYH = cpu->registers.IYL; break;
        case FD_LD_IYH_A:  cpu->registers.IYH = cpu->registers.A; break;
        case FD_LD_IYL_B:  cpu->registers.IYL = cpu->registers.B; break;
        case FD_LD_IYL_C:  cpu->registers.IYL = cpu->registers.C; break;
        case FD_LD_IYL_D:  cpu->registers.IYL = cpu->registers.D; break;
        case FD_LD_IYL_E:  cpu->registers.IYL = cpu->registers.E; break;
        case FD_LD_IYL_IYH: cpu->registers.IYL = cpu->registers.IYH; break;
        case FD_LD_IYL_IYL: cpu->registers.IYL = cpu->registers.IYL; break;
        case FD_LD_IYL_A:  cpu->registers.IYL = cpu->registers.A; break;
        case FD_LD_IY_D_B: z80_write8(cpu, cpu->registers.IY + (int8_t)z80_fetch8(cpu), cpu->registers.B); break;
        case FD_LD_IY_D_C: z80_write8(cpu, cpu->registers.IY + (int8_t)z80_fetch8(cpu), cpu->registers.C); break;
        case FD_LD_IY_D_D: z80_write8(cpu, cpu->registers.IY + (int8_t)z80_fetch8(cpu), cpu->registers.D); break;
        case FD_LD_IY_D_E: z80_write8(cpu, cpu->registers.IY + (int8_t)z80_fetch8(cpu), cpu->registers.E); break;
        case FD_LD_IY_D_H: z80_write8(cpu, cpu->registers.IY + (int8_t)z80_fetch8(cpu), cpu->registers.H); break;
        case FD_LD_IY_D_L: z80_write8(cpu, cpu->registers.IY + (int8_t)z80_fetch8(cpu), cpu->registers.L); break;
        case FD_LD_IY_D_A: z80_write8(cpu, cpu->registers.IY + (int8_t)z80_fetch8(cpu), cpu->registers.A); break;
        case FD_LD_A_B:    cpu->registers.A = cpu->registers.B; break;
        case FD_LD_A_C:    cpu->registers.A = cpu->registers.C; break;
        case FD_LD_A_D:    cpu->registers.A = cpu->registers.D; break;
        case FD_LD_A_E:    cpu->registers.A = cpu->registers.E; break;
        case FD_LD_A_IYH:  cpu->registers.A = cpu->registers.IYH; break;
        case FD_LD_A_IYL:  cpu->registers.A = cpu->registers.IYL; break;
        case FD_LD_A_A:    cpu->registers.A = cpu->registers.A; break;
        case FD_LD_IYH_N:  cpu->registers.IYH  = z80_fetch8(cpu); break;
        case FD_LD_IYL_N:  cpu->registers.IYL = z80_fetch8(cpu); break;
        case FD_LD_B_IY_D: cpu->registers.B = z80_read8(cpu, (cpu->registers.IY + (int8_t)z80_fetch8(cpu))); break;
        case FD_LD_C_IY_D: cpu->registers.C = z80_read8(cpu, (cpu->registers.IY + (int8_t)z80_fetch8(cpu))); break;
        case FD_LD_D_IY_D: cpu->registers.D = z80_read8(cpu, (cpu->registers.IY + (int8_t)z80_fetch8(cpu))); break;
        case FD_LD_E_IY_D: cpu->registers.E = z80_read8(cpu, (cpu->registers.IY + (int8_t)z80_fetch8(cpu))); break;
        case FD_LD_H_IY_D: cpu->registers.H = z80_read8(cpu, (cpu->registers.IY + (int8_t)z80_fetch8(cpu))); break;
        case FD_LD_L_IY_D: cpu->registers.L = z80_read8(cpu, (cpu->registers.IY + (int8_t)z80_fetch8(cpu))); break;
        case FD_LD_NN_IY: z80_write16(cpu, z80_fetch16(cpu), cpu->registers.IY); break;
        case FD_LD_IY_NN_ind: cpu->registers.IY = z80_read16(cpu, z80_fetch16(cpu)); break;
        case FD_LD_A_IY_D: cpu->registers.A = z80_read8(cpu, (cpu->registers.IY + (int8_t)z80_fetch8(cpu))); break;
        case FD_POP_IY: cpu->registers.IY = z80_stack_pop16(cpu); break;
        case FD_PUSH_IY: z80_stack_push16(cpu, cpu->registers.IY); break;
        case FD_JP_IY:  cpu->registers.PC = cpu->registers.IY; break;
        case FD_ADD_A_IYH:   z80_op_add8(cpu, cpu->registers.IYH); break;
        case FD_ADD_A_IYL:   z80_op_add8(cpu, cpu->registers.IYL); break;
        case FD_ADD_A_IY_D:  z80_op_add8(cpu, z80_read8(cpu, cpu->registers.IY + (int8_t)z80_fetch8(cpu))); break;
        case FD_ADC_A_IYH:   z80_op_adc8(cpu, cpu->registers.IYH); break;
        case FD_ADC_A_IYL:   z80_op_adc8(cpu, cpu->registers.IYL); break;
        case FD_ADC_A_IY_D:  z80_op_adc8(cpu, z80_read8(cpu, cpu->registers.IY + (int8_t)z80_fetch8(cpu))); break;
        case FD_SUB_A_IYH:   z80_op_sub8(cpu, cpu->registers.IYH); break;
        case FD_SUB_A_IYL:   z80_op_sub8(cpu, cpu->registers.IYL); break;
        case FD_SUB_A_IY_D:  z80_op_sub8(cpu, z80_read8(cpu, cpu->registers.IY + (int8_t)z80_fetch8(cpu))); break;
        case FD_SBC_A_IYH:   z80_op_sbc8(cpu, cpu->registers.IYH); break;
        case FD_SBC_A_IYL:   z80_op_sbc8(cpu, cpu->registers.IYL); break;
        case FD_SBC_A_IY_D:  z80_op_sbc8(cpu, z80_read8(cpu, cpu->registers.IY + (int8_t)z80_fetch8(cpu))); break;
        case FD_AND_A_IYH:   z80_op_and(cpu, cpu->registers.IYH); break;
        case FD_AND_A_IYL:   z80_op_and(cpu, cpu->registers.IYL); break;
        case FD_AND_A_IY_D:  z80_op_and(cpu, z80_read8(cpu, cpu->registers.IY + (int8_t)z80_fetch8(cpu))); break;
        case FD_XOR_A_IYH:   z80_op_xor(cpu, cpu->registers.IYH); break;
        case FD_XOR_A_IYL:   z80_op_xor(cpu, cpu->registers.IYL); break;
        case FD_XOR_A_IY_D:  z80_op_xor(cpu, z80_read8(cpu, cpu->registers.IY + (int8_t)z80_fetch8(cpu))); break;
        case FD_OR_A_IYH:    z80_op_or(cpu, cpu->registers.IYH); break;
        case FD_OR_A_IYL:    z80_op_or(cpu, cpu->registers.IYL); break;
        case FD_OR_A_IY_D:   z80_op_or(cpu, z80_read8(cpu, cpu->registers.IY + (int8_t)z80_fetch8(cpu))); break;
        case FD_CP_A_IYH:    z80_op_cp(cpu, cpu->registers.A, cpu->registers.IYH); break;
        case FD_CP_A_IYL:    z80_op_cp(cpu, cpu->registers.A, cpu->registers.IYL); break;
        case FD_CP_A_IY_D:   z80_op_cp(cpu, cpu->registers.A, z80_read8(cpu, cpu->registers.IY + (int8_t)z80_fetch8(cpu))); break;
        case FD_PREFIY_CB:   z80_execute_fd_cb_instruction(cpu, z80_fetch8(cpu)); break;
        default:
            printf("Unimplemented 0xFD Instruction: 0x%02X\n", opcode);
            cpu->running = false;
            cpu->halted = true;
            break;
    }
}