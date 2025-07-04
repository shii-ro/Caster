#include <stdio.h>
#include "z80_op.h"
#include "z80_flags.h"

// Z80 FD (IY) prefix instruction opcodes
enum z80_fd_opcodes
{
    // 16-BIT ARITHMETIC - ADD IY with register pairs
    FD_ADD_IY_BC = 0x09,    // ADD IY, BC
    FD_ADD_IY_DE = 0x19,    // ADD IY, DE
    FD_ADD_IY_IY = 0x29,    // ADD IY, IY
    FD_ADD_IY_SP = 0x39,    // ADD IY, SP
    
    // 16-BIT LOAD OPERATIONS
    FD_LD_IY_NN = 0x21,     // LD IY, nn
    FD_LD_NN_IY = 0x22,     // LD (nn), IY
    FD_LD_IY_NN_IND = 0x2A, // LD IY, (nn)
    
    // 16-BIT INCREMENT/DECREMENT
    FD_INC_IY = 0x23,       // INC IY
    FD_DEC_IY = 0x2B,       // DEC IY
    
    // 8-BIT LOAD OPERATIONS WITH DISPLACEMENT
    FD_LD_B_IY_D = 0x46,    // LD B, (IY+d)
    FD_LD_C_IY_D = 0x4E,    // LD C, (IY+d)
    FD_LD_D_IY_D = 0x56,    // LD D, (IY+d)
    FD_LD_E_IY_D = 0x5E,    // LD E, (IY+d)
    FD_LD_H_IY_D = 0x66,    // LD H, (IY+d)
    FD_LD_L_IY_D = 0x6E,    // LD L, (IY+d)
    FD_LD_A_IY_D = 0x7E,    // LD A, (IY+d)
    
    FD_LD_IY_D_B = 0x70,    // LD (IY+d), B
    FD_LD_IY_D_C = 0x71,    // LD (IY+d), C
    FD_LD_IY_D_D = 0x72,    // LD (IY+d), D
    FD_LD_IY_D_E = 0x73,    // LD (IY+d), E
    FD_LD_IY_D_H = 0x74,    // LD (IY+d), H
    FD_LD_IY_D_L = 0x75,    // LD (IY+d), L
    FD_LD_IY_D_A = 0x77,    // LD (IY+d), A
    FD_LD_IY_D_N = 0x36,    // LD (IY+d), n
    
    // 8-BIT ARITHMETIC WITH DISPLACEMENT
    FD_ADD_A_IY_D = 0x86,   // ADD A, (IY+d)
    FD_ADC_A_IY_D = 0x8E,   // ADC A, (IY+d)
    FD_SUB_A_IY_D = 0x96,     // SUB (IY+d)
    FD_SBC_A_IY_D = 0x9E,   // SBC A, (IY+d)
    FD_AND_A_IY_D = 0xA6,     // AND (IY+d)
    FD_XOR_A_IY_D = 0xAE,     // XOR (IY+d)
    FD_OR_A_IY_D = 0xB6,      // OR (IY+d)
    FD_CP_A_IY_D = 0xBE,      // CP (IY+d)
    
    // 8-BIT INCREMENT/DECREMENT WITH DISPLACEMENT
    FD_INC_IY_D = 0x34,     // INC (IY+d)
    FD_DEC_IY_D = 0x35,     // DEC (IY+d)
    
    // STACK OPERATIONS
    FD_PUSH_IY = 0xE5,      // PUSH IY
    FD_POP_IY = 0xE1,       // POP IY
    
    // JUMP OPERATIONS
    FD_JP_IY = 0xE9,        // JP (IY)
    
    // EXCHANGE OPERATIONS
    FD_EX_SP_IY = 0xE3,     // EX (SP), IY
    
    // IY HIGH/LOW REGISTER OPERATIONS
    FD_LD_IYH_N = 0x26,     // LD IYH, n
    FD_LD_IYL_N = 0x2E,     // LD IYL, n
    FD_LD_B_IYH = 0x44,     // LD B, IYH
    FD_LD_B_IYL = 0x45,     // LD B, IYL
    FD_LD_C_IYH = 0x4C,     // LD C, IYH
    FD_LD_C_IYL = 0x4D,     // LD C, IYL
    FD_LD_D_IYH = 0x54,     // LD D, IYH
    FD_LD_D_IYL = 0x55,     // LD D, IYL
    FD_LD_E_IYH = 0x5C,     // LD E, IYH
    FD_LD_E_IYL = 0x5D,     // LD E, IYL
    FD_LD_IYH_B = 0x60,     // LD IYH, B
    FD_LD_IYH_C = 0x61,     // LD IYH, C
    FD_LD_IYH_D = 0x62,     // LD IYH, D
    FD_LD_IYH_E = 0x63,     // LD IYH, E
    FD_LD_IYH_IYH = 0x64,   // LD IYH, IYH
    FD_LD_IYH_IYL = 0x65,   // LD IYH, IYL
    FD_LD_IYH_A = 0x67,     // LD IYH, A
    FD_LD_IYL_B = 0x68,     // LD IYL, B
    FD_LD_IYL_C = 0x69,     // LD IYL, C
    FD_LD_IYL_D = 0x6A,     // LD IYL, D
    FD_LD_IYL_E = 0x6B,     // LD IYL, E
    FD_LD_IYL_IYH = 0x6C,   // LD IYL, IYH
    FD_LD_IYL_IYL = 0x6D,   // LD IYL, IYL
    FD_LD_IYL_A = 0x6F,     // LD IYL, A
    FD_LD_A_IYH = 0x7C,     // LD A, IYH
    FD_LD_A_IYL = 0x7D,     // LD A, IYL
    
    // ARITHMETIC WITH IY HIGH/LOW
    FD_ADD_A_IYH = 0x84,    // ADD A, IYH
    FD_ADD_A_IYL = 0x85,    // ADD A, IYL
    FD_ADC_A_IYH = 0x8C,    // ADC A, IYH
    FD_ADC_A_IYL = 0x8D,    // ADC A, IYL
    FD_SUB_A_IYH = 0x94,      // SUB IYH
    FD_SUB_A_IYL = 0x95,      // SUB IYL
    FD_SBC_A_IYH = 0x9C,    // SBC A, IYH
    FD_SBC_A_IYL = 0x9D,    // SBC A, IYL
    FD_AND_A_IYH = 0xA4,      // AND IYH
    FD_AND_A_IYL = 0xA5,      // AND IYL
    FD_XOR_A_IYH = 0xAC,      // XOR IYH
    FD_XOR_A_IYL = 0xAD,      // XOR IYL
    FD_OR_A_IYH = 0xB4,       // OR IYH
    FD_OR_A_IYL = 0xB5,       // OR IYL
    FD_CP_A_IYH = 0xBC,       // CP IYH
    FD_CP_A_IYL = 0xBD,       // CP IYL
    
    // INCREMENT/DECREMENT IY HIGH/LOW
    FD_INC_IYH = 0x24,      // INC IYH
    FD_INC_IYL = 0x2C,      // INC IYL
    FD_DEC_IYH = 0x25,      // DEC IYH
    FD_DEC_IYL = 0x2D,       // DEC IYL

    // PREFIX INSTRUCTIONS for bit operations
    FD_PREFIX_CB = 0xCB,    
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
        case FD_PREFIX_CB:   z80_execute_fd_cb_instruction(cpu, z80_fetch8(cpu)); break;
        default:
            printf("Unimplemented 0xFD Instruction: 0x%02X\n", opcode);
            cpu->running = false;
            cpu->halted = true;
            break;
    }
}