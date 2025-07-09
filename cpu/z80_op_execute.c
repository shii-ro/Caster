#include <stdio.h>
#include "z80_op.h"
#include "z80_flags.h"

// Debug tracking
uint16_t last_sp_before_inc_dec_e = 0;
uint16_t last_pc_before_inc_dec_e = 0;
bool debug_inc_dec_e = false;

enum z80_opcodes
{
    // CONTROL INSTRUCTIONS
    NOP = 0x00,
    HALT = 0x76,
    DI = 0xF3,
    EI = 0xFB,
    
    // 8-BIT LOAD INSTRUCTIONS - Immediate
    LD_B_n = 0x06,
    LD_C_n = 0x0E,
    LD_D_n = 0x16,
    LD_E_n = 0x1E,
    LD_H_n = 0x26,
    LD_L_n = 0x2E,
    LD_A_n = 0x3E,
    LD_HL_n = 0x36,
    
    // 8-BIT LOAD INSTRUCTIONS - Register to Register
    LD_B_B = 0x40, LD_B_C = 0x41, LD_B_D = 0x42, LD_B_E = 0x43,
    LD_B_H = 0x44, LD_B_L = 0x45, LD_B_HL = 0x46, LD_B_A = 0x47,
    LD_C_B = 0x48, LD_C_C = 0x49, LD_C_D = 0x4A, LD_C_E = 0x4B,
    LD_C_H = 0x4C, LD_C_L = 0x4D, LD_C_HL = 0x4E, LD_C_A = 0x4F,
    LD_D_B = 0x50, LD_D_C = 0x51, LD_D_D = 0x52, LD_D_E = 0x53,
    LD_D_H = 0x54, LD_D_L = 0x55, LD_D_HL = 0x56, LD_D_A = 0x57,
    LD_E_B = 0x58, LD_E_C = 0x59, LD_E_D = 0x5A, LD_E_E = 0x5B,
    LD_E_H = 0x5C, LD_E_L = 0x5D, LD_E_HL = 0x5E, LD_E_A = 0x5F,
    LD_H_B = 0x60, LD_H_C = 0x61, LD_H_D = 0x62, LD_H_E = 0x63,
    LD_H_H = 0x64, LD_H_L = 0x65, LD_H_HL = 0x66, LD_H_A = 0x67,
    LD_L_B = 0x68, LD_L_C = 0x69, LD_L_D = 0x6A, LD_L_E = 0x6B,
    LD_L_H = 0x6C, LD_L_L = 0x6D, LD_L_HL = 0x6E, LD_L_A = 0x6F,
    LD_HL_B = 0x70, LD_HL_C = 0x71, LD_HL_D = 0x72, LD_HL_E = 0x73,
    LD_HL_H = 0x74, LD_HL_L = 0x75, LD_HL_A = 0x77,
    LD_A_B = 0x78, LD_A_C = 0x79, LD_A_D = 0x7A, LD_A_E = 0x7B,
    LD_A_H = 0x7C, LD_A_L = 0x7D, LD_A_HL = 0x7E, LD_A_A = 0x7F,
    
    // 8-BIT LOAD INSTRUCTIONS - Indirect
    LD_BC_A = 0x02, LD_DE_A = 0x12, LD_A_BC = 0x0A, LD_A_DE = 0x1A,
    LD_nn_A = 0x32, LD_A_nn = 0x3A,
    
    // 16-BIT LOAD INSTRUCTIONS
    LD_BC_nn = 0x01, LD_DE_nn = 0x11, LD_HL_nn = 0x21, LD_SP_nn = 0x31,
    LD_nn_HL = 0x22, LD_HL_nn_ind = 0x2A, LD_SP_HL = 0xF9,
    
    // 8-BIT INCREMENT/DECREMENT  // 16-BIT INCREMENT/DECREMENT
    INC_BC = 0x03, INC_B = 0x04, DEC_B = 0x05, DEC_BC = 0x0B, INC_C = 0x0C, DEC_C = 0x0D,
    INC_DE = 0x13, INC_D = 0x14, DEC_D = 0x15, DEC_DE = 0x1B, INC_E = 0x1C, DEC_E = 0x1D,
    INC_HL = 0x23, INC_H = 0x24, DEC_H = 0x25, DEC_HL = 0x2B, INC_L = 0x2C, DEC_L = 0x2D,
    INC_SP = 0x33, INC_HL_ = 0x34, DEC_HL_ = 0x35, DEC_SP = 0x3B, INC_A = 0x3C, DEC_A = 0x3D, 
       
