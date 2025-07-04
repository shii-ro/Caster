#include <stdio.h>
#include "z80_op.h"
#include "z80_flags.h"

void z80_op_call(struct z80_t *cpu, bool condition)
{
    uint16_t addr = z80_fetch16(cpu); // 6 cycles

    if (condition)
    {
        // intercept BDOS calls
        if (addr == 0x0005)
        {
            switch (cpu->registers.C)
            {
            case 1: // Console Input
                // cpu->registers.A = 0x1A; // Return Ctrl-Z (EOF) for now
                break;

            case 2: // Console Output
                printf("%c", cpu->registers.E);
                break;

            case 6: // Direct Console I/O
                if (cpu->registers.E == 0xFF)
                {
                    // Check if key available - return 0 for now
                    // cpu->registers.A = 0;
                }
                else
                {
                    // Output character
                    printf("%c", cpu->registers.E);
                }
                break;

            case 9: // Print String at DE until '$'
            {
                uint16_t str_addr = cpu->registers.DE;
                char c;
                while ((c = (char)z80_read8(cpu, str_addr)) != '$')
                {
                    printf("%c", c);
                    str_addr++;
                }
                break;
            }

            case 11: // Console Status
                // Return 0xFF if character ready, 0x00 if not
                // cpu->registers.A = 0x00; // No character ready
                break;

            default:
                // For unimplemented BDOS calls, just return
                break;
            }

            return;
        }

        // Normal CALL instruction
        z80_stack_push16(cpu, cpu->registers.PC); // 6 cycles
        cpu->cycle_count++;                       // 1 cycle
        cpu->registers.PC = addr;
    }
    // If condition is false, do nothing (conditional call not taken)
}

void z80_op_ret(struct z80_t *cpu, bool condition)
{
    cpu->cycle_count++; // 1 cycle
    if(condition)
    {
        uint16_t addr = z80_stack_pop16(cpu); // 6 cycles
        cpu->registers.PC = addr;  // Always set PC to the popped address
    }
}

void z80_op_jp(struct z80_t *cpu, bool condition)
{
    uint16_t jp_addr = z80_fetch16(cpu); // 6 cycles
    if(condition)
        cpu->registers.PC = jp_addr;
}

void z80_op_jr(struct z80_t *cpu, bool condition)
{
    int8_t offset = (int8_t)z80_fetch8(cpu);

    if (condition)
    {
        cpu->registers.PC += offset;
        cpu->cycle_count += 5;  // Branch taken penalty
    }
}

uint8_t z80_op_inc8(struct z80_t *cpu, uint8_t r)
{
    uint8_t result = r + 1;
    set_flags_inc8(cpu, r, result);  
    return result;
}

uint8_t z80_op_dec8(struct z80_t *cpu, uint8_t r)
{
    uint8_t result = r - 1;
    set_flags_dec8(cpu, r, result);   
    return result;
}

void z80_op_cp(struct z80_t *cpu, uint8_t a, uint8_t b)
{
    set_flags_sub8(cpu, a, b);
}

void z80_op_or(struct z80_t *cpu, uint8_t r)
{
    cpu->registers.A = cpu->registers.A | r;
    set_flags_logical_or_xor(cpu, cpu->registers.A);
}

void z80_op_xor(struct z80_t *cpu, uint8_t r)
{
    cpu->registers.A = cpu->registers.A ^ r;
    set_flags_logical_or_xor(cpu, cpu->registers.A);
}

void z80_op_and(struct z80_t *cpu, uint8_t r)
{
    cpu->registers.A = cpu->registers.A & r;
    set_flags_logical_and(cpu, cpu->registers.A);
}

void z80_op_bit(struct z80_t *cpu, uint8_t bit, uint8_t r)
{
    set_flags_bit(cpu, bit, r);
}

uint8_t z80_op_res(struct z80_t *cpu, uint8_t bit, uint8_t r)
{
    uint8_t result = ~(1 << bit) & (r);
    return result;
}

uint8_t z80_op_set(struct z80_t *cpu, uint8_t bit, uint8_t r)
{
    uint8_t result = (1 << bit) | r;
    return result;
}

uint8_t z80_op_rlc(struct z80_t *cpu, uint8_t r)
{
    return set_flags_rlc(cpu, r);
}

uint8_t z80_op_rl(struct z80_t *cpu, uint8_t r)
{
    return set_flags_rl(cpu, r);
}

uint8_t z80_op_rrc(struct z80_t *cpu, uint8_t r)
{
    return set_flags_rrc(cpu, r);
}

uint8_t z80_op_rr(struct z80_t *cpu, uint8_t r)
{
    return set_flags_rr(cpu, r);
}

uint8_t z80_op_sla(struct z80_t *cpu, uint8_t r)
{
    return set_flags_sla(cpu, r);
}

uint8_t z80_op_sra(struct z80_t *cpu, uint8_t r)
{
    return set_flags_sra(cpu, r);
}

uint8_t z80_op_sll(struct z80_t *cpu, uint8_t r)
{
    return set_flags_sll(cpu, r);
}

uint8_t z80_op_srl(struct z80_t *cpu, uint8_t r)
{
    return set_flags_srl(cpu, r);
}

uint16_t z80_op_add16(struct z80_t *cpu, uint16_t rp, uint16_t value)
{
    cpu->cycle_count += 7;
    uint16_t result = rp + value; 
    set_flags_add16(cpu, rp, value, result);
    return result;
}

void z80_op_add8(struct z80_t *cpu, uint8_t r)
{
    cpu->registers.A = set_flags_add8(cpu, cpu->registers.A, r);
}

void z80_op_sub8(struct z80_t *cpu, uint8_t r)
{
    cpu->registers.A = set_flags_sub8(cpu, cpu->registers.A, r);
}

void z80_op_adc8(struct z80_t *cpu, uint8_t r)
{
    cpu->registers.A = set_flags_adc8(cpu, cpu->registers.A, r);
}

void z80_op_sbc8(struct z80_t *cpu, uint8_t r)
{
    cpu->registers.A = set_flags_sbc8(cpu, cpu->registers.A, r);
}

void z80_op_adc16(struct z80_t *cpu, uint16_t rr)
{
    cpu->cycle_count += 7;
    uint16_t result = cpu->registers.HL + rr + IS_C_SET(cpu);
    set_flags_adc16(cpu, cpu->registers.HL, rr, result);
    cpu->registers.HL = result;
}

void z80_op_sbc16(struct z80_t *cpu, uint16_t rr)
{
    cpu->cycle_count += 7;
    uint16_t result = cpu->registers.HL - rr - IS_C_SET(cpu);
    set_flags_sbc16(cpu, cpu->registers.HL, rr, result);
    cpu->registers.HL = result;
}
