#include <stdio.h>
#include "z80_op.h"
#include "z80_flags.h"

// Z80 CB-prefixed Opcode enumeration (complete set)
enum z80_dd_cb_opcodes
{
    // ROTATE LEFT CIRCULAR (RLC) - 8-bit registers
    DD_CB_RLC_B = 0x00,  // RLC B
    DD_CB_RLC_C = 0x01,  // RLC C
    DD_CB_RLC_D = 0x02,  // RLC D
    DD_CB_RLC_E = 0x03,  // RLC E
    DD_CB_RLC_H = 0x04,  // RLC H
    DD_CB_RLC_L = 0x05,  // RLC L
    DD_CB_RLC_HL = 0x06, // RLC (HL)
    DD_CB_RLC_A = 0x07,  // RLC A

    // ROTATE RIGHT CIRCULAR (RRC) - 8-bit registers
    DD_CB_RRC_B = 0x08,  // RRC B
    DD_CB_RRC_C = 0x09,  // RRC C
    DD_CB_RRC_D = 0x0A,  // RRC D
    DD_CB_RRC_E = 0x0B,  // RRC E
    DD_CB_RRC_H = 0x0C,  // RRC H
    DD_CB_RRC_L = 0x0D,  // RRC L
    DD_CB_RRC_HL = 0x0E, // RRC (HL)
    DD_CB_RRC_A = 0x0F,  // RRC A

    // ROTATE LEFT (RL) - 8-bit registers
    DD_CB_RL_B = 0x10,  // RL B
    DD_CB_RL_C = 0x11,  // RL C
    DD_CB_RL_D = 0x12,  // RL D
    DD_CB_RL_E = 0x13,  // RL E
    DD_CB_RL_H = 0x14,  // RL H
    DD_CB_RL_L = 0x15,  // RL L
    DD_CB_RL_HL = 0x16, // RL (HL)
    DD_CB_RL_A = 0x17,  // RL A

    // ROTATE RIGHT (RR) - 8-bit registers
    DD_CB_RR_B = 0x18,  // RR B
    DD_CB_RR_C = 0x19,  // RR C
    DD_CB_RR_D = 0x1A,  // RR D
    DD_CB_RR_E = 0x1B,  // RR E
    DD_CB_RR_H = 0x1C,  // RR H
    DD_CB_RR_L = 0x1D,  // RR L
    DD_CB_RR_HL = 0x1E, // RR (HL)
    DD_CB_RR_A = 0x1F,  // RR A

    // SHIFT LEFT ARITHMETIC (SLA) - 8-bit registers
    DD_CB_SLA_B = 0x20,  // SLA B
    DD_CB_SLA_C = 0x21,  // SLA C
    DD_CB_SLA_D = 0x22,  // SLA D
    DD_CB_SLA_E = 0x23,  // SLA E
    DD_CB_SLA_H = 0x24,  // SLA H
    DD_CB_SLA_L = 0x25,  // SLA L
    DD_CB_SLA_HL = 0x26, // SLA (HL)
    DD_CB_SLA_A = 0x27,  // SLA A

    // SHIFT RIGHT ARITHMETIC (SRA) - 8-bit registers
    DD_CB_SRA_B = 0x28,  // SRA B
    DD_CB_SRA_C = 0x29,  // SRA C
    DD_CB_SRA_D = 0x2A,  // SRA D
    DD_CB_SRA_E = 0x2B,  // SRA E
    DD_CB_SRA_H = 0x2C,  // SRA H
    DD_CB_SRA_L = 0x2D,  // SRA L
    DD_CB_SRA_HL = 0x2E, // SRA (HL)
    DD_CB_SRA_A = 0x2F,  // SRA A

    // SHIFT LEFT LOGICAL (SLL) - 8-bit registers (undocumented)
    DD_CB_SLL_B = 0x30,  // SLL B
    DD_CB_SLL_C = 0x31,  // SLL C
    DD_CB_SLL_D = 0x32,  // SLL D
    DD_CB_SLL_E = 0x33,  // SLL E
    DD_CB_SLL_H = 0x34,  // SLL H
    DD_CB_SLL_L = 0x35,  // SLL L
    DD_CB_SLL_HL = 0x36, // SLL (HL)
    DD_CB_SLL_A = 0x37,  // SLL A

    // SHIFT RIGHT LOGICAL (SRL) - 8-bit registers
    DD_CB_SRL_B = 0x38,  // SRL B
    DD_CB_SRL_C = 0x39,  // SRL C
    DD_CB_SRL_D = 0x3A,  // SRL D
    DD_CB_SRL_E = 0x3B,  // SRL E
    DD_CB_SRL_H = 0x3C,  // SRL H
    DD_CB_SRL_L = 0x3D,  // SRL L
    DD_CB_SRL_HL = 0x3E, // SRL (HL)
    DD_CB_SRL_A = 0x3F,  // SRL A

    // BIT TEST - Bit 0
    DD_CB_BIT_0_B = 0x40,  // BIT 0, B
    DD_CB_BIT_0_C = 0x41,  // BIT 0, C
    DD_CB_BIT_0_D = 0x42,  // BIT 0, D
    DD_CB_BIT_0_E = 0x43,  // BIT 0, E
    DD_CB_BIT_0_H = 0x44,  // BIT 0, H
    DD_CB_BIT_0_L = 0x45,  // BIT 0, L
    DD_CB_BIT_0_HL = 0x46, // BIT 0, (HL)
    DD_CB_BIT_0_A = 0x47,  // BIT 0, A

    // BIT TEST - Bit 1
    DD_CB_BIT_1_B = 0x48,  // BIT 1, B
    DD_CB_BIT_1_C = 0x49,  // BIT 1, C
    DD_CB_BIT_1_D = 0x4A,  // BIT 1, D
    DD_CB_BIT_1_E = 0x4B,  // BIT 1, E
    DD_CB_BIT_1_H = 0x4C,  // BIT 1, H
    DD_CB_BIT_1_L = 0x4D,  // BIT 1, L
    DD_CB_BIT_1_HL = 0x4E, // BIT 1, (HL)
    DD_CB_BIT_1_A = 0x4F,  // BIT 1, A