    // 8-BIT ARITHMETIC
    ADD_A_B = 0x80, ADD_A_C = 0x81, ADD_A_D = 0x82, ADD_A_E = 0x83,
    ADD_A_H = 0x84, ADD_A_L = 0x85, ADD_A_HL = 0x86, ADD_A_A = 0x87,
    ADD_A_n = 0xC6,
    ADC_A_B = 0x88, ADC_A_C = 0x89, ADC_A_D = 0x8A, ADC_A_E = 0x8B,
    ADC_A_H = 0x8C, ADC_A_L = 0x8D, ADC_A_HL = 0x8E, ADC_A_A = 0x8F,
    ADC_A_n = 0xCE,
    SUB_B = 0x90, SUB_C = 0x91, SUB_D = 0x92, SUB_E = 0x93,
    SUB_H = 0x94, SUB_L = 0x95, SUB_HL = 0x96, SUB_A = 0x97,
    SUB_n = 0xD6,
    SBC_A_B = 0x98, SBC_A_C = 0x99, SBC_A_D = 0x9A, SBC_A_E = 0x9B,
    SBC_A_H = 0x9C, SBC_A_L = 0x9D, SBC_A_HL = 0x9E, SBC_A_A = 0x9F,
    SBC_A_n = 0xDE,
    
    // 16-BIT ARITHMETIC
    ADD_HL_BC = 0x09, ADD_HL_DE = 0x19, ADD_HL_HL = 0x29, ADD_HL_SP = 0x39,
    
    // LOGICAL OPERATIONS
    AND_B = 0xA0, AND_C = 0xA1, AND_D = 0xA2, AND_E = 0xA3,
    AND_H = 0xA4, AND_L = 0xA5, AND_HL = 0xA6, AND_A = 0xA7, AND_n = 0xE6,
    XOR_B = 0xA8, XOR_C = 0xA9, XOR_D = 0xAA, XOR_E = 0xAB,
    XOR_H = 0xAC, XOR_L = 0xAD, XOR_HL = 0xAE, XOR_A = 0xAF, XOR_n = 0xEE,
    OR_B = 0xB0, OR_C = 0xB1, OR_D = 0xB2, OR_E = 0xB3,
    OR_H = 0xB4, OR_L = 0xB5, OR_HL = 0xB6, OR_A = 0xB7, OR_n = 0xF6,
    
    // COMPARE OPERATIONS
    CP_B = 0xB8, CP_C = 0xB9, CP_D = 0xBA, CP_E = 0xBB,
    CP_H = 0xBC, CP_L = 0xBD, CP_HL = 0xBE, CP_A = 0xBF, CP_n = 0xFE,
    
    // ROTATE AND SHIFT OPERATIONS
    RLCA = 0x07, RRCA = 0x0F, RRA = 0x1F, RLA = 0x17,
    
    // MISCELLANEOUS OPERATIONS
    DAA = 0x27,  // Decimal Adjust Accumulator
    CPL = 0x2F,  // Complement
    SCF = 0x37,  // Set Carry Flag
    CCF = 0x3F,  // Complement Carry Flag
    
    // JUMP INSTRUCTIONS
    JP_NZ_nn = 0xC2, JP_nn = 0xC3, JP_Z_nn = 0xCA,
    JP_NC_nn = 0xD2, JP_C_nn = 0xDA,
    JP_PO_nn = 0xE2, JP_HL = 0xE9, JP_PE_nn = 0xEA,
    JP_P_nn = 0xF2, JP_M_nn = 0xFA,
    JR_d = 0x18, JR_NZ_d = 0x20, JR_Z_d = 0x28, JR_NC_d = 0x30, JR_C_d = 0x38,
    DJNZ_e = 0x10,
    
    // CALL INSTRUCTIONS
    CALL_nn = 0xCD, CALL_NZ_nn = 0xC4, CALL_Z_nn = 0xCC,
    CALL_NC_nn = 0xD4, CALL_C_nn = 0xDC,
    CALL_PO_nn = 0xE4, CALL_PE_nn = 0xEC,
    CALL_P_nn = 0xF4, CALL_M_nn = 0xFC,
    
    // RETURN INSTRUCTIONS
    RET = 0xC9, RET_NZ = 0xC0, RET_Z = 0xC8,
    RET_NC = 0xD0, RET_C = 0xD8,
    RET_PO = 0xE0, RET_PE = 0xE8,
    RET_P = 0xF0, RET_M = 0xF8,
    
