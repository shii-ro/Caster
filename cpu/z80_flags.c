#include <stdbool.h>
#include <stdint.h>
#include "z80_flags.h"

static inline void set_flag_if(struct z80_t *cpu, uint8_t flag, bool condition)
{
    if (condition)
        cpu->registers.F |= flag;
    else
        cpu->registers.F &= ~flag;
}

static inline void set_flag(struct z80_t *cpu, uint8_t flag)
{
    cpu->registers.F |= flag;
}

static inline void clear_flag(struct z80_t *cpu, uint8_t flag)
{
    cpu->registers.F &= ~flag;
}

static inline uint8_t get_flag(struct z80_t *cpu, uint8_t flag)
{
    return (cpu->registers.F & flag) ? 1 : 0;
}

static bool parity_table[256];

// Check if a value has even parity
static inline bool is_even_parity(uint8_t value)
{
    return parity_table[value];
}

void init_parity_table(void)
{
    for (int i = 0; i < 256; i++)
    {
        int count = 0;
        int val = i;

        while (val)
        {
            count += val & 1;
            val >>= 1;
        }

        parity_table[i] = (count & 1) == 0; // even number of 1s → even parity
    }
}

// Set flags for 8-bit increment operations (INC)
void set_flags_inc8(struct z80_t *cpu, uint8_t original, uint8_t result)
{
    clear_flag(cpu,  MASK_N);
    set_flag_if(cpu, MASK_S, (result & 0x80));
    set_flag_if(cpu, MASK_Z, (result == 0));
    set_flag_if(cpu, MASK_H, (original & 0x0F) == 0x0F);
    set_flag_if(cpu, MASK_PV, (original == 0x7F));

    set_flag_if(cpu, MASK_3, result & MASK_3);
    set_flag_if(cpu, MASK_5, result & MASK_5);
}

// Set flags for 8-bit decrement operations (DEC)
void set_flags_dec8(struct z80_t *cpu, uint8_t original, uint8_t result)
{
    set_flag(cpu, MASK_N);
    set_flag_if(cpu, MASK_S, (result & 0x80));
    set_flag_if(cpu, MASK_Z, (result == 0));
    set_flag_if(cpu, MASK_H, (original & 0x0F) == 0x00);
    set_flag_if(cpu, MASK_PV, (original == 0x80));

    set_flag_if(cpu, MASK_3, (result & MASK_3));
    set_flag_if(cpu, MASK_5, (result & MASK_5));
}

// Set flags for 8-bit arithmetic operations
uint8_t set_flags_add8(struct z80_t *cpu, uint8_t a, uint8_t b)
{
    uint16_t result = a + b;
    clear_flag(cpu,  MASK_N);
    set_flag_if(cpu, MASK_S, (result & MASK_S));
    set_flag_if(cpu, MASK_Z, (result & UINT8_MAX) == 0);
    set_flag_if(cpu, MASK_H, ((a & NIBBLE_LOW) + (b & NIBBLE_LOW)) > NIBBLE_LOW);
    set_flag_if(cpu, MASK_PV, ((a ^ result) & (b ^ result) & 0x80));
    set_flag_if(cpu, MASK_C, (result > UINT8_MAX));
    set_flag_if(cpu, MASK_3, (result & MASK_3));
    set_flag_if(cpu, MASK_5, (result & MASK_5));
    return result & UINT8_MAX;
}

// Set flags for 8-bit subtraction operations
uint8_t set_flags_sub8(struct z80_t *cpu, uint8_t a, uint8_t b)
{
    uint16_t result = a - b;
    set_flag(cpu,    MASK_N);
    set_flag_if(cpu, MASK_S, (result & MASK_S));
    set_flag_if(cpu, MASK_Z, (result & UINT8_MAX) == 0);
    set_flag_if(cpu, MASK_H, (a & NIBBLE_LOW) < (b & NIBBLE_LOW));
    set_flag_if(cpu, MASK_PV, ((a ^ b) & (a ^ result) & 0x80));
    set_flag_if(cpu, MASK_C, (a < b));
    set_flag_if(cpu, MASK_3, (result & MASK_3));
    set_flag_if(cpu, MASK_5, (result & MASK_5));
    return result & UINT8_MAX;
}