    // BIT TEST - Bit 2
    DD_CB_BIT_2_B = 0x50,  // BIT 2, B
    DD_CB_BIT_2_C = 0x51,  // BIT 2, C
    DD_CB_BIT_2_D = 0x52,  // BIT 2, D
    DD_CB_BIT_2_E = 0x53,  // BIT 2, E
    DD_CB_BIT_2_H = 0x54,  // BIT 2, H
    DD_CB_BIT_2_L = 0x55,  // BIT 2, L
    DD_CB_BIT_2_HL = 0x56, // BIT 2, (HL)
    DD_CB_BIT_2_A = 0x57,  // BIT 2, A

    // BIT TEST - Bit 3
    DD_CB_BIT_3_B = 0x58,  // BIT 3, B
    DD_CB_BIT_3_C = 0x59,  // BIT 3, C
    DD_CB_BIT_3_D = 0x5A,  // BIT 3, D
    DD_CB_BIT_3_E = 0x5B,  // BIT 3, E
    DD_CB_BIT_3_H = 0x5C,  // BIT 3, H
    DD_CB_BIT_3_L = 0x5D,  // BIT 3, L
    DD_CB_BIT_3_HL = 0x5E, // BIT 3, (HL)
    DD_CB_BIT_3_A = 0x5F,  // BIT 3, A

    // BIT TEST - Bit 4
    DD_CB_BIT_4_B = 0x60,  // BIT 4, B
    DD_CB_BIT_4_C = 0x61,  // BIT 4, C
    DD_CB_BIT_4_D = 0x62,  // BIT 4, D
    DD_CB_BIT_4_E = 0x63,  // BIT 4, E
    DD_CB_BIT_4_H = 0x64,  // BIT 4, H
    DD_CB_BIT_4_L = 0x65,  // BIT 4, L
    DD_CB_BIT_4_HL = 0x66, // BIT 4, (HL)
    DD_CB_BIT_4_A = 0x67,  // BIT 4, A

    // BIT TEST - Bit 5
    DD_CB_BIT_5_B = 0x68,  // BIT 5, B
    DD_CB_BIT_5_C = 0x69,  // BIT 5, C
    DD_CB_BIT_5_D = 0x6A,  // BIT 5, D
    DD_CB_BIT_5_E = 0x6B,  // BIT 5, E
    DD_CB_BIT_5_H = 0x6C,  // BIT 5, H
    DD_CB_BIT_5_L = 0x6D,  // BIT 5, L
    DD_CB_BIT_5_HL = 0x6E, // BIT 5, (HL)
    DD_CB_BIT_5_A = 0x6F,  // BIT 5, A

    // BIT TEST - Bit 6
    DD_CB_BIT_6_B = 0x70,  // BIT 6, B
    DD_CB_BIT_6_C = 0x71,  // BIT 6, C
    DD_CB_BIT_6_D = 0x72,  // BIT 6, D
    DD_CB_BIT_6_E = 0x73,  // BIT 6, E
    DD_CB_BIT_6_H = 0x74,  // BIT 6, H
    DD_CB_BIT_6_L = 0x75,  // BIT 6, L
    DD_CB_BIT_6_HL = 0x76, // BIT 6, (HL)
    DD_CB_BIT_6_A = 0x77,  // BIT 6, A

    // BIT TEST - Bit 7
    DD_CB_BIT_7_B = 0x78,  // BIT 7, B
    DD_CB_BIT_7_C = 0x79,  // BIT 7, C
    DD_CB_BIT_7_D = 0x7A,  // BIT 7, D
    DD_CB_BIT_7_E = 0x7B,  // BIT 7, E
    DD_CB_BIT_7_H = 0x7C,  // BIT 7, H
    DD_CB_BIT_7_L = 0x7D,  // BIT 7, L
    DD_CB_BIT_7_HL = 0x7E, // BIT 7, (HL)
    DD_CB_BIT_7_A = 0x7F,  // BIT 7, A

    // BIT RESET - Bit 0
    DD_CB_RES_0_B = 0x80,  // RES 0, B
    DD_CB_RES_0_C = 0x81,  // RES 0, C
    DD_CB_RES_0_D = 0x82,  // RES 0, D
    DD_CB_RES_0_E = 0x83,  // RES 0, E
    DD_CB_RES_0_H = 0x84,  // RES 0, H
    DD_CB_RES_0_L = 0x85,  // RES 0, L
    DD_CB_RES_0_HL = 0x86, // RES 0, (HL)
    DD_CB_RES_0_A = 0x87,  // RES 0, A

    // BIT RESET - Bit 1
    DD_CB_RES_1_B = 0x88,  // RES 1, B
    DD_CB_RES_1_C = 0x89,  // RES 1, C
    DD_CB_RES_1_D = 0x8A,  // RES 1, D
    DD_CB_RES_1_E = 0x8B,  // RES 1, E
    DD_CB_RES_1_H = 0x8C,  // RES 1, H
    DD_CB_RES_1_L = 0x8D,  // RES 1, L
    DD_CB_RES_1_HL = 0x8E, // RES 1, (HL)
    DD_CB_RES_1_A = 0x8F,  // RES 1, A

    // BIT RESET - Bit 2
    DD_CB_RES_2_B = 0x90,  // RES 2, B
    DD_CB_RES_2_C = 0x91,  // RES 2, C
    DD_CB_RES_2_D = 0x92,  // RES 2, D
    DD_CB_RES_2_E = 0x93,  // RES 2, E
    DD_CB_RES_2_H = 0x94,  // RES 2, H
    DD_CB_RES_2_L = 0x95,  // RES 2, L
    DD_CB_RES_2_HL = 0x96, // RES 2, (HL)
    DD_CB_RES_2_A = 0x97,  // RES 2, A

