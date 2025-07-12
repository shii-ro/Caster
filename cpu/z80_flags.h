#ifndef Z80_FLAGS_H_
#define Z80_FLAGS_H_

#include "z80.h"

#define NIBBLE_LOW 0x0F
#define NIBBLE_HIGH 0xF0

// Z80 Flag bit positions
#define FLAG_S      7   // Sign flag
#define FLAG_Z      6   // Zero flag  
#define FLAG_5      5
#define FLAG_H      4   // Half carry flag
#define FLAG_3      3
#define FLAG_PV     2   // Parity/Overflow flag
#define FLAG_N      1   // Add/Subtract flag
#define FLAG_C      0   // Carry flag

// Flag bit masks
#define MASK_S      (1 << FLAG_S)
#define MASK_Z      (1 << FLAG_Z)
#define MASK_H      (1 << FLAG_H)
#define MASK_PV     (1 << FLAG_PV)
#define MASK_N      (1 << FLAG_N)
#define MASK_C      (1 << FLAG_C)
#define MASK_5      (1 << FLAG_5)
#define MASK_3      (1 << FLAG_3)

// Macros to test flags - return non-zero (true) if set, zero (false) if clear
#define FLAG_IS_SET(cpu, flag)      ((cpu)->registers.F & (flag))
#define FLAG_IS_CLEAR(cpu, flag)    (!((cpu)->registers.F & (flag)))

// Macros to set/clear individual flags
#define SET_FLAG(cpu, flag)         ((cpu)->registers.F |= (flag))
#define CLEAR_FLAG(cpu, flag)       ((cpu)->registers.F &= ~(flag))
#define SET_FLAG_IF(cpu, flag, condition) \
    do { \
        if (condition) SET_FLAG(cpu, flag); \
        else CLEAR_FLAG(cpu, flag); \
    } while(0)
// Convenience macros for specific flags
#define SET_S(cpu)          SET_FLAG(cpu, MASK_S)
#define SET_Z(cpu)          SET_FLAG(cpu, MASK_Z)
#define SET_H(cpu)          SET_FLAG(cpu, MASK_H)
#define SET_PV(cpu)         SET_FLAG(cpu, MASK_PV)
#define SET_N(cpu)          SET_FLAG(cpu, MASK_N)
#define SET_C(cpu)          SET_FLAG(cpu, MASK_C)

#define CLEAR_S(cpu)        CLEAR_FLAG(cpu, MASK_S)
#define CLEAR_Z(cpu)        CLEAR_FLAG(cpu, MASK_Z)
#define CLEAR_H(cpu)        CLEAR_FLAG(cpu, MASK_H)
#define CLEAR_PV(cpu)       CLEAR_FLAG(cpu, MASK_PV)
#define CLEAR_N(cpu)        CLEAR_FLAG(cpu, MASK_N)
#define CLEAR_C(cpu)        CLEAR_FLAG(cpu, MASK_C)

// Optimized flag test macros - return 1 if set, 0 if clear
#define IS_S_SET(cpu)       ((((cpu)->registers.F) >> FLAG_S) & 1)
#define IS_Z_SET(cpu)       ((((cpu)->registers.F) >> FLAG_Z) & 1)
#define IS_H_SET(cpu)       ((((cpu)->registers.F) >> FLAG_H) & 1)
#define IS_PV_SET(cpu)      ((((cpu)->registers.F) >> FLAG_PV) & 1)
#define IS_N_SET(cpu)       ((((cpu)->registers.F) >> FLAG_N) & 1)
#define IS_C_SET(cpu)       ((((cpu)->registers.F) >> FLAG_C) & 1)
#define IS_S_UNSET(cpu)     (!IS_S_SET(cpu))
#define IS_Z_UNSET(cpu)     (!IS_Z_SET(cpu))
#define IS_H_UNSET(cpu)     (!IS_H_SET(cpu))
#define IS_PV_UNSET(cpu)    (!IS_PV_SET(cpu))
#define IS_N_UNSET(cpu)     (!IS_N_SET(cpu))
#define IS_C_UNSET(cpu)     (!IS_C_SET(cpu))

void init_parity_table();
void set_flags_inc8(struct z80_t *cpu, uint8_t original, uint8_t result);
void set_flags_dec8(struct z80_t *cpu, uint8_t original, uint8_t result);
void set_flags_logical_or_xor(struct z80_t *cpu, uint8_t result);
void set_flags_logical_and(struct z80_t *cpu, uint8_t result);
void set_flags_cp(struct z80_t *cpu, uint8_t a, uint8_t b);
void set_flags_bit(struct z80_t *cpu, uint8_t bit, uint8_t r);

uint8_t set_flags_add8(struct z80_t *cpu, uint8_t a, uint8_t b);
uint8_t set_flags_sub8(struct z80_t *cpu, uint8_t a, uint8_t b);
uint8_t set_flags_adc8(struct z80_t *cpu, uint8_t a, uint8_t b);
uint8_t set_flags_sbc8(struct z80_t *cpu, uint8_t a, uint8_t b);

void set_flags_add16(struct z80_t *cpu, uint16_t a, uint16_t b, uint16_t result);
void set_flags_sub16(struct z80_t *cpu, uint16_t a, uint16_t b, uint16_t result);
void set_flags_adc16(struct z80_t *cpu, uint16_t a, uint16_t b, uint16_t result);
void set_flags_sbc16(struct z80_t *cpu, uint16_t a, uint16_t b, uint16_t result);
void set_flags_block_transfer(struct z80_t *cpu, uint16_t bc_after);
void set_flags_block_io(struct z80_t *cpu);
void set_flags_block_outi(struct z80_t *cpu);
uint8_t set_flags_cpd(struct z80_t *cpu, uint8_t r);
uint8_t set_flags_cpi(struct z80_t *cpu, uint8_t r);
uint8_t set_flags_rlca(struct z80_t *cpu);
uint8_t set_flags_rrca(struct z80_t *cpu);
uint8_t set_flags_rra(struct z80_t *cpu);
uint8_t set_flags_rla(struct z80_t *cpu);
uint8_t set_flags_rlc(struct z80_t *cpu, uint8_t r);
uint8_t set_flags_rl(struct z80_t *cpu, uint8_t r);
uint8_t set_flags_rrc(struct z80_t *cpu, uint8_t r);
uint8_t set_flags_rr(struct z80_t *cpu, uint8_t r);
uint8_t set_flags_sla(struct z80_t *cpu, uint8_t r);
uint8_t set_flags_sra(struct z80_t *cpu, uint8_t r);
uint8_t set_flags_sll(struct z80_t *cpu, uint8_t r);
uint8_t set_flags_srl(struct z80_t *cpu, uint8_t r);
uint8_t set_flags_daa(struct z80_t *cpu);
uint8_t set_flags_cpl(struct z80_t *cpu);
void set_flags_rotate_digit(struct z80_t *cpu);
void set_flags_ccf(struct z80_t *cpu);
void set_flags_scf(struct z80_t *cpu);
#endif