uint8_t set_flags_adc8(struct z80_t *cpu, uint8_t a, uint8_t b)
{
    uint8_t carry = IS_C_SET(cpu);
    uint16_t result = a + b + carry;
    clear_flag(cpu,  MASK_N);
    set_flag_if(cpu, MASK_S, (result & MASK_S));
    set_flag_if(cpu, MASK_Z, (result & UINT8_MAX) == 0);
    set_flag_if(cpu, MASK_H, ((a & NIBBLE_LOW) + (b & NIBBLE_LOW) + carry) > NIBBLE_LOW);
    set_flag_if(cpu, MASK_PV, ((a ^ result) & (b ^ result) & 0x80));
    set_flag_if(cpu, MASK_C, (result > UINT8_MAX));
    set_flag_if(cpu, MASK_3, (result & MASK_3));
    set_flag_if(cpu, MASK_5, (result & MASK_5));
    return result & UINT8_MAX;
}

uint8_t set_flags_sbc8(struct z80_t *cpu, uint8_t a, uint8_t b)
{
    uint8_t carry = IS_C_SET(cpu);
    uint16_t result = a - b - carry;
    set_flag(cpu,    MASK_N);
    set_flag_if(cpu, MASK_S, (result & MASK_S));
    set_flag_if(cpu, MASK_Z, (result & UINT8_MAX) == 0);
    set_flag_if(cpu, MASK_H, (a & NIBBLE_LOW) < ((b & NIBBLE_LOW) + carry));
    set_flag_if(cpu, MASK_PV, ((a ^ b) & (a ^ result) & 0x80));
    set_flag_if(cpu, MASK_C, (a < (b + carry)));
    set_flag_if(cpu, MASK_3, result & MASK_3); 
    set_flag_if(cpu, MASK_5, result & MASK_5);  
    return result & UINT8_MAX;
}

// Set flags for 16-bit addition operations (ADD HL,rr)
void set_flags_add16(struct z80_t *cpu, uint16_t a, uint16_t b, uint16_t result)
{
    clear_flag(cpu, MASK_N);
    set_flag_if(cpu, MASK_H, ((a & 0x0FFF) + (b & 0x0FFF)) > 0x0FFF);
    set_flag_if(cpu, MASK_C, ((uint32_t)a + (uint32_t)b) > 0xFFFF);

    set_flag_if(cpu, MASK_3, (result >> 8) & 0x08);
    set_flag_if(cpu, MASK_5, (result >> 8) & 0x20);
}

// For OR and XOR operations
void set_flags_logical_or_xor(struct z80_t *cpu, uint8_t result)
{
    clear_flag(cpu, MASK_N);
    clear_flag(cpu, MASK_H);
    clear_flag(cpu, MASK_C);
    set_flag_if(cpu, MASK_S, result & 0x80);
    set_flag_if(cpu, MASK_Z, result == 0);
    set_flag_if(cpu, MASK_PV, is_even_parity(result));

    set_flag_if(cpu, MASK_3, result & 0x0008);
    set_flag_if(cpu, MASK_5, result & 0x0020);
}

// For AND operations
void set_flags_logical_and(struct z80_t *cpu, uint8_t result)
{
    // Clear N and C flags
    clear_flag(cpu, MASK_N);
    clear_flag(cpu, MASK_C);
    set_flag(cpu, MASK_H);
    set_flag_if(cpu, MASK_S, result & 0x80);
    set_flag_if(cpu, MASK_Z, result == 0);
    set_flag_if(cpu, MASK_PV, is_even_parity(result));

    set_flag_if(cpu, MASK_3, result & MASK_3);
    set_flag_if(cpu, MASK_5, result & MASK_5);
}

// Set flags for comparison operations (same as subtraction but don't store result)
void set_flags_cp(struct z80_t *cpu, uint8_t a, uint8_t b)
{
    set_flags_sub8(cpu, a, b);
}