    // RESTART INSTRUCTIONS
    RST_00 = 0xC7, RST_08 = 0xCF, RST_10 = 0xD7, RST_18 = 0xDF,
    RST_20 = 0xE7, RST_28 = 0xEF, RST_30 = 0xF7, RST_38 = 0xFF,
    
    // STACK OPERATIONS
    POP_BC = 0xC1, POP_DE = 0xD1, POP_HL = 0xE1, POP_AF = 0xF1,
    PUSH_BC = 0xC5, PUSH_DE = 0xD5, PUSH_HL = 0xE5, PUSH_AF = 0xF5,
    
    // EXCHANGE INSTRUCTIONS
    EX_AF_AF = 0x08, EXX = 0xD9, EX_DE_HL = 0xEB, EX_SP_HL = 0xE3,
    
    // I/O INSTRUCTIONS
    IN_A_n = 0xDB,   // Input from port n to A
    OUT_n_A = 0xD3,  // Output A to port n
    
    // PREFIX INSTRUCTIONS
    DD_PREFIX = 0xDD, ED_PREFIX = 0xED, FD_PREFIX = 0xFD, CB_PREFIX = 0xCB
};

void z80_execute_instruction(struct z80_t *cpu, uint8_t opcode)
{
    switch (opcode)
    {
    // === CONTROL INSTRUCTIONS ===
    case NOP: break;
    case HALT: cpu->halted = true; break;
    case DI: cpu->iff1 = cpu->iff2 = false; break;
    case EI: cpu->iff1 = cpu->iff2 = true; break;
    
    // === 8-BIT LOAD INSTRUCTIONS - Immediate ===
    case LD_B_n: cpu->registers.B = z80_fetch8(cpu); break;
    case LD_C_n: cpu->registers.C = z80_fetch8(cpu); break;
    case LD_D_n: cpu->registers.D = z80_fetch8(cpu); break;
    case LD_E_n: cpu->registers.E = z80_fetch8(cpu); break;
    case LD_H_n: cpu->registers.H = z80_fetch8(cpu); break;
    case LD_L_n: cpu->registers.L = z80_fetch8(cpu); break;
    case LD_A_n: cpu->registers.A = z80_fetch8(cpu); break;
    case LD_HL_n: z80_write8(cpu, cpu->registers.HL, z80_fetch8(cpu)); break;
    
    // === 8-BIT LOAD INSTRUCTIONS - Register to Register ===
    case LD_B_B: cpu->registers.B = cpu->registers.B; break;
    case LD_B_C: cpu->registers.B = cpu->registers.C; break;
    case LD_B_D: cpu->registers.B = cpu->registers.D; break;
    case LD_B_E: cpu->registers.B = cpu->registers.E; break;
    case LD_B_H: cpu->registers.B = cpu->registers.H; break;
    case LD_B_L: cpu->registers.B = cpu->registers.L; break;
    case LD_B_HL: cpu->registers.B = z80_read8(cpu, cpu->registers.HL); break;
    case LD_B_A: cpu->registers.B = cpu->registers.A; break;
    case LD_C_B: cpu->registers.C = cpu->registers.B; break;
    case LD_C_C: cpu->registers.C = cpu->registers.C; break;
    case LD_C_D: cpu->registers.C = cpu->registers.D; break;
    case LD_C_E: cpu->registers.C = cpu->registers.E; break;
    case LD_C_H: cpu->registers.C = cpu->registers.H; break;
    case LD_C_L: cpu->registers.C = cpu->registers.L; break;
    case LD_C_HL: cpu->registers.C = z80_read8(cpu, cpu->registers.HL); break;
    case LD_C_A: cpu->registers.C = cpu->registers.A; break;
    case LD_D_B: cpu->registers.D = cpu->registers.B; break;
    case LD_D_C: cpu->registers.D = cpu->registers.C; break;
    case LD_D_D: cpu->registers.D = cpu->registers.D; break;
    case LD_D_E: cpu->registers.D = cpu->registers.E; break;
    case LD_D_H: cpu->registers.D = cpu->registers.H; break;
    case LD_D_L: cpu->registers.D = cpu->registers.L; break;
    case LD_D_HL: cpu->registers.D = z80_read8(cpu, cpu->registers.HL); break;
    case LD_D_A: cpu->registers.D = cpu->registers.A; break;
    case LD_E_B: cpu->registers.E = cpu->registers.B; break;
    case LD_E_C: cpu->registers.E = cpu->registers.C; break;
    case LD_E_D: cpu->registers.E = cpu->registers.D; break;
    case LD_E_E: cpu->registers.E = cpu->registers.E; break;
    case LD_E_H: cpu->registers.E = cpu->registers.H; break;
    case LD_E_L: cpu->registers.E = cpu->registers.L; break;
    case LD_E_HL: cpu->registers.E = z80_read8(cpu, cpu->registers.HL); break;
    case LD_E_A: cpu->registers.E = cpu->registers.A; break;
    case LD_H_B: cpu->registers.H = cpu->registers.B; break;
    case LD_H_C: cpu->registers.H = cpu->registers.C; break;
    case LD_H_D: cpu->registers.H = cpu->registers.D; break;
    case LD_H_E: cpu->registers.H = cpu->registers.E; break;
    case LD_H_H: cpu->registers.H = cpu->registers.H; break;
    case LD_H_L: cpu->registers.H = cpu->registers.L; break;
    case LD_H_HL: cpu->registers.H = z80_read8(cpu, cpu->registers.HL); break;
    case LD_H_A: cpu->registers.H = cpu->registers.A; break;
    case LD_L_B: cpu->registers.L = cpu->registers.B; break;
    case LD_L_C: cpu->registers.L = cpu->registers.C; break;
    case LD_L_D: cpu->registers.L = cpu->registers.D; break;
    case LD_L_E: cpu->registers.L = cpu->registers.E; break;
    case LD_L_H: cpu->registers.L = cpu->registers.H; break;
    case LD_L_L: cpu->registers.L = cpu->registers.L; break;
    case LD_L_HL: cpu->registers.L = z80_read8(cpu, cpu->registers.HL); break;
    case LD_L_A: cpu->registers.L = cpu->registers.A; break;
    case LD_HL_B: z80_write8(cpu, cpu->registers.HL, cpu->registers.B); break;
    case LD_HL_C: z80_write8(cpu, cpu->registers.HL, cpu->registers.C); break;
    case LD_HL_D: z80_write8(cpu, cpu->registers.HL, cpu->registers.D); break;
    case LD_HL_E: z80_write8(cpu, cpu->registers.HL, cpu->registers.E); break;
    case LD_HL_H: z80_write8(cpu, cpu->registers.HL, cpu->registers.H); break;
    case LD_HL_L: z80_write8(cpu, cpu->registers.HL, cpu->registers.L); break;
    case LD_HL_A: z80_write8(cpu, cpu->registers.HL, cpu->registers.A); break;
    case LD_A_B: cpu->registers.A = cpu->registers.B; break;
    case LD_A_C: cpu->registers.A = cpu->registers.C; break;
    case LD_A_D: cpu->registers.A = cpu->registers.D; break;
    case LD_A_E: cpu->registers.A = cpu->registers.E; break;
    case LD_A_H: cpu->registers.A = cpu->registers.H; break;
    case LD_A_L: cpu->registers.A = cpu->registers.L; break;
    case LD_A_HL: cpu->registers.A = z80_read8(cpu, cpu->registers.HL); break;
    case LD_A_A: cpu->registers.A = cpu->registers.A; break;
    
    // === 8-BIT LOAD INSTRUCTIONS - Indirect ===
    case LD_BC_A: z80_write8(cpu, cpu->registers.BC, cpu->registers.A); break;
    case LD_DE_A: z80_write8(cpu, cpu->registers.DE, cpu->registers.A); break;
    case LD_A_BC: cpu->registers.A = z80_read8(cpu, cpu->registers.BC); break;
    case LD_A_DE: cpu->registers.A = z80_read8(cpu, cpu->registers.DE); break;
    case LD_nn_A: z80_write8(cpu, z80_fetch16(cpu), cpu->registers.A); break;
    case LD_A_nn: cpu->registers.A = z80_read8(cpu, z80_fetch16(cpu)); break;
    
    // === 16-BIT LOAD INSTRUCTIONS ===
    case LD_BC_nn: cpu->registers.BC = z80_fetch16(cpu); break;
    case LD_DE_nn: cpu->registers.DE = z80_fetch16(cpu); break;
    case LD_HL_nn: cpu->registers.HL = z80_fetch16(cpu); break;
    case LD_SP_nn: cpu->registers.SP = z80_fetch16(cpu); break;
    case LD_nn_HL: z80_write16(cpu, z80_fetch16(cpu), cpu->registers.HL); break;
    case LD_HL_nn_ind: cpu->registers.HL = z80_read16(cpu, z80_fetch16(cpu)); break;
    case LD_SP_HL: cpu->registers.SP = cpu->registers.HL; break;
    
    // === 8-BIT INCREMENT/DECREMENT ===
    case INC_B: cpu->registers.B = z80_op_inc8(cpu, cpu->registers.B); break;
    case INC_C: cpu->registers.C = z80_op_inc8(cpu, cpu->registers.C); break;
    case INC_D: cpu->registers.D = z80_op_inc8(cpu, cpu->registers.D); break;
    case INC_E: cpu->registers.E = z80_op_inc8(cpu, cpu->registers.E); break;
    case INC_H: cpu->registers.H = z80_op_inc8(cpu, cpu->registers.H); break;
    case INC_L: cpu->registers.L = z80_op_inc8(cpu, cpu->registers.L); break;
    case INC_A: cpu->registers.A = z80_op_inc8(cpu, cpu->registers.A); break;
    case INC_HL_:
        {
            uint8_t original = z80_read8(cpu, cpu->registers.HL);
            uint8_t result = z80_op_inc8(cpu, original);
            cpu->cycle_count++;
            z80_write8(cpu, cpu->registers.HL, result);
        }
        break;
    case DEC_B: cpu->registers.B = z80_op_dec8(cpu, cpu->registers.B); break;
    case DEC_C: cpu->registers.C = z80_op_dec8(cpu, cpu->registers.C); break;
    case DEC_D: cpu->registers.D = z80_op_dec8(cpu, cpu->registers.D); break;
    case DEC_E: cpu->registers.E = z80_op_dec8(cpu, cpu->registers.E); break;
    case DEC_H: cpu->registers.H = z80_op_dec8(cpu, cpu->registers.H); break;
    case DEC_L: cpu->registers.L = z80_op_dec8(cpu, cpu->registers.L); break;
    case DEC_A: cpu->registers.A = z80_op_dec8(cpu, cpu->registers.A); break;
    case DEC_HL_:
        {
            uint8_t original = z80_read8(cpu, cpu->registers.HL);
            uint8_t result = z80_op_dec8(cpu, original);
            cpu->cycle_count++;
            z80_write8(cpu, cpu->registers.HL, result);
        }
        break;
    
    // === 16-BIT INCREMENT/DECREMENT ===
    case INC_BC:   cpu->cycle_count += 2; cpu->registers.BC++; break;
    case INC_DE:   cpu->cycle_count += 2; cpu->registers.DE++; break;
    case INC_HL:   cpu->cycle_count += 2; cpu->registers.HL++; break;
    case INC_SP:   cpu->cycle_count += 2; cpu->registers.SP++; break;
    case DEC_BC:   cpu->cycle_count += 2; cpu->registers.BC--; break;
    case DEC_DE:   cpu->cycle_count += 2; cpu->registers.DE--; break;
    case DEC_HL:   cpu->cycle_count += 2; cpu->registers.HL--; break;
    case DEC_SP:   cpu->cycle_count += 2; cpu->registers.SP--; break;
    
    // === 8-BIT ARITHMETIC ===
    case ADD_A_B:  z80_op_add8(cpu, cpu->registers.B); break;
    case ADD_A_C:  z80_op_add8(cpu, cpu->registers.C); break;
    case ADD_A_D:  z80_op_add8(cpu, cpu->registers.D); break;
    case ADD_A_E:  z80_op_add8(cpu, cpu->registers.E); break;
    case ADD_A_H:  z80_op_add8(cpu, cpu->registers.H); break;
    case ADD_A_L:  z80_op_add8(cpu, cpu->registers.L); break;
    case ADD_A_HL: z80_op_add8(cpu, z80_read8(cpu, cpu->registers.HL)); break;
    case ADD_A_A:  z80_op_add8(cpu, cpu->registers.A); break;
    case ADD_A_n:  z80_op_add8(cpu, z80_fetch8(cpu)); break;
    case ADC_A_B:  z80_op_adc8(cpu, cpu->registers.B); break;
    case ADC_A_C:  z80_op_adc8(cpu, cpu->registers.C); break;
    case ADC_A_D:  z80_op_adc8(cpu, cpu->registers.D); break;
    case ADC_A_E:  z80_op_adc8(cpu, cpu->registers.E); break;
    case ADC_A_H:  z80_op_adc8(cpu, cpu->registers.H); break;
    case ADC_A_L:  z80_op_adc8(cpu, cpu->registers.L); break;
    case ADC_A_HL: z80_op_adc8(cpu, z80_read8(cpu, cpu->registers.HL)); break;
    case ADC_A_A:  z80_op_adc8(cpu, cpu->registers.A); break;
    case ADC_A_n:  z80_op_adc8(cpu, z80_fetch8(cpu)); break;
    case SUB_B:    z80_op_sub8(cpu, cpu->registers.B); break;
    case SUB_C:    z80_op_sub8(cpu, cpu->registers.C); break;
    case SUB_D:    z80_op_sub8(cpu, cpu->registers.D); break;
    case SUB_E:    z80_op_sub8(cpu, cpu->registers.E); break;
    case SUB_H:    z80_op_sub8(cpu, cpu->registers.H); break;
    case SUB_L:    z80_op_sub8(cpu, cpu->registers.L); break;
    case SUB_HL:   z80_op_sub8(cpu, z80_read8(cpu, cpu->registers.HL)); break;
    case SUB_A:    z80_op_sub8(cpu, cpu->registers.A); break;
    case SUB_n:    z80_op_sub8(cpu, z80_fetch8(cpu)); break;
    case SBC_A_B:  z80_op_sbc8(cpu, cpu->registers.B); break; 
    case SBC_A_C:  z80_op_sbc8(cpu, cpu->registers.C);break; 
    case SBC_A_D:  z80_op_sbc8(cpu, cpu->registers.D);break; 
    case SBC_A_E:  z80_op_sbc8(cpu, cpu->registers.E);break; 
    case SBC_A_H:  z80_op_sbc8(cpu, cpu->registers.H);break; 
    case SBC_A_L:  z80_op_sbc8(cpu, cpu->registers.L);break; 
    case SBC_A_HL: z80_op_sbc8(cpu, z80_read8(cpu, cpu->registers.HL));break; 
    case SBC_A_A:  z80_op_sbc8(cpu, cpu->registers.A);break;
    case SBC_A_n:  z80_op_sbc8(cpu, z80_fetch8(cpu)); break;
    
    // === 16-BIT ARITHMETIC ===
    case ADD_HL_BC: cpu->cycle_count += 7; cpu->registers.HL = z80_op_add16(cpu, cpu->registers.HL, cpu->registers.BC); break;
    case ADD_HL_DE: cpu->cycle_count += 7; cpu->registers.HL = z80_op_add16(cpu, cpu->registers.HL, cpu->registers.DE); break;
    case ADD_HL_HL: cpu->cycle_count += 7; cpu->registers.HL = z80_op_add16(cpu, cpu->registers.HL, cpu->registers.HL); break;
    case ADD_HL_SP: cpu->cycle_count += 7; cpu->registers.HL = z80_op_add16(cpu, cpu->registers.HL, cpu->registers.SP); break;
    
    // === LOGICAL OPERATIONS ===
    case AND_B: z80_op_and(cpu, cpu->registers.B); break;
    case AND_C: z80_op_and(cpu, cpu->registers.C); break;
    case AND_D: z80_op_and(cpu, cpu->registers.D); break;
    case AND_E: z80_op_and(cpu, cpu->registers.E); break;
    case AND_H: z80_op_and(cpu, cpu->registers.H); break;
    case AND_L: z80_op_and(cpu, cpu->registers.L); break;
    case AND_HL: z80_op_and(cpu, z80_read8(cpu, cpu->registers.HL)); break;
    case AND_A: z80_op_and(cpu, cpu->registers.A); break;
    case AND_n: z80_op_and(cpu, z80_fetch8(cpu)); break;
    case XOR_B: z80_op_xor(cpu, cpu->registers.B); break;
    case XOR_C: z80_op_xor(cpu, cpu->registers.C); break;
    case XOR_D: z80_op_xor(cpu, cpu->registers.D); break;
    case XOR_E: z80_op_xor(cpu, cpu->registers.E); break;
    case XOR_H: z80_op_xor(cpu, cpu->registers.H); break;
    case XOR_L: z80_op_xor(cpu, cpu->registers.L); break;
    case XOR_HL: z80_op_xor(cpu, z80_read8(cpu, cpu->registers.HL)); break;
    case XOR_A: z80_op_xor(cpu, cpu->registers.A); break;
    case XOR_n: z80_op_xor(cpu, z80_fetch8(cpu)); break;
    case OR_B: z80_op_or(cpu, cpu->registers.B); break;
    case OR_C: z80_op_or(cpu, cpu->registers.C); break;
    case OR_D: z80_op_or(cpu, cpu->registers.D); break;
    case OR_E: z80_op_or(cpu, cpu->registers.E); break;
    case OR_H: z80_op_or(cpu, cpu->registers.H); break;
    case OR_L: z80_op_or(cpu, cpu->registers.L); break;
    case OR_HL: z80_op_or(cpu, z80_read8(cpu, cpu->registers.HL)); break;
    case OR_A: z80_op_or(cpu, cpu->registers.A); break;
    case OR_n: z80_op_or(cpu, z80_fetch8(cpu)); break;
    // === COMPARE OPERATIONS ===
    case CP_B: z80_op_cp(cpu, cpu->registers.A, cpu->registers.B); break;
    case CP_C: z80_op_cp(cpu, cpu->registers.A, cpu->registers.C); break;
    case CP_D: z80_op_cp(cpu, cpu->registers.A, cpu->registers.D); break;
    case CP_E: z80_op_cp(cpu, cpu->registers.A, cpu->registers.E); break;
    case CP_H: z80_op_cp(cpu, cpu->registers.A, cpu->registers.H); break;
    case CP_L: z80_op_cp(cpu, cpu->registers.A, cpu->registers.L); break;
    case CP_HL: z80_op_cp(cpu, cpu->registers.A, z80_read8(cpu, cpu->registers.HL)); break;
    case CP_A: z80_op_cp(cpu, cpu->registers.A, cpu->registers.A); break;
    case CP_n: z80_op_cp(cpu, cpu->registers.A, z80_fetch8(cpu)); break;
    
    // === ROTATE AND SHIFT OPERATIONS ===
    case RLCA: cpu->registers.A = set_flags_rlca(cpu); break;
    case RRCA: cpu->registers.A = set_flags_rrca(cpu); break;
    case RRA: cpu->registers.A = set_flags_rra(cpu); break;
    case RLA: cpu->registers.A = set_flags_rla(cpu); break;

    // === JUMP INSTRUCTIONS ===
    case JP_nn:    z80_op_jp(cpu, true);             break;
    case JP_NZ_nn: z80_op_jp(cpu, IS_Z_UNSET(cpu));  break;
    case JP_Z_nn:  z80_op_jp(cpu, IS_Z_SET(cpu));    break;
    case JP_NC_nn: z80_op_jp(cpu, IS_C_UNSET(cpu));  break;
    case JP_C_nn:  z80_op_jp(cpu, IS_C_SET(cpu));    break;
    case JP_PO_nn: z80_op_jp(cpu, IS_PV_UNSET(cpu)); break;
    case JP_PE_nn: z80_op_jp(cpu, IS_PV_SET(cpu));   break;
    case JP_P_nn:  z80_op_jp(cpu, IS_S_UNSET(cpu));  break;
    case JP_M_nn:  z80_op_jp(cpu, IS_S_SET(cpu));    break;
    case JP_HL: cpu->registers.PC = cpu->registers.HL;  break;
    case JR_d:    z80_op_jr(cpu, true);             break;
    case JR_NZ_d: z80_op_jr(cpu, IS_Z_UNSET(cpu));  break;
    case JR_Z_d:  z80_op_jr(cpu, IS_Z_SET(cpu));    break;
    case JR_NC_d: z80_op_jr(cpu, IS_C_UNSET(cpu));  break;
    case JR_C_d:  z80_op_jr(cpu, IS_C_SET(cpu));    break;
    case DJNZ_e:
        {
            int8_t offset = (int8_t)z80_fetch8(cpu);
            cpu->registers.B--;
            cpu->cycle_count += 5; // Base cycles
            if (cpu->registers.B != 0) {
                cpu->registers.PC += offset;
                cpu->cycle_count += 3; // Additional cycles for taken branch
            }
        }
        break;
    
    // === CALL INSTRUCTIONS ===
    case CALL_nn:    z80_op_call(cpu, true); break;
    case CALL_Z_nn:  z80_op_call(cpu, IS_Z_SET(cpu)); break;
    case CALL_NZ_nn: z80_op_call(cpu, IS_Z_UNSET(cpu)); break;
    case CALL_C_nn:  z80_op_call(cpu, IS_C_SET(cpu)); break;
    case CALL_NC_nn: z80_op_call(cpu, IS_C_UNSET(cpu)); break;
    
    // === RETURN INSTRUCTIONS ===
    case RET:    z80_op_ret(cpu, true); break;
    case RET_Z:  z80_op_ret(cpu, IS_Z_SET(cpu)); break;
    case RET_NZ: z80_op_ret(cpu, IS_Z_UNSET(cpu)); break;
    case RET_PE: z80_op_ret(cpu, IS_PV_SET(cpu)); break;
    case RET_PO: z80_op_ret(cpu, IS_PV_UNSET(cpu)); break;
    case RET_C:  z80_op_ret(cpu, IS_C_SET(cpu)); break;
    case RET_NC: z80_op_ret(cpu, IS_C_UNSET(cpu)); break;
    case RET_M:  z80_op_ret(cpu, IS_S_SET(cpu)); break;
    case RET_P:  z80_op_ret(cpu, IS_S_UNSET(cpu)); break;

    // === STACK OPERATIONS ===
    case POP_BC: cpu->registers.BC = z80_stack_pop16(cpu); break;
    case POP_DE: cpu->registers.DE = z80_stack_pop16(cpu); break;
    case POP_HL: cpu->registers.HL = z80_stack_pop16(cpu); break;
    case POP_AF: cpu->registers.AF = z80_stack_pop16(cpu); break;
    case PUSH_BC: z80_stack_push16(cpu, cpu->registers.BC); cpu->cycle_count++; break;
    case PUSH_DE: z80_stack_push16(cpu, cpu->registers.DE); cpu->cycle_count++; break;
    case PUSH_HL: z80_stack_push16(cpu, cpu->registers.HL); cpu->cycle_count++; break;
    case PUSH_AF: z80_stack_push16(cpu, cpu->registers.AF); cpu->cycle_count++; break;

    case RST_00: z80_op_rst(cpu, 0x0000); break;
    case RST_08: z80_op_rst(cpu, 0x0008); break;
    case RST_10: z80_op_rst(cpu, 0x0010); break;
    case RST_18: z80_op_rst(cpu, 0x0018); break;
    case RST_20: z80_op_rst(cpu, 0x0020); break;
    case RST_28: z80_op_rst(cpu, 0x0028); break;
    case RST_30: z80_op_rst(cpu, 0x0030); break;
    case RST_38: z80_op_rst(cpu, 0x0038); break;
    
    // === EXCHANGE INSTRUCTIONS ===
    case EX_AF_AF:
        {
            uint16_t temp = cpu->registers._AF;
            cpu->registers._AF = cpu->registers.AF;
            cpu->registers.AF = temp;
        }
        break;
    case EXX:
        {
            uint16_t temp = cpu->registers._BC;
            cpu->registers._BC = cpu->registers.BC;
            cpu->registers.BC = temp;
            
            temp = cpu->registers._DE;
            cpu->registers._DE = cpu->registers.DE;
            cpu->registers.DE = temp;
            
            temp = cpu->registers._HL;
            cpu->registers._HL = cpu->registers.HL;
            cpu->registers.HL = temp;
        }
        break;
    case EX_DE_HL:
        {
            uint16_t temp = cpu->registers.DE;
            cpu->registers.DE = cpu->registers.HL;
            cpu->registers.HL = temp;
            cpu->cycle_count += 3;
        }
        break;
    
    case DAA: cpu->registers.A = set_flags_daa(cpu); break;
    case CPL: cpu->registers.A = set_flags_cpl(cpu); break;
    case SCF: set_flags_scf(cpu); break;
    case CCF: set_flags_ccf(cpu); break;

    case IN_A_n:
    {
        uint8_t port = z80_fetch8(cpu);
        cpu->registers.A = z80_port_in(cpu, port);
        break;
    }
    case OUT_n_A:
    {
        uint8_t port = z80_fetch8(cpu);
        z80_port_out(cpu, port, cpu->registers.A);
        break;
    }
    
    // === PREFIX INSTRUCTIONS ===
    case DD_PREFIX: z80_execute_dd_instruction(cpu, z80_fetch8(cpu)); break;
    case ED_PREFIX: z80_execute_ed_instruction(cpu, z80_fetch8(cpu)); break;
    case FD_PREFIX: z80_execute_fd_instruction(cpu, z80_fetch8(cpu)); break;
    case CB_PREFIX: z80_execute_cb_instruction(cpu, z80_fetch8(cpu)); break;
    default:
        // printf("Unimplemented Instruction: 0x%02X at PC: 0x%04X\n", opcode, cpu->registers.PC - 1);
        cpu->running = false;
        cpu->halted = true;
        break;
    }
}