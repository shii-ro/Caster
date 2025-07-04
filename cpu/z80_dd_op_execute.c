#include <stdio.h>
#include "z80_op.h"
#include "z80_flags.h"

// Z80 DD-prefixed Opcode enumeration (complete set)
enum z80_dd_opcodes
{
    // 16-BIT ARITHMETIC - ADD IX with register pairs
    DD_ADD_IX_BC = 0x09,    // ADD IX, BC
    DD_ADD_IX_DE = 0x19,    // ADD IX, DE
    DD_ADD_IX_IX = 0x29,    // ADD IX, IX
    DD_ADD_IX_SP = 0x39,    // ADD IX, SP
    
    // 16-BIT LOAD INSTRUCTIONS
    DD_LD_IX_NN = 0x21,     // LD IX, NN
    DD_LD_NN_IX = 0x22,     // LD (NN), IX
    DD_LD_IX_NN_ind = 0x2A, // LD IX, (NN)
    
    // 16-BIT INCREMENT/DECREMENT
    DD_INC_IX = 0x23,       // INC IX
    DD_DEC_IX = 0x2B,       // DEC IX
    
    // 8-BIT INCREMENT/DECREMENT - (IX+d)
    DD_INC_IX_D = 0x34,     // INC (IX+d)
    DD_DEC_IX_D = 0x35,     // DEC (IX+d)
    
    // 8-BIT LOAD INSTRUCTIONS - Immediate to (IX+d)
    DD_LD_IX_D_n = 0x36,    // LD (IX+d), n
    
    // 8-BIT LOAD INSTRUCTIONS - Register to/from IXH, IXL
    DD_LD_B_IXH = 0x44,     // LD B, IXH
    DD_LD_B_IXL = 0x45,     // LD B, IXL
    DD_LD_C_IXH = 0x4C,     // LD C, IXH
    DD_LD_C_IXL = 0x4D,     // LD C, IXL
    DD_LD_D_IXH = 0x54,     // LD D, IXH
    DD_LD_D_IXL = 0x55,     // LD D, IXL
    DD_LD_E_IXH = 0x5C,     // LD E, IXH
    DD_LD_E_IXL = 0x5D,     // LD E, IXL
    
    DD_LD_IXH_B = 0x60,     // LD IXH, B
    DD_LD_IXH_C = 0x61,     // LD IXH, C
    DD_LD_IXH_D = 0x62,     // LD IXH, D
    DD_LD_IXH_E = 0x63,     // LD IXH, E
    DD_LD_IXH_IXH = 0x64,   // LD IXH, IXH
    DD_LD_IXH_IXL = 0x65,   // LD IXH, IXL
    DD_LD_H_IX_D = 0x66,    // LD H, (IX+d)
    DD_LD_IXH_A = 0x67,     // LD IXH, A
    
    DD_LD_IXL_B = 0x68,     // LD IXL, B
    DD_LD_IXL_C = 0x69,     // LD IXL, C
    DD_LD_IXL_D = 0x6A,     // LD IXL, D
    DD_LD_IXL_E = 0x6B,     // LD IXL, E
    DD_LD_IXL_IXH = 0x6C,   // LD IXL, IXH
    DD_LD_IXL_IXL = 0x6D,   // LD IXL, IXL
    DD_LD_L_IX_D = 0x6E,    // LD L, (IX+d)
    DD_LD_IXL_A = 0x6F,     // LD IXL, A
    
    // 8-BIT LOAD INSTRUCTIONS - Register to/from (IX+d)
    DD_LD_IX_D_B = 0x70,    // LD (IX+d), B
    DD_LD_IX_D_C = 0x71,    // LD (IX+d), C
    DD_LD_IX_D_D = 0x72,    // LD (IX+d), D
    DD_LD_IX_D_E = 0x73,    // LD (IX+d), E
    DD_LD_IX_D_H = 0x74,    // LD (IX+d), H
    DD_LD_IX_D_L = 0x75,    // LD (IX+d), L
    DD_LD_IX_D_A = 0x77,    // LD (IX+d), A
    
    DD_LD_A_IXH = 0x7C,     // LD A, IXH
    DD_LD_A_IXL = 0x7D,     // LD A, IXL
    DD_LD_A_IX_D = 0x7E,    // LD A, (IX+d)
    
    // 8-BIT ARITHMETIC - ADD
    DD_ADD_A_IXH = 0x84,    // ADD A, IXH
    DD_ADD_A_IXL = 0x85,    // ADD A, IXL
    DD_ADD_A_IX_D = 0x86,   // ADD A, (IX+d)
    
    // 8-BIT ARITHMETIC - ADC (Add with Carry)
    DD_ADC_A_IXH = 0x8C,    // ADC A, IXH
    DD_ADC_A_IXL = 0x8D,    // ADC A, IXL
    DD_ADC_A_IX_D = 0x8E,   // ADC A, (IX+d)
    
    // 8-BIT ARITHMETIC - SUB
    DD_SUB_A_IXH = 0x94,    // SUB A, IXH
    DD_SUB_A_IXL = 0x95,    // SUB A, IXL
    DD_SUB_A_IX_D = 0x96,   // SUB A, (IX+d)
    
    // 8-BIT ARITHMETIC - SBC (Subtract with Carry)
    DD_SBC_A_IXH = 0x9C,    // SBC A, IXH
    DD_SBC_A_IXL = 0x9D,    // SBC A, IXL
    DD_SBC_A_IX_D = 0x9E,   // SBC A, (IX+d)
    
    // 8-BIT LOGICAL - AND
    DD_AND_A_IXH = 0xA4,    // AND A, IXH
    DD_AND_A_IXL = 0xA5,    // AND A, IXL
    DD_AND_A_IX_D = 0xA6,   // AND A, (IX+d)
    
    // 8-BIT LOGICAL - XOR
    DD_XOR_A_IXH = 0xAC,    // XOR A, IXH
    DD_XOR_A_IXL = 0xAD,    // XOR A, IXL
    DD_XOR_A_IX_D = 0xAE,   // XOR A, (IX+d)
    
    // 8-BIT LOGICAL - OR
    DD_OR_A_IXH = 0xB4,     // OR A, IXH
    DD_OR_A_IXL = 0xB5,     // OR A, IXL
    DD_OR_A_IX_D = 0xB6,    // OR A, (IX+d)
    
    // 8-BIT ARITHMETIC - CP (Compare)
    DD_CP_A_IXH = 0xBC,     // CP A, IXH
    DD_CP_A_IXL = 0xBD,     // CP A, IXL
    DD_CP_A_IX_D = 0xBE,    // CP A, (IX+d)
    
    // STACK OPERATIONS
    DD_POP_IX = 0xE1,       // POP IX
    DD_EX_SP_IX = 0xE3,     // EX (SP), IX
    DD_PUSH_IX = 0xE5,      // PUSH IX
    
    // JUMP INSTRUCTIONS
    DD_JP_IX = 0xE9,        // JP (IX)
    
    // PREFIX INSTRUCTIONS for bit operations
    DD_PREFIX_CB = 0xCB,    // DD CB prefix (bit operations on (IX+d))
    
    // 8-BIT IMMEDIATE LOAD to IXH/IXL
    DD_LD_IXH_n = 0x26,     // LD IXH, n
    DD_LD_IXL_n = 0x2E      // LD IXL, n
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