void set_flags_adc16(struct z80_t *cpu, uint16_t a, uint16_t b, uint16_t result)
{
    uint8_t carry = IS_C_SET(cpu);
    set_flag_if(cpu, MASK_S, result & 0x8000);
    clear_flag(cpu, MASK_N);
    set_flag_if(cpu, MASK_Z, result == 0);
    set_flag_if(cpu, MASK_H, ((a & 0x0FFF) + (b & 0x0FFF) + carry) > 0x0FFF);
    bool overflow = ((a ^ result) & (b ^ result) & 0x8000) != 0;  
    set_flag_if(cpu, MASK_PV, overflow);
    // Set C flag if carry occurred (unsigned overflow)
    set_flag_if(cpu, MASK_C, ((uint32_t)a + (uint32_t)b + carry) > 0xFFFF);

    set_flag_if(cpu, MASK_3, result & MASK_3);
    set_flag_if(cpu, MASK_5, result & MASK_5);
}

void set_flags_sbc16(struct z80_t *cpu, uint16_t a, uint16_t b, uint16_t result)
{
    uint8_t carry_in = IS_C_SET(cpu);
    set_flag(cpu, MASK_N);
    set_flag_if(cpu, MASK_S, (result & 0x8000) != 0);
    set_flag_if(cpu, MASK_Z, result == 0);
    set_flag_if(cpu, MASK_H, (a & 0x0FFF) < ((b & 0x0FFF) + carry_in));
    int16_t sa = (int16_t)a;
    int16_t sb = (int16_t)b;
    int16_t sr = (int16_t)result;
    set_flag_if(cpu, MASK_PV, ((sa ^ sb) & (sa ^ sr)) < 0); // Check MSB for sign change
    set_flag_if(cpu, MASK_C, (uint32_t)a < ((uint32_t)b + carry_in));
    set_flag_if(cpu, MASK_3, (result >> 8) & MASK_3);
    set_flag_if(cpu, MASK_5, (result >> 8) & MASK_5);
}

// Common flag setter for block transfer instructions (LDI, LDD, LDIR, LDDR)
void set_flags_block_transfer(struct z80_t *cpu, uint16_t bc_after)
{
    CLEAR_N(cpu);
    CLEAR_H(cpu);
    set_flag_if(cpu, MASK_PV, bc_after != 0);

    bc_after = bc_after >> 8;
    set_flag_if(cpu, MASK_3, bc_after & MASK_3);
    set_flag_if(cpu, MASK_5, bc_after & MASK_5);
}

void set_flags_block_io(struct z80_t *cpu)
{
    set_flag(cpu, MASK_Z);
    set_flag(cpu, MASK_N);
}

uint8_t set_flags_cpd(struct z80_t *cpu, uint8_t r)
{
    uint8_t result = cpu->registers.A - r;
    cpu->registers.HL--;
    cpu->registers.BC--;

    set_flag_if(cpu, MASK_S, (result & 0x80));
    set_flag_if(cpu, MASK_Z, (result == 0));
    set_flag_if(cpu, MASK_H, (cpu->registers.A & NIBBLE_LOW) < (r & NIBBLE_LOW));
    set_flag_if(cpu, MASK_PV, cpu->registers.BC != 0);
    set_flag(cpu,  MASK_N);
    return result;
}

uint8_t set_flags_cpi(struct z80_t *cpu, uint8_t r)
{
    uint8_t result = cpu->registers.A - r;
    cpu->registers.HL++;
    cpu->registers.BC--;

    set_flag_if(cpu, MASK_S, (result & 0x80));
    set_flag_if(cpu, MASK_Z, (result == 0));
    set_flag_if(cpu, MASK_H, (cpu->registers.A & NIBBLE_LOW) < (r & NIBBLE_LOW));
    set_flag_if(cpu, MASK_PV, cpu->registers.BC != 0);
    set_flag(cpu,  MASK_N);
    return result;
}

uint8_t set_flags_rlca(struct z80_t *cpu)
{
    uint8_t carry = (cpu->registers.A >> 7) & 0x1;
    uint8_t result = (cpu->registers.A << 1) | carry;
    clear_flag(cpu, MASK_H);
    clear_flag(cpu, MASK_N);
    set_flag_if(cpu, MASK_C, carry);
    return result;
}

uint8_t set_flags_rrca(struct z80_t *cpu)
{
    uint8_t carry = cpu->registers.A & 0x1;
    uint8_t result = (carry << 7) | (cpu->registers.A >> 1);
    clear_flag(cpu, MASK_H);
    clear_flag(cpu, MASK_N);
    set_flag_if(cpu, MASK_C, carry);
    return result;
}