    // BIT RESET - Bit 3
    DD_CB_RES_3_B = 0x98,  // RES 3, B
    DD_CB_RES_3_C = 0x99,  // RES 3, C
    DD_CB_RES_3_D = 0x9A,  // RES 3, D
    DD_CB_RES_3_E = 0x9B,  // RES 3, E
    DD_CB_RES_3_H = 0x9C,  // RES 3, H
    DD_CB_RES_3_L = 0x9D,  // RES 3, L
    DD_CB_RES_3_HL = 0x9E, // RES 3, (HL)
    DD_CB_RES_3_A = 0x9F,  // RES 3, A

    // BIT RESET - Bit 4
    DD_CB_RES_4_B = 0xA0,  // RES 4, B
    DD_CB_RES_4_C = 0xA1,  // RES 4, C
    DD_CB_RES_4_D = 0xA2,  // RES 4, D
    DD_CB_RES_4_E = 0xA3,  // RES 4, E
    DD_CB_RES_4_H = 0xA4,  // RES 4, H
    DD_CB_RES_4_L = 0xA5,  // RES 4, L
    DD_CB_RES_4_HL = 0xA6, // RES 4, (HL)
    DD_CB_RES_4_A = 0xA7,  // RES 4, A

    // BIT RESET - Bit 5
    DD_CB_RES_5_B = 0xA8,  // RES 5, B
    DD_CB_RES_5_C = 0xA9,  // RES 5, C
    DD_CB_RES_5_D = 0xAA,  // RES 5, D
    DD_CB_RES_5_E = 0xAB,  // RES 5, E
    DD_CB_RES_5_H = 0xAC,  // RES 5, H
    DD_CB_RES_5_L = 0xAD,  // RES 5, L
    DD_CB_RES_5_HL = 0xAE, // RES 5, (HL)
    DD_CB_RES_5_A = 0xAF,  // RES 5, A

    // BIT RESET - Bit 6
    DD_CB_RES_6_B = 0xB0,  // RES 6, B
    DD_CB_RES_6_C = 0xB1,  // RES 6, C
    DD_CB_RES_6_D = 0xB2,  // RES 6, D
    DD_CB_RES_6_E = 0xB3,  // RES 6, E
    DD_CB_RES_6_H = 0xB4,  // RES 6, H
    DD_CB_RES_6_L = 0xB5,  // RES 6, L
    DD_CB_RES_6_HL = 0xB6, // RES 6, (HL)
    DD_CB_RES_6_A = 0xB7,  // RES 6, A

    // BIT RESET - Bit 7
    DD_CB_RES_7_B = 0xB8,  // RES 7, B
    DD_CB_RES_7_C = 0xB9,  // RES 7, C
    DD_CB_RES_7_D = 0xBA,  // RES 7, D
    DD_CB_RES_7_E = 0xBB,  // RES 7, E
    DD_CB_RES_7_H = 0xBC,  // RES 7, H
    DD_CB_RES_7_L = 0xBD,  // RES 7, L
    DD_CB_RES_7_HL = 0xBE, // RES 7, (HL)
    DD_CB_RES_7_A = 0xBF,  // RES 7, A

    // BIT SET - Bit 0
    DD_CB_SET_0_B = 0xC0,  // SET 0, B
    DD_CB_SET_0_C = 0xC1,  // SET 0, C
    DD_CB_SET_0_D = 0xC2,  // SET 0, D
    DD_CB_SET_0_E = 0xC3,  // SET 0, E
    DD_CB_SET_0_H = 0xC4,  // SET 0, H
    DD_CB_SET_0_L = 0xC5,  // SET 0, L
    DD_CB_SET_0_HL = 0xC6, // SET 0, (HL)
    DD_CB_SET_0_A = 0xC7,  // SET 0, A

    // BIT SET - Bit 1
    DD_CB_SET_1_B = 0xC8,  // SET 1, B
    DD_CB_SET_1_C = 0xC9,  // SET 1, C
    DD_CB_SET_1_D = 0xCA,  // SET 1, D
    DD_CB_SET_1_E = 0xCB,  // SET 1, E
    DD_CB_SET_1_H = 0xCC,  // SET 1, H
    DD_CB_SET_1_L = 0xCD,  // SET 1, L
    DD_CB_SET_1_HL = 0xCE, // SET 1, (HL)
    DD_CB_SET_1_A = 0xCF,  // SET 1, A

    // BIT SET - Bit 2
    DD_CB_SET_2_B = 0xD0,  // SET 2, B
    DD_CB_SET_2_C = 0xD1,  // SET 2, C
    DD_CB_SET_2_D = 0xD2,  // SET 2, D
    DD_CB_SET_2_E = 0xD3,  // SET 2, E
    DD_CB_SET_2_H = 0xD4,  // SET 2, H
    DD_CB_SET_2_L = 0xD5,  // SET 2, L
    DD_CB_SET_2_HL = 0xD6, // SET 2, (HL)
    DD_CB_SET_2_A = 0xD7,  // SET 2, A

    // BIT SET - Bit 3
    DD_CB_SET_3_B = 0xD8,  // SET 3, B
    DD_CB_SET_3_C = 0xD9,  // SET 3, C
    DD_CB_SET_3_D = 0xDA,  // SET 3, D
    DD_CB_SET_3_E = 0xDB,  // SET 3, E
    DD_CB_SET_3_H = 0xDC,  // SET 3, H
    DD_CB_SET_3_L = 0xDD,  // SET 3, L
    DD_CB_SET_3_HL = 0xDE, // SET 3, (HL)
    DD_CB_SET_3_A = 0xDF,  // SET 3, A

