#include <stdio.h>
#include "z80_op.h"
#include "z80_flags.h"

// Z80 ED-prefixed Opcode enumeration (corrected)
enum z80_ed_opcodes
{
    // INPUT/OUTPUT INSTRUCTIONS
    ED_IN_B_C = 0x40,       // IN B,(C)
    ED_OUT_C_B = 0x41,      // OUT (C),B
    ED_IN_C_C = 0x48,       // IN C,(C)
    ED_OUT_C_C = 0x49,      // OUT (C),C
    ED_IN_D_C = 0x50,       // IN D,(C)
    ED_OUT_C_D = 0x51,      // OUT (C),D
    ED_IN_E_C = 0x58,       // IN E,(C)
    ED_OUT_C_E = 0x59,      // OUT (C),E
    ED_IN_H_C = 0x60,       // IN H,(C)
    ED_OUT_C_H = 0x61,      // OUT (C),H
    ED_IN_L_C = 0x68,       // IN L,(C)
    ED_OUT_C_L = 0x69,      // OUT (C),L
    ED_IN_A_C = 0x78,       // IN A,(C)
    ED_OUT_C_A = 0x79,      // OUT (C),A
    
    // 16-BIT ARITHMETIC - SBC/ADC HL with register pairs
    ED_SBC_HL_BC = 0x42,    // SBC HL,BC
    ED_ADC_HL_BC = 0x4A,    // ADC HL,BC
    ED_SBC_HL_DE = 0x52,    // SBC HL,DE
    ED_ADC_HL_DE = 0x5A,    // ADC HL,DE
    ED_SBC_HL_HL = 0x62,    // SBC HL,HL
    ED_ADC_HL_HL = 0x6A,    // ADC HL,HL
    ED_SBC_HL_SP = 0x72,    // SBC HL,SP
    ED_ADC_HL_SP = 0x7A,    // ADC HL,SP
    
    // 16-BIT LOAD INSTRUCTIONS
    ED_LD_NN_BC = 0x43,     // LD (NN),BC
    ED_LD_BC_NN = 0x4B,     // LD BC,(NN)
    ED_LD_NN_DE = 0x53,     // LD (NN),DE
    ED_LD_DE_NN = 0x5B,     // LD DE,(NN)
    ED_LD_NN_SP = 0x73,     // LD (NN),SP
    ED_LD_SP_NN = 0x7B,     // LD SP,(NN)
    
    // SPECIAL ACCUMULATOR/FLAG OPERATIONS
    ED_NEG = 0x44,          // NEG
    ED_LD_I_A = 0x47,       // LD I,A
    ED_LD_A_I = 0x57,       // LD A,I
    ED_LD_R_A = 0x4F,       // LD R,A
    ED_LD_A_R = 0x5F,       // LD A,R
    
    // RETURN FROM INTERRUPT
    ED_RETN = 0x45,         // RETN
    ED_RETI = 0x4D,         // RETI
    
    // INTERRUPT MODE SETTING
    ED_IM_0 = 0x46,         // IM 0
    ED_IM_1 = 0x56,         // IM 1
    ED_IM_2 = 0x5E,         // IM 2
    
    // ROTATE DIGIT INSTRUCTIONS
    ED_RRD = 0x67,          // RRD
    ED_RLD = 0x6F,          // RLD
    
    // BLOCK TRANSFER INSTRUCTIONS
    ED_LDI = 0xA0,          // LDI
    ED_CPI = 0xA1,          // CPI
    ED_INI = 0xA2,          // INI
    ED_OUTI = 0xA3,         // OUTI
    ED_LDD = 0xA8,          // LDD
    ED_CPD = 0xA9,          // CPD
    ED_IND = 0xAA,          // IND
    ED_OUTD = 0xAB,         // OUTD
    ED_LDIR = 0xB0,         // LDIR
    ED_CPIR = 0xB1,         // CPIR
    ED_INIR = 0xB2,         // INIR
    ED_OTIR = 0xB3,         // OTIR
    ED_LDDR = 0xB8,         // LDDR
    ED_CPDR = 0xB9,         // CPDR
    ED_INDR = 0xBA,         // INDR
    ED_OTDR = 0xBB          // OTDR
};