void set_flags_bit(struct z80_t *cpu, uint8_t bit, uint8_t r)
{
    uint8_t bit_is_zero = !((r >> bit) & 0x1); // Check if the tested bit is 0

    clear_flag(cpu, MASK_N); 
    set_flag(cpu, MASK_H);   
    set_flag_if(cpu, MASK_Z, bit_is_zero);

}

uint8_t set_flags_rlc(struct z80_t *cpu, uint8_t r)
{
    uint8_t bit7 = r & 0x80;
    r = (r << 1) | (bit7 >> 7);
    set_flag_if(cpu, MASK_S, (r & 0x80));
    set_flag_if(cpu, MASK_Z, (r == 0));
    clear_flag(cpu,  MASK_H);
    set_flag_if(cpu, MASK_PV, is_even_parity(r));
    clear_flag(cpu,  MASK_N);
    set_flag_if(cpu, MASK_C, bit7);
    return r;
}

uint8_t set_flags_rl(struct z80_t *cpu, uint8_t r)
{
    uint8_t result = (r << 1) | (get_flag(cpu, MASK_C));
    set_flag_if(cpu, MASK_S, (result & 0x80));
    set_flag_if(cpu, MASK_Z, (result == 0));
    clear_flag(cpu,  MASK_H);
    set_flag_if(cpu, MASK_PV, is_even_parity(result));
    clear_flag(cpu,  MASK_N);
    set_flag_if(cpu, MASK_C, r & 0x80);
    return result;
}

uint8_t set_flags_rrc(struct z80_t *cpu, uint8_t r)
{
    uint8_t bit1 = r & 0x01;
    r = (r >> 1) | (bit1 << 7);
    set_flag_if(cpu, MASK_S, (r & 0x80));
    set_flag_if(cpu, MASK_Z, (r == 0));
    clear_flag(cpu,  MASK_H);
    set_flag_if(cpu, MASK_PV, is_even_parity(r));
    clear_flag(cpu,  MASK_N);
    set_flag_if(cpu, MASK_C, bit1);
    return r;
}

uint8_t set_flags_rr(struct z80_t *cpu, uint8_t r)
{
    uint8_t result = (r >> 1) | (get_flag(cpu, MASK_C)) << 7;
    set_flag_if(cpu, MASK_S, (result & 0x80));
    set_flag_if(cpu, MASK_Z, (result == 0));
    clear_flag(cpu, MASK_H);
    set_flag_if(cpu, MASK_PV, is_even_parity(result));
    clear_flag(cpu, MASK_N);
    set_flag_if(cpu, MASK_C, r & 0x1);
    return result;
}

uint8_t set_flags_sla(struct z80_t *cpu, uint8_t r)
{
    uint8_t result = r << 1;
    set_flag_if(cpu, MASK_S, (result & 0x80));
    set_flag_if(cpu, MASK_Z, (result == 0));
    clear_flag(cpu, MASK_H);
    set_flag_if(cpu, MASK_PV, is_even_parity(result));
    clear_flag(cpu, MASK_N);
    set_flag_if(cpu, MASK_C, (r & 0x80));
    return result;
}

uint8_t set_flags_sra(struct z80_t *cpu, uint8_t r)
{
    uint8_t result = (r >> 1) | (r & 0x80);
    set_flag_if(cpu, MASK_S, (result & 0x80));
    set_flag_if(cpu, MASK_Z, (result == 0));
    clear_flag(cpu, MASK_H);
    set_flag_if(cpu, MASK_PV, is_even_parity(result));
    clear_flag(cpu, MASK_N);
    set_flag_if(cpu, MASK_C, (r & 0x1));
    return result;
}

uint8_t set_flags_sll(struct z80_t *cpu, uint8_t r)
{
    uint8_t result = (r << 1) | 0x1;
    set_flag_if(cpu, MASK_S, (result & 0x80));
    set_flag_if(cpu, MASK_Z, (result == 0));
    clear_flag(cpu, MASK_H);
    set_flag_if(cpu, MASK_PV, is_even_parity(result));
    clear_flag(cpu, MASK_N);
    set_flag_if(cpu, MASK_C, (r & 0x80));
    return result;
}

