#include "z80.h"

void z80_op_add8(struct z80_t *cpu, uint8_t r);
void z80_op_adc8(struct z80_t *cpu, uint8_t r);
uint16_t z80_op_add16(struct z80_t *cpu, uint16_t rp, uint16_t value);
void z80_op_adc16(struct z80_t *cpu, uint16_t rr);
void z80_op_sub8(struct z80_t *cpu, uint8_t r);
void z80_op_sbc8(struct z80_t *cpu, uint8_t r);
void z80_op_sbc16(struct z80_t *cpu, uint16_t rr);
void z80_op_call(struct z80_t *cpu, bool condition);
void z80_op_ret(struct z80_t *cpu, bool condition);
void z80_op_jp(struct z80_t *cpu, bool condition);
void z80_op_jr(struct z80_t *cpu, bool condition);
uint8_t z80_op_inc8(struct z80_t *cpu, uint8_t r);
uint8_t z80_op_dec8(struct z80_t *cpu, uint8_t r);
void z80_op_cp(struct z80_t *cpu, uint8_t a, uint8_t b);
void z80_op_inc16(struct z80_t *cpu, uint16_t *r);
void z80_op_or(struct z80_t *cpu, uint8_t r);
void z80_op_xor(struct z80_t *cpu, uint8_t r);
void z80_op_and(struct z80_t *cpu, uint8_t r);
void z80_op_neg(struct z80_t *cpu);
void z80_op_rrd(struct z80_t *cpu);
void z80_op_rld(struct z80_t *cpu);
uint8_t z80_op_rl(struct z80_t *cpu, uint8_t r);
uint8_t z80_op_rlc(struct z80_t *cpu, uint8_t r);
uint8_t z80_op_rrc(struct z80_t *cpu, uint8_t r);
uint8_t z80_op_rr(struct z80_t *cpu, uint8_t r);
void z80_op_bit(struct z80_t *cpu, uint8_t bit, uint8_t r);
uint8_t z80_op_res(struct z80_t *cpu, uint8_t bit, uint8_t r);
uint8_t z80_op_set(struct z80_t *cpu, uint8_t bit, uint8_t r);
uint8_t z80_op_sla(struct z80_t *cpu, uint8_t r);
uint8_t z80_op_sra(struct z80_t *cpu, uint8_t r);
uint8_t z80_op_sll(struct z80_t *cpu, uint8_t r);
uint8_t z80_op_srl(struct z80_t *cpu, uint8_t r);
void z80_op_rst(struct z80_t *cpu, uint16_t p);