    // BIT SET - Bit 4
    DD_CB_SET_4_B = 0xE0,  // SET 4, B
    DD_CB_SET_4_C = 0xE1,  // SET 4, C
    DD_CB_SET_4_D = 0xE2,  // SET 4, D
    DD_CB_SET_4_E = 0xE3,  // SET 4, E
    DD_CB_SET_4_H = 0xE4,  // SET 4, H
    DD_CB_SET_4_L = 0xE5,  // SET 4, L
    DD_CB_SET_4_HL = 0xE6, // SET 4, (HL)
    DD_CB_SET_4_A = 0xE7,  // SET 4, A

    // BIT SET - Bit 5
    DD_CB_SET_5_B = 0xE8,  // SET 5, B
    DD_CB_SET_5_C = 0xE9,  // SET 5, C
    DD_CB_SET_5_D = 0xEA,  // SET 5, D
    DD_CB_SET_5_E = 0xEB,  // SET 5, E
    DD_CB_SET_5_H = 0xEC,  // SET 5, H
    DD_CB_SET_5_L = 0xED,  // SET 5, L
    DD_CB_SET_5_HL = 0xEE, // SET 5, (HL)
    DD_CB_SET_5_A = 0xEF,  // SET 5, A

    // BIT SET - Bit 6
    DD_CB_SET_6_B = 0xF0,  // SET 6, B
    DD_CB_SET_6_C = 0xF1,  // SET 6, C
    DD_CB_SET_6_D = 0xF2,  // SET 6, D
    DD_CB_SET_6_E = 0xF3,  // SET 6, E
    DD_CB_SET_6_H = 0xF4,  // SET 6, H
    DD_CB_SET_6_L = 0xF5,  // SET 6, L
    DD_CB_SET_6_HL = 0xF6, // SET 6, (HL)
    DD_CB_SET_6_A = 0xF7,  // SET 6, A

    // BIT SET - Bit 7
    DD_CB_SET_7_B = 0xF8,  // SET 7, B
    DD_CB_SET_7_C = 0xF9,  // SET 7, C
    DD_CB_SET_7_D = 0xFA,  // SET 7, D
    DD_CB_SET_7_E = 0xFB,  // SET 7, E
    DD_CB_SET_7_H = 0xFC,  // SET 7, H
    DD_CB_SET_7_L = 0xFD,  // SET 7, L
    DD_CB_SET_7_HL = 0xFE, // SET 7, (HL)
    DD_CB_SET_7_A = 0xFF   // SET 7, A
};