uint8_t set_flags_srl(struct z80_t *cpu, uint8_t r)
{
    uint8_t result = (r >> 1);
    set_flag_if(cpu, MASK_S, (result & 0x80));
    set_flag_if(cpu, MASK_Z, (result == 0));
    clear_flag(cpu, MASK_H);
    set_flag_if(cpu, MASK_PV, is_even_parity(result));
    clear_flag(cpu, MASK_N);
    set_flag_if(cpu, MASK_C, (r & 0x1));
    return result;
}

uint8_t set_flags_cpl(struct z80_t *cpu)
{
    set_flag(cpu, MASK_H);
    set_flag(cpu, MASK_N);
    return ~cpu->registers.A;
}

void set_flags_rotate_digit(struct z80_t *cpu)
{
    set_flag_if(cpu, MASK_S, cpu->registers.A & 0x80);
    set_flag_if(cpu, MASK_Z, cpu->registers.A == 0);
    clear_flag(cpu, MASK_H);
    set_flag_if(cpu, MASK_PV, is_even_parity(cpu->registers.A));
    clear_flag(cpu, MASK_N);
}

void set_flags_ccf(struct z80_t *cpu)
{
    bool old_carry = (cpu->registers.F & MASK_C) != 0;
    clear_flag(cpu, MASK_N);
    set_flag_if(cpu, MASK_H, old_carry);
    set_flag_if(cpu, MASK_C, !old_carry);
}

void set_flags_scf(struct z80_t *cpu)
{
    clear_flag(cpu, MASK_H);
    clear_flag(cpu, MASK_N);
    set_flag(cpu, MASK_C);
}

uint8_t set_flags_rra(struct z80_t *cpu)
{
    uint8_t carry_in = get_flag(cpu, MASK_C);
    uint8_t result = (cpu->registers.A >> 1) | (carry_in << 7);

    set_flag_if(cpu, MASK_C, cpu->registers.A & 0x01);
    clear_flag(cpu, MASK_H);
    clear_flag(cpu, MASK_N);
    
    return result;
}

uint8_t set_flags_rla(struct z80_t *cpu)
{
    uint8_t carry_in = get_flag(cpu, MASK_C);
    uint8_t result = (cpu->registers.A << 1) | carry_in;

    set_flag_if(cpu, MASK_C, cpu->registers.A & 0x80);
    clear_flag(cpu, MASK_H);
    clear_flag(cpu, MASK_N);
    
    return result;
}

uint8_t set_flags_daa(struct z80_t *cpu)
{
    uint8_t old_A = cpu->registers.A;
    uint8_t old_F = cpu->registers.F;

    uint8_t correction = 0x00;
    bool carry_out = false;
    bool half_carry_out = false;

    uint8_t new_A = 0x0;

    if (old_F & MASK_N)
    {
        if ((old_F & MASK_H) || ((old_A & 0x0F) > 0x09))
        {
            correction += 0x06;
        }

        if ((old_F & MASK_C) || (old_A > 0x99))
        {
            correction += 0x60;
            carry_out = true; // Preserve carry for subtraction
        }

        new_A = old_A - correction;
        half_carry_out = (old_A & 0x0F) < (correction & 0x0F);
    }
    else
    {
        // Lower nibble correction
        if ((old_F & MASK_H) || ((old_A & 0x0F) > 0x09))
        {
            correction += 0x06;
        }

        // Upper nibble correction
        if ((old_F & MASK_C) || (old_A > 0x99) ||
            ((old_A > 0x8F) && ((old_A & 0x0F) > 0x09)))
        {
            correction += 0x60;
            carry_out = true;
        }

        new_A = old_A + correction;
        half_carry_out = ((old_A & 0x0F) + (correction & 0x0F)) > 0x0F;
    }

    // Update flags
    set_flag_if(cpu, MASK_C, carry_out);
    set_flag_if(cpu, MASK_H, half_carry_out);
    set_flag_if(cpu, MASK_S, (new_A & 0x80) != 0);
    set_flag_if(cpu, MASK_Z, new_A == 0);
    set_flag_if(cpu, MASK_PV, is_even_parity(new_A));

    return new_A;
}