void z80_execute_ed_instruction(struct z80_t *cpu, uint8_t opcode)
{
    switch (opcode)
    {
    // INPUT/OUTPUT INSTRUCTIONS
    // case ED_IN_B_C: cpu->registers.B = z80_input(cpu, cpu->registers.C); break;
    // case ED_OUT_C_B: z80_output(cpu, cpu->registers.C, cpu->registers.B); break;
    // case ED_IN_C_C: cpu->registers.C = z80_input(cpu, cpu->registers.C); break;
    // case ED_OUT_C_C: z80_output(cpu, cpu->registers.C, cpu->registers.C); break;
    // case ED_IN_D_C: cpu->registers.D = z80_input(cpu, cpu->registers.C); break;
    // case ED_OUT_C_D: z80_output(cpu, cpu->registers.C, cpu->registers.D); break;
    // case ED_IN_E_C: cpu->registers.E = z80_input(cpu, cpu->registers.C); break;
    // case ED_OUT_C_E: z80_output(cpu, cpu->registers.C, cpu->registers.E); break;
    // case ED_IN_H_C: cpu->registers.H = z80_input(cpu, cpu->registers.C); break;
    // case ED_OUT_C_H: z80_output(cpu, cpu->registers.C, cpu->registers.H); break;
    // case ED_IN_L_C: cpu->registers.L = z80_input(cpu, cpu->registers.C); break;
    // case ED_OUT_C_L: z80_output(cpu, cpu->registers.C, cpu->registers.L); break;
    // case ED_IN_A_C: cpu->registers.A = z80_input(cpu, cpu->registers.C); break;
    // case ED_OUT_C_A: z80_output(cpu, cpu->registers.C, cpu->registers.A); break;
    
    // 16-BIT ARITHMETIC
    case ED_SBC_HL_BC: z80_op_sbc16(cpu, cpu->registers.BC); break;
    case ED_ADC_HL_BC: z80_op_adc16(cpu, cpu->registers.BC); break;
    case ED_SBC_HL_DE: z80_op_sbc16(cpu, cpu->registers.DE); break;
    case ED_ADC_HL_DE: z80_op_adc16(cpu, cpu->registers.DE); break;
    case ED_SBC_HL_HL: z80_op_sbc16(cpu, cpu->registers.HL); break;
    case ED_ADC_HL_HL: z80_op_adc16(cpu, cpu->registers.HL); break;
    case ED_SBC_HL_SP: z80_op_sbc16(cpu, cpu->registers.SP); break;
    case ED_ADC_HL_SP: z80_op_adc16(cpu, cpu->registers.SP); break;
    
    // 16-BIT LOAD INSTRUCTIONS
    case ED_LD_NN_BC: z80_write16(cpu, z80_fetch16(cpu), cpu->registers.BC); break;
    case ED_LD_BC_NN: cpu->registers.BC = z80_read16(cpu, z80_fetch16(cpu)); break;
    case ED_LD_NN_DE: z80_write16(cpu, z80_fetch16(cpu), cpu->registers.DE); break;
    case ED_LD_DE_NN: cpu->registers.DE = z80_read16(cpu, z80_fetch16(cpu)); break;
    case ED_LD_NN_SP: z80_write16(cpu, z80_fetch16(cpu), cpu->registers.SP); break;
    case ED_LD_SP_NN: cpu->registers.SP = z80_read16(cpu, z80_fetch16(cpu)); break;
    
    // SPECIAL OPERATIONS
    // case ED_NEG: z80_op_neg(cpu); break;
    // case ED_LD_I_A: cpu->registers.I = cpu->registers.A; break;
    // case ED_LD_A_I: 
    //     cpu->registers.A = cpu->registers.I;
    //     set_flags_ld_a_ir(cpu, cpu->registers.I);
    //     break;
    // case ED_LD_R_A: cpu->registers.R = cpu->registers.A; break;
    // case ED_LD_A_R: 
    //     cpu->registers.A = cpu->registers.R;
    //     set_flags_ld_a_ir(cpu, cpu->registers.R);
    //     break;
    
    // INTERRUPT HANDLING
    // case ED_RETN: z80_op_retn(cpu); break;
    // case ED_RETI: z80_op_reti(cpu); break;
    // case ED_IM_0: cpu->interrupt_mode = 0; break;
    // case ED_IM_1: cpu->interrupt_mode = 1; break;
    // case ED_IM_2: cpu->interrupt_mode = 2; break;
    
    // ROTATE DIGIT INSTRUCTIONS
    // case ED_RRD: z80_op_rrd(cpu); break;
    // case ED_RLD: z80_op_rld(cpu); break;
    
    // BLOCK TRANSFER INSTRUCTIONS
    // case ED_LDI:
    //     {
    //         uint8_t value = z80_read8(cpu, cpu->registers.HL);
    //         z80_write8(cpu, cpu->registers.DE, value);
    //         cpu->registers.HL++;
    //         cpu->registers.DE++;
    //         cpu->registers.BC--;
    //         set_flags_block_transfer(cpu, cpu->registers.BC);
    //     }
    //     break;
        
    case ED_CPI:
        {
            uint8_t value = z80_read8(cpu, cpu->registers.HL);
            set_flags_cpi(cpu, value);
        }
        break;
        
    case ED_LDD:
        {
            uint8_t value = z80_read8(cpu, cpu->registers.HL);
            z80_write8(cpu, cpu->registers.DE, value);
            cpu->registers.HL--;
            cpu->registers.DE--;
            cpu->registers.BC--;
            set_flags_block_transfer(cpu, cpu->registers.BC);
        }
        break;
        
    case ED_CPD:
        {
            uint8_t value = z80_read8(cpu, cpu->registers.HL);
            set_flags_cpd(cpu, value);
        }
        break;
        
    case ED_LDIR:
        {
            uint8_t value = z80_read8(cpu, cpu->registers.HL);
            z80_write8(cpu, cpu->registers.DE, value);
            cpu->registers.HL++;
            cpu->registers.DE++;
            cpu->registers.BC--;
            set_flags_block_transfer(cpu, cpu->registers.BC);
            
            if (cpu->registers.BC != 0)
            {
                cpu->registers.PC -= 2;
                cpu->cycle_count += 5;
            }
        }
        break;
        
    case ED_CPIR:
        {
            uint8_t value = z80_read8(cpu, cpu->registers.HL);
            set_flags_cpi(cpu, value);
            
            // Continue if BC != 0 AND Z flag is clear (values not equal)
            if (cpu->registers.BC != 0 && !(cpu->registers.F & MASK_Z))
            {
                cpu->registers.PC -= 2;
                cpu->cycle_count += 5;
            }
        }
        break;
        
    // case ED_LDDR:
    //     {
    //         uint8_t value = z80_read8(cpu, cpu->registers.HL);
    //         z80_write8(cpu, cpu->registers.DE, value);
    //         cpu->registers.HL--;
    //         cpu->registers.DE--;
    //         cpu->registers.BC--;
    //         set_flags_block_transfer(cpu, cpu->registers.BC);
            
    //         if (cpu->registers.BC != 0)
    //         {
    //             cpu->registers.PC -= 2;
    //             cpu->cycle_count += 5;
    //         }
    //     }
    //     break;
        
    case ED_CPDR:
        {
            uint8_t value = z80_read8(cpu, cpu->registers.HL);
            set_flags_cpd(cpu, value);
            
            // Continue if BC != 0 AND Z flag is clear (values not equal)
            if (cpu->registers.BC != 0 && !(cpu->registers.F & MASK_Z))
            {
                cpu->registers.PC -= 2;
                cpu->cycle_count += 5;
            }
        }
        break;
        
    // // I/O Block instructions (simplified - you'll need proper I/O handling)
    // case ED_INI:
    //     {
    //         uint8_t value = z80_input(cpu, cpu->registers.C);
    //         z80_write8(cpu, cpu->registers.HL, value);
    //         cpu->registers.HL++;
    //         cpu->registers.B--;
    //         set_flags_block_io(cpu, cpu->registers.B);
    //     }
    //     break;
        
    // case ED_IND:
    //     {
    //         uint8_t value = z80_input(cpu, cpu->registers.C);
    //         z80_write8(cpu, cpu->registers.HL, value);
    //         cpu->registers.HL--;
    //         cpu->registers.B--;
    //         set_flags_block_io(cpu, cpu->registers.B);
    //     }
    //     break;
        
    // case ED_OUTI:
    //     {
    //         uint8_t value = z80_read8(cpu, cpu->registers.HL);
    //         z80_output(cpu, cpu->registers.C, value);
    //         cpu->registers.HL++;
    //         cpu->registers.B--;
    //         set_flags_block_io(cpu, cpu->registers.B);
    //     }
    //     break;
        
    // case ED_OUTD:
    //     {
    //         uint8_t value = z80_read8(cpu, cpu->registers.HL);
    //         z80_output(cpu, cpu->registers.C, value);
    //         cpu->registers.HL--;
    //         cpu->registers.B--;
    //         set_flags_block_io(cpu, cpu->registers.B);
    //     }
    //     break;
        
    // case ED_INIR:
    //     {
    //         uint8_t value = z80_input(cpu, cpu->registers.C);
    //         z80_write8(cpu, cpu->registers.HL, value);
    //         cpu->registers.HL++;
    //         cpu->registers.B--;
    //         set_flags_block_io(cpu, cpu->registers.B);
            
    //         if (cpu->registers.B != 0)
    //         {
    //             cpu->registers.PC -= 2;
    //             cpu->cycle_count += 5;
    //         }
    //     }
    //     break;
        
    // case ED_INDR:
    //     {
    //         uint8_t value = z80_input(cpu, cpu->registers.C);
    //         z80_write8(cpu, cpu->registers.HL, value);
    //         cpu->registers.HL--;
    //         cpu->registers.B--;
    //         set_flags_block_io(cpu, cpu->registers.B);
            
    //         if (cpu->registers.B != 0)
    //         {
    //             cpu->registers.PC -= 2;
    //             cpu->cycle_count += 5;
    //         }
    //     }
    //     break;
        
    // case ED_OTIR:
    //     {
    //         uint8_t value = z80_read8(cpu, cpu->registers.HL);
    //         z80_output(cpu, cpu->registers.C, value);
    //         cpu->registers.HL++;
    //         cpu->registers.B--;
    //         set_flags_block_io(cpu, cpu->registers.B);
            
    //         if (cpu->registers.B != 0)
    //         {
    //             cpu->registers.PC -= 2;
    //             cpu->cycle_count += 5;
    //         }
    //     }
    //     break;
        
    // case ED_OTDR:
    //     {
    //         uint8_t value = z80_read8(cpu, cpu->registers.HL);
    //         z80_output(cpu, cpu->registers.C, value);
    //         cpu->registers.HL--;
    //         cpu->registers.B--;
    //         set_flags_block_io(cpu, cpu->registers.B);
            
    //         if (cpu->registers.B != 0)
    //         {
    //             cpu->registers.PC -= 2;
    //             cpu->cycle_count += 5;
    //         }
    //     }
    //     break;
        
    default:
        printf("Unimplemented 0xED Instruction: 0x%02X\n", opcode);
        cpu->running = false;
        cpu->halted = true;
        break;
    }
}