void z80_execute_dd_cb_instruction(struct z80_t *cpu, int8_t d)
{
    uint8_t cb_opcode = z80_fetch8(cpu);
    switch (cb_opcode)
    {
    // ROTATE LEFT CIRCULAR (RLC) - 8-bit registers
    case DD_CB_RLC_B: cpu->registers.B = z80_op_rlc(cpu, z80_read8(cpu, (cpu->registers.IX + d))); break;
    case DD_CB_RLC_C: cpu->registers.C = z80_op_rlc(cpu, z80_read8(cpu, (cpu->registers.IX + d))); break;
    case DD_CB_RLC_D: cpu->registers.D = z80_op_rlc(cpu, z80_read8(cpu, (cpu->registers.IX + d))); break;
    case DD_CB_RLC_E: cpu->registers.E = z80_op_rlc(cpu, z80_read8(cpu, (cpu->registers.IX + d))); break;
    case DD_CB_RLC_H: cpu->registers.H = z80_op_rlc(cpu, z80_read8(cpu, (cpu->registers.IX + d))); break;
    case DD_CB_RLC_L: cpu->registers.L = z80_op_rlc(cpu, z80_read8(cpu, (cpu->registers.IX + d))); break;
    case DD_CB_RLC_HL: z80_write8(cpu, (cpu->registers.IX + d), z80_op_rlc(cpu, z80_read8(cpu, (cpu->registers.IX + d)))); break;
    case DD_CB_RLC_A: cpu->registers.A = z80_op_rlc(cpu, z80_read8(cpu, (cpu->registers.IX + d))); break;

    // ROTATE RIGHT CIRCULAR (RRC) - 8-bit registers
    case DD_CB_RRC_B: cpu->registers.B = z80_op_rrc(cpu, z80_read8(cpu, (cpu->registers.IX + d))); break;
    case DD_CB_RRC_C: cpu->registers.C = z80_op_rrc(cpu, z80_read8(cpu, (cpu->registers.IX + d))); break;
    case DD_CB_RRC_D: cpu->registers.D = z80_op_rrc(cpu, z80_read8(cpu, (cpu->registers.IX + d))); break;
    case DD_CB_RRC_E: cpu->registers.E = z80_op_rrc(cpu, z80_read8(cpu, (cpu->registers.IX + d))); break;
    case DD_CB_RRC_H: cpu->registers.H = z80_op_rrc(cpu, z80_read8(cpu, (cpu->registers.IX + d))); break;
    case DD_CB_RRC_L: cpu->registers.L = z80_op_rrc(cpu, z80_read8(cpu, (cpu->registers.IX + d))); break;
    case DD_CB_RRC_HL: z80_write8(cpu, (cpu->registers.IX + d), z80_op_rrc(cpu, z80_read8(cpu, (cpu->registers.IX + d)))); break;
    case DD_CB_RRC_A: cpu->registers.A = z80_op_rrc(cpu, z80_read8(cpu, (cpu->registers.IX + d))); break;

    // ROTATE LEFT (RL) - 8-bit registers
    case DD_CB_RL_B: cpu->registers.B = z80_op_rl(cpu, z80_read8(cpu, (cpu->registers.IX + d))); break;
    case DD_CB_RL_C: cpu->registers.C = z80_op_rl(cpu, z80_read8(cpu, (cpu->registers.IX + d))); break;
    case DD_CB_RL_D: cpu->registers.D = z80_op_rl(cpu, z80_read8(cpu, (cpu->registers.IX + d))); break;
    case DD_CB_RL_E: cpu->registers.E = z80_op_rl(cpu, z80_read8(cpu, (cpu->registers.IX + d))); break;
    case DD_CB_RL_H: cpu->registers.H = z80_op_rl(cpu, z80_read8(cpu, (cpu->registers.IX + d))); break;
    case DD_CB_RL_L: cpu->registers.L = z80_op_rl(cpu, z80_read8(cpu, (cpu->registers.IX + d))); break;
    case DD_CB_RL_HL: z80_write8(cpu, (cpu->registers.IX + d), z80_op_rl(cpu, z80_read8(cpu, (cpu->registers.IX + d)))); break;
    case DD_CB_RL_A: cpu->registers.A = z80_op_rl(cpu, z80_read8(cpu, (cpu->registers.IX + d))); break;

    // ROTATE RIGHT (RR) - 8-bit registers
    case DD_CB_RR_B: cpu->registers.B = z80_op_rr(cpu, z80_read8(cpu, (cpu->registers.IX + d))); break;
    case DD_CB_RR_C: cpu->registers.C = z80_op_rr(cpu, z80_read8(cpu, (cpu->registers.IX + d))); break;
    case DD_CB_RR_D: cpu->registers.D = z80_op_rr(cpu, z80_read8(cpu, (cpu->registers.IX + d))); break;
    case DD_CB_RR_E: cpu->registers.E = z80_op_rr(cpu, z80_read8(cpu, (cpu->registers.IX + d))); break;
    case DD_CB_RR_H: cpu->registers.H = z80_op_rr(cpu, z80_read8(cpu, (cpu->registers.IX + d))); break;
    case DD_CB_RR_L: cpu->registers.L = z80_op_rr(cpu, z80_read8(cpu, (cpu->registers.IX + d))); break;
    case DD_CB_RR_HL: z80_write8(cpu, (cpu->registers.IX + d), z80_op_rr(cpu, z80_read8(cpu, (cpu->registers.IX + d)))); break;
    case DD_CB_RR_A: cpu->registers.A = z80_op_rr(cpu, z80_read8(cpu, (cpu->registers.IX + d))); break;

    // SHIFT LEFT ARITHMETIC (SLA) - 8-bit registers
    case DD_CB_SLA_B: cpu->registers.B = z80_op_sla(cpu, z80_read8(cpu, (cpu->registers.IX + d))); break;
    case DD_CB_SLA_C: cpu->registers.C = z80_op_sla(cpu, z80_read8(cpu, (cpu->registers.IX + d))); break;
    case DD_CB_SLA_D: cpu->registers.D = z80_op_sla(cpu, z80_read8(cpu, (cpu->registers.IX + d))); break;
    case DD_CB_SLA_E: cpu->registers.E = z80_op_sla(cpu, z80_read8(cpu, (cpu->registers.IX + d))); break;
    case DD_CB_SLA_H: cpu->registers.H = z80_op_sla(cpu, z80_read8(cpu, (cpu->registers.IX + d))); break;
    case DD_CB_SLA_L: cpu->registers.L = z80_op_sla(cpu, z80_read8(cpu, (cpu->registers.IX + d))); break;
    case DD_CB_SLA_HL: z80_write8(cpu, (cpu->registers.IX + d), z80_op_sla(cpu, z80_read8(cpu, (cpu->registers.IX + d)))); break;
    case DD_CB_SLA_A: cpu->registers.A = z80_op_sla(cpu, z80_read8(cpu, (cpu->registers.IX + d))); break;

    // SHIFT RIGHT ARITHMETIC (SRA) - 8-bit registers
    case DD_CB_SRA_B: cpu->registers.B = z80_op_sra(cpu, z80_read8(cpu, (cpu->registers.IX + d))); break;
    case DD_CB_SRA_C: cpu->registers.C = z80_op_sra(cpu, z80_read8(cpu, (cpu->registers.IX + d))); break;
    case DD_CB_SRA_D: cpu->registers.D = z80_op_sra(cpu, z80_read8(cpu, (cpu->registers.IX + d))); break;
    case DD_CB_SRA_E: cpu->registers.E = z80_op_sra(cpu, z80_read8(cpu, (cpu->registers.IX + d))); break;
    case DD_CB_SRA_H: cpu->registers.H = z80_op_sra(cpu, z80_read8(cpu, (cpu->registers.IX + d))); break;
    case DD_CB_SRA_L: cpu->registers.L = z80_op_sra(cpu, z80_read8(cpu, (cpu->registers.IX + d))); break;
    case DD_CB_SRA_HL: z80_write8(cpu, (cpu->registers.IX + d), z80_op_sra(cpu, z80_read8(cpu, (cpu->registers.IX + d)))); break;
    case DD_CB_SRA_A: cpu->registers.A = z80_op_sra(cpu, z80_read8(cpu, (cpu->registers.IX + d))); break;

    // SHIFT LEFT LOGICAL (SLL) - 8-bit registers (undocumented)
    case DD_CB_SLL_B: cpu->registers.B = z80_op_sll(cpu, z80_read8(cpu, (cpu->registers.IX + d))); break;
    case DD_CB_SLL_C: cpu->registers.C = z80_op_sll(cpu, z80_read8(cpu, (cpu->registers.IX + d))); break;
    case DD_CB_SLL_D: cpu->registers.D = z80_op_sll(cpu, z80_read8(cpu, (cpu->registers.IX + d))); break;
    case DD_CB_SLL_E: cpu->registers.E = z80_op_sll(cpu, z80_read8(cpu, (cpu->registers.IX + d))); break;
    case DD_CB_SLL_H: cpu->registers.H = z80_op_sll(cpu, z80_read8(cpu, (cpu->registers.IX + d))); break;
    case DD_CB_SLL_L: cpu->registers.L = z80_op_sll(cpu, z80_read8(cpu, (cpu->registers.IX + d))); break;
    case DD_CB_SLL_HL: z80_write8(cpu, (cpu->registers.IX + d), z80_op_sll(cpu, z80_read8(cpu, (cpu->registers.IX + d)))); break;
    case DD_CB_SLL_A: cpu->registers.A = z80_op_sll(cpu, z80_read8(cpu, (cpu->registers.IX + d))); break;

    // SHIFT RIGHT LOGICAL (SRL) - 8-bit registers
    case DD_CB_SRL_B: cpu->registers.B = z80_op_srl(cpu, z80_read8(cpu, (cpu->registers.IX + d))); break;
    case DD_CB_SRL_C: cpu->registers.C = z80_op_srl(cpu, z80_read8(cpu, (cpu->registers.IX + d))); break;
    case DD_CB_SRL_D: cpu->registers.D = z80_op_srl(cpu, z80_read8(cpu, (cpu->registers.IX + d))); break;
    case DD_CB_SRL_E: cpu->registers.E = z80_op_srl(cpu, z80_read8(cpu, (cpu->registers.IX + d))); break;
    case DD_CB_SRL_H: cpu->registers.H = z80_op_srl(cpu, z80_read8(cpu, (cpu->registers.IX + d))); break;
    case DD_CB_SRL_L: cpu->registers.L = z80_op_srl(cpu, z80_read8(cpu, (cpu->registers.IX + d))); break;
    case DD_CB_SRL_HL: z80_write8(cpu, (cpu->registers.IX + d), z80_op_srl(cpu, z80_read8(cpu, (cpu->registers.IX + d)))); break;
    case DD_CB_SRL_A: cpu->registers.A = z80_op_srl(cpu, z80_read8(cpu, (cpu->registers.IX + d))); break;

    // BIT TEST - Bit 0
    case DD_CB_BIT_0_B: 
    case DD_CB_BIT_0_C: 
    case DD_CB_BIT_0_D: 
    case DD_CB_BIT_0_E: 
    case DD_CB_BIT_0_H: 
    case DD_CB_BIT_0_L: 
    case DD_CB_BIT_0_HL: 
    case DD_CB_BIT_0_A: z80_op_bit(cpu, 0, z80_read8(cpu, (uint16_t)(cpu->registers.IX + d))); break;

    // BIT TEST - Bit 1
    case DD_CB_BIT_1_B: 
    case DD_CB_BIT_1_C: 
    case DD_CB_BIT_1_D: 
    case DD_CB_BIT_1_E: 
    case DD_CB_BIT_1_H: 
    case DD_CB_BIT_1_L: 
    case DD_CB_BIT_1_HL: 
    case DD_CB_BIT_1_A: z80_op_bit(cpu, 1, z80_read8(cpu, (uint16_t)(cpu->registers.IX + d))); break;

    // BIT TEST - Bit 2
    case DD_CB_BIT_2_B: 
    case DD_CB_BIT_2_C: 
    case DD_CB_BIT_2_D: 
    case DD_CB_BIT_2_E: 
    case DD_CB_BIT_2_H: 
    case DD_CB_BIT_2_L: 
    case DD_CB_BIT_2_HL: 
    case DD_CB_BIT_2_A: z80_op_bit(cpu, 2, z80_read8(cpu, (uint16_t)(cpu->registers.IX + d))); break;

    // BIT TEST - Bit 3
    case DD_CB_BIT_3_B: 
    case DD_CB_BIT_3_C: 
    case DD_CB_BIT_3_D: 
    case DD_CB_BIT_3_E: 
    case DD_CB_BIT_3_H: 
    case DD_CB_BIT_3_L: 
    case DD_CB_BIT_3_HL:
    case DD_CB_BIT_3_A: z80_op_bit(cpu, 3, z80_read8(cpu, (uint16_t)(cpu->registers.IX + d))); break;

    // BIT TEST - Bit 4
    case DD_CB_BIT_4_B: 
    case DD_CB_BIT_4_C: 
    case DD_CB_BIT_4_D: 
    case DD_CB_BIT_4_E: 
    case DD_CB_BIT_4_H: 
    case DD_CB_BIT_4_L: 
    case DD_CB_BIT_4_HL: 
    case DD_CB_BIT_4_A: z80_op_bit(cpu, 4, z80_read8(cpu, (uint16_t)(cpu->registers.IX + d))); break;

    // BIT TEST - Bit 5
    case DD_CB_BIT_5_B: 
    case DD_CB_BIT_5_C: 
    case DD_CB_BIT_5_D: 
    case DD_CB_BIT_5_E: 
    case DD_CB_BIT_5_H: 
    case DD_CB_BIT_5_L: 
    case DD_CB_BIT_5_HL: 
    case DD_CB_BIT_5_A: z80_op_bit(cpu, 5, z80_read8(cpu, (uint16_t)(cpu->registers.IX + d))); break;

    // BIT TEST - Bit 6
    case DD_CB_BIT_6_B: 
    case DD_CB_BIT_6_C: 
    case DD_CB_BIT_6_D: 
    case DD_CB_BIT_6_E: 
    case DD_CB_BIT_6_H: 
    case DD_CB_BIT_6_L: 
    case DD_CB_BIT_6_HL: 
    case DD_CB_BIT_6_A: z80_op_bit(cpu, 6, z80_read8(cpu, (uint16_t)(cpu->registers.IX + d))); break;

    // BIT TEST - Bit 7
    case DD_CB_BIT_7_B: 
    case DD_CB_BIT_7_C: 
    case DD_CB_BIT_7_D: 
    case DD_CB_BIT_7_E: 
    case DD_CB_BIT_7_H: 
    case DD_CB_BIT_7_L: 
    case DD_CB_BIT_7_HL: 
    case DD_CB_BIT_7_A: z80_op_bit(cpu, 7, z80_read8(cpu, (uint16_t)(cpu->registers.IX + d))); break;

    // BIT RESET - Bit 0
    case DD_CB_RES_0_B: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.B = z80_op_res(cpu, 0, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_RES_0_C: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.C = z80_op_res(cpu, 0, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_RES_0_D: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.D = z80_op_res(cpu, 0, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_RES_0_E: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.E = z80_op_res(cpu, 0, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_RES_0_H: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.H = z80_op_res(cpu, 0, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_RES_0_L: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.L = z80_op_res(cpu, 0, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_RES_0_HL: { z80_write8(cpu, cpu->registers.IX + d, z80_op_res(cpu, 0, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_RES_0_A: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.A = z80_op_res(cpu, 0, z80_read8(cpu, cpu->registers.IX + d)));} break;

    // BIT RESET - Bit 1
    case DD_CB_RES_1_B: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.B = z80_op_res(cpu, 1, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_RES_1_C: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.C = z80_op_res(cpu, 1, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_RES_1_D: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.D = z80_op_res(cpu, 1, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_RES_1_E: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.E = z80_op_res(cpu, 1, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_RES_1_H: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.H = z80_op_res(cpu, 1, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_RES_1_L: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.L = z80_op_res(cpu, 1, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_RES_1_HL: { z80_write8(cpu, cpu->registers.IX + d, z80_op_res(cpu, 1, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_RES_1_A: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.A = z80_op_res(cpu, 1, z80_read8(cpu, cpu->registers.IX + d)));} break;

    // BIT RESET - Bit 2
    case DD_CB_RES_2_B: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.B = z80_op_res(cpu, 2, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_RES_2_C: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.C = z80_op_res(cpu, 2, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_RES_2_D: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.D = z80_op_res(cpu, 2, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_RES_2_E: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.E = z80_op_res(cpu, 2, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_RES_2_H: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.H = z80_op_res(cpu, 2, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_RES_2_L: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.L = z80_op_res(cpu, 2, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_RES_2_HL: { z80_write8(cpu, cpu->registers.IX + d, z80_op_res(cpu, 2, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_RES_2_A: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.A = z80_op_res(cpu, 2, z80_read8(cpu, cpu->registers.IX + d)));} break;

    // BIT RESET - Bit 3
    case DD_CB_RES_3_B: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.B = z80_op_res(cpu, 3, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_RES_3_C: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.C = z80_op_res(cpu, 3, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_RES_3_D: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.D = z80_op_res(cpu, 3, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_RES_3_E: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.E = z80_op_res(cpu, 3, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_RES_3_H: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.H = z80_op_res(cpu, 3, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_RES_3_L: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.L = z80_op_res(cpu, 3, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_RES_3_HL: { z80_write8(cpu, cpu->registers.IX + d, z80_op_res(cpu, 3, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_RES_3_A: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.A = z80_op_res(cpu, 3, z80_read8(cpu, cpu->registers.IX + d)));} break;

    // BIT RESET - Bit 4
    case DD_CB_RES_4_B: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.B = z80_op_res(cpu, 4, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_RES_4_C: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.C = z80_op_res(cpu, 4, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_RES_4_D: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.D = z80_op_res(cpu, 4, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_RES_4_E: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.E = z80_op_res(cpu, 4, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_RES_4_H: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.H = z80_op_res(cpu, 4, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_RES_4_L: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.L = z80_op_res(cpu, 4, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_RES_4_HL: { z80_write8(cpu, cpu->registers.IX + d, z80_op_res(cpu, 4, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_RES_4_A: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.A = z80_op_res(cpu, 4, z80_read8(cpu, cpu->registers.IX + d)));} break;

    // BIT RESET - Bit 5
    case DD_CB_RES_5_B: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.B = z80_op_res(cpu, 5, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_RES_5_C: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.C = z80_op_res(cpu, 5, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_RES_5_D: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.D = z80_op_res(cpu, 5, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_RES_5_E: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.E = z80_op_res(cpu, 5, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_RES_5_H: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.H = z80_op_res(cpu, 5, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_RES_5_L: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.L = z80_op_res(cpu, 5, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_RES_5_HL: { z80_write8(cpu, cpu->registers.IX + d, z80_op_res(cpu, 5, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_RES_5_A: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.A = z80_op_res(cpu, 5, z80_read8(cpu, cpu->registers.IX + d)));} break;

    // BIT RESET - Bit 6
    case DD_CB_RES_6_B: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.B = z80_op_res(cpu, 6, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_RES_6_C: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.C = z80_op_res(cpu, 6, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_RES_6_D: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.D = z80_op_res(cpu, 6, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_RES_6_E: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.E = z80_op_res(cpu, 6, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_RES_6_H: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.H = z80_op_res(cpu, 6, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_RES_6_L: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.L = z80_op_res(cpu, 6, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_RES_6_HL: { z80_write8(cpu, cpu->registers.IX + d, z80_op_res(cpu, 6, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_RES_6_A: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.A = z80_op_res(cpu, 6, z80_read8(cpu, cpu->registers.IX + d)));} break;

    // BIT RESET - Bit 7
    case DD_CB_RES_7_B: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.B = z80_op_res(cpu, 7, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_RES_7_C: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.C = z80_op_res(cpu, 7, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_RES_7_D: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.D = z80_op_res(cpu, 7, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_RES_7_E: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.E = z80_op_res(cpu, 7, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_RES_7_H: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.H = z80_op_res(cpu, 7, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_RES_7_L: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.L = z80_op_res(cpu, 7, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_RES_7_HL: { z80_write8(cpu, cpu->registers.IX + d, z80_op_res(cpu, 7, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_RES_7_A: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.A = z80_op_res(cpu, 7, z80_read8(cpu, cpu->registers.IX + d)));} break;

    // BIT SET - Bit 0
    case DD_CB_SET_0_B: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.B = z80_op_set(cpu, 0, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_SET_0_C: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.C = z80_op_set(cpu, 0, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_SET_0_D: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.D = z80_op_set(cpu, 0, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_SET_0_E: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.E = z80_op_set(cpu, 0, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_SET_0_H: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.H = z80_op_set(cpu, 0, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_SET_0_L: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.L = z80_op_set(cpu, 0, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_SET_0_HL: { z80_write8(cpu, cpu->registers.IX + d, z80_op_set(cpu, 0, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_SET_0_A: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.A = z80_op_set(cpu, 0, z80_read8(cpu, cpu->registers.IX + d)));} break;

    // BIT SET - Bit 1
    case DD_CB_SET_1_B: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.B = z80_op_set(cpu, 1, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_SET_1_C: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.C = z80_op_set(cpu, 1, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_SET_1_D: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.D = z80_op_set(cpu, 1, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_SET_1_E: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.E = z80_op_set(cpu, 1, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_SET_1_H: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.H = z80_op_set(cpu, 1, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_SET_1_L: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.L = z80_op_set(cpu, 1, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_SET_1_HL: { z80_write8(cpu, cpu->registers.IX + d, z80_op_set(cpu, 1, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_SET_1_A: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.A = z80_op_set(cpu, 1, z80_read8(cpu, cpu->registers.IX + d)));} break;

    // BIT SET - Bit 2
    case DD_CB_SET_2_B: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.B = z80_op_set(cpu, 2, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_SET_2_C: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.C = z80_op_set(cpu, 2, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_SET_2_D: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.D = z80_op_set(cpu, 2, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_SET_2_E: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.E = z80_op_set(cpu, 2, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_SET_2_H: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.H = z80_op_set(cpu, 2, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_SET_2_L: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.L = z80_op_set(cpu, 2, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_SET_2_HL: { z80_write8(cpu, cpu->registers.IX + d, z80_op_set(cpu, 2, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_SET_2_A: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.A = z80_op_set(cpu, 2, z80_read8(cpu, cpu->registers.IX + d)));} break;

    // BIT SET - Bit 3
    case DD_CB_SET_3_B: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.B = z80_op_set(cpu, 3, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_SET_3_C: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.C = z80_op_set(cpu, 3, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_SET_3_D: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.D = z80_op_set(cpu, 3, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_SET_3_E: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.E = z80_op_set(cpu, 3, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_SET_3_H: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.H = z80_op_set(cpu, 3, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_SET_3_L: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.L = z80_op_set(cpu, 3, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_SET_3_HL: { z80_write8(cpu, cpu->registers.IX + d, z80_op_set(cpu, 3, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_SET_3_A: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.A = z80_op_set(cpu, 3, z80_read8(cpu, cpu->registers.IX + d)));} break;

    // BIT SET - Bit 4
    case DD_CB_SET_4_B: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.B = z80_op_set(cpu, 4, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_SET_4_C: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.C = z80_op_set(cpu, 4, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_SET_4_D: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.D = z80_op_set(cpu, 4, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_SET_4_E: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.E = z80_op_set(cpu, 4, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_SET_4_H: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.H = z80_op_set(cpu, 4, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_SET_4_L: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.L = z80_op_set(cpu, 4, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_SET_4_HL: { z80_write8(cpu, cpu->registers.IX + d, z80_op_set(cpu, 4, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_SET_4_A: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.A = z80_op_set(cpu, 4, z80_read8(cpu, cpu->registers.IX + d)));} break;

    // BIT SET - Bit 5
    case DD_CB_SET_5_B: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.B = z80_op_set(cpu, 5, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_SET_5_C: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.C = z80_op_set(cpu, 5, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_SET_5_D: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.D = z80_op_set(cpu, 5, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_SET_5_E: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.E = z80_op_set(cpu, 5, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_SET_5_H: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.H = z80_op_set(cpu, 5, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_SET_5_L: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.L = z80_op_set(cpu, 5, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_SET_5_HL: { z80_write8(cpu, cpu->registers.IX + d, z80_op_set(cpu, 5, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_SET_5_A: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.A = z80_op_set(cpu, 5, z80_read8(cpu, cpu->registers.IX + d)));} break;

    // BIT SET - Bit 6
    case DD_CB_SET_6_B: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.B = z80_op_set(cpu, 6, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_SET_6_C: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.C = z80_op_set(cpu, 6, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_SET_6_D: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.D = z80_op_set(cpu, 6, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_SET_6_E: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.E = z80_op_set(cpu, 6, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_SET_6_H: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.H = z80_op_set(cpu, 6, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_SET_6_L: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.L = z80_op_set(cpu, 6, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_SET_6_HL: { z80_write8(cpu, cpu->registers.IX + d, z80_op_set(cpu, 6, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_SET_6_A: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.A = z80_op_set(cpu, 6, z80_read8(cpu, cpu->registers.IX + d)));} break;

    // BIT SET - Bit 7
    case DD_CB_SET_7_B: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.B = z80_op_set(cpu, 7, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_SET_7_C: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.C = z80_op_set(cpu, 7, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_SET_7_D: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.D = z80_op_set(cpu, 7, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_SET_7_E: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.E = z80_op_set(cpu, 7, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_SET_7_H: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.H = z80_op_set(cpu, 7, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_SET_7_L: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.L = z80_op_set(cpu, 7, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_SET_7_HL: { z80_write8(cpu, cpu->registers.IX + d, z80_op_set(cpu, 7, z80_read8(cpu, cpu->registers.IX + d)));} break;
    case DD_CB_SET_7_A: { z80_write8(cpu, cpu->registers.IX + d, cpu->registers.A = z80_op_set(cpu, 7, z80_read8(cpu, cpu->registers.IX + d)));} break;

    default:
        printf("Unimplemented CB Instruction: 0x%02X\n", cb_opcode);
        cpu->running = false;
        cpu->halted = true;
        break;
    }
}