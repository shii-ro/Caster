#ifndef Z80_OPCODE_TABLE_H_
#define Z80_OPCODE_TABLE_H_

#include <stdint.h>

// Opcode information structure
typedef struct
{
    const char *name;
    uint8_t length;
    uint8_t cycles;
} opcode_info_t;

// Z80 opcode lookup table
static const opcode_info_t opcode_table[256] =
    {
        // 0x00-0x0F
        {"NOP", 1, 4},         // 0x00
        {"LD BC,%04X", 3, 10}, // 0x01
        {"LD (BC),A", 1, 7},   // 0x02
        {"INC BC", 1, 6},      // 0x03
        {"INC B", 1, 4},       // 0x04
        {"DEC B", 1, 4},       // 0x05
        {"LD B,%02X", 2, 7},   // 0x06
        {"RLCA", 1, 4},        // 0x07
        {"EX AF,AF'", 1, 4},   // 0x08
        {"ADD HL,BC", 1, 11},  // 0x09
        {"LD A,(BC)", 1, 7},   // 0x0A
        {"DEC BC", 1, 6},      // 0x0B
        {"INC C", 1, 4},       // 0x0C
        {"DEC C", 1, 4},       // 0x0D
        {"LD C,%02X", 2, 7},   // 0x0E
        {"RRCA", 1, 4},        // 0x0F

        // 0x10-0x1F
        {"DJNZ %02X", 2, 8},   // 0x10 (8/13 cycles - conditional)
        {"LD DE,%04X", 3, 10}, // 0x11
        {"LD (DE),A", 1, 7},   // 0x12
        {"INC DE", 1, 6},      // 0x13
        {"INC D", 1, 4},       // 0x14
        {"DEC D", 1, 4},       // 0x15
        {"LD D,%02X", 2, 7},   // 0x16
        {"RLA", 1, 4},         // 0x17
        {"JR %02X", 2, 12},    // 0x18
        {"ADD HL,DE", 1, 11},  // 0x19
        {"LD A,(DE)", 1, 7},   // 0x1A
        {"DEC DE", 1, 6},      // 0x1B
        {"INC E", 1, 4},       // 0x1C
        {"DEC E", 1, 4},       // 0x1D
        {"LD E,%02X", 2, 7},   // 0x1E
        {"RRA", 1, 4},         // 0x1F

        // 0x20-0x2F
        {"JR NZ,%02X", 2, 7},    // 0x20 (7/12 cycles - conditional)
        {"LD HL,%04X", 3, 10},   // 0x21
        {"LD (%04X),HL", 3, 16}, // 0x22
        {"INC HL", 1, 6},        // 0x23
        {"INC H", 1, 4},         // 0x24
        {"DEC H", 1, 4},         // 0x25
        {"LD H,%02X", 2, 7},     // 0x26
        {"DAA", 1, 4},           // 0x27
        {"JR Z,%02X", 2, 7},     // 0x28 (7/12 cycles - conditional)
        {"ADD HL,HL", 1, 11},    // 0x29
        {"LD HL,(%04X)", 3, 16}, // 0x2A
        {"DEC HL", 1, 6},        // 0x2B
        {"INC L", 1, 4},         // 0x2C
        {"DEC L", 1, 4},         // 0x2D
        {"LD L,%02X", 2, 7},     // 0x2E
        {"CPL", 1, 4},           // 0x2F

        // 0x30-0x3F
        {"JR NC,%02X", 2, 7},    // 0x30 (7/12 cycles - conditional)
        {"LD SP,%04X", 3, 10},   // 0x31
        {"LD (%04X),A", 3, 13},  // 0x32
        {"INC SP", 1, 6},        // 0x33
        {"INC (HL)", 1, 11},     // 0x34
        {"DEC (HL)", 1, 11},     // 0x35
        {"LD (HL),%02X", 2, 10}, // 0x36
        {"SCF", 1, 4},           // 0x37
        {"JR C,%02X", 2, 7},     // 0x38 (7/12 cycles - conditional)
        {"ADD HL,SP", 1, 11},    // 0x39
        {"LD A,(%04X)", 3, 13},  // 0x3A
        {"DEC SP", 1, 6},        // 0x3B
        {"INC A", 1, 4},         // 0x3C
        {"DEC A", 1, 4},         // 0x3D
        {"LD A,%02X", 2, 7},     // 0x3E
        {"CCF", 1, 4},           // 0x3F

        // 0x40-0x4F - LD r,r' instructions
        {"LD B,B", 1, 4},    // 0x40
        {"LD B,C", 1, 4},    // 0x41
        {"LD B,D", 1, 4},    // 0x42
        {"LD B,E", 1, 4},    // 0x43
        {"LD B,H", 1, 4},    // 0x44
        {"LD B,L", 1, 4},    // 0x45
        {"LD B,(HL)", 1, 7}, // 0x46
        {"LD B,A", 1, 4},    // 0x47
        {"LD C,B", 1, 4},    // 0x48
        {"LD C,C", 1, 4},    // 0x49
        {"LD C,D", 1, 4},    // 0x4A
        {"LD C,E", 1, 4},    // 0x4B
        {"LD C,H", 1, 4},    // 0x4C
        {"LD C,L", 1, 4},    // 0x4D
        {"LD C,(HL)", 1, 7}, // 0x4E
        {"LD C,A", 1, 4},    // 0x4F

        // 0x50-0x5F - LD r,r' instructions (continued)
        {"LD D,B", 1, 4},    // 0x50
        {"LD D,C", 1, 4},    // 0x51
        {"LD D,D", 1, 4},    // 0x52
        {"LD D,E", 1, 4},    // 0x53
        {"LD D,H", 1, 4},    // 0x54
        {"LD D,L", 1, 4},    // 0x55
        {"LD D,(HL)", 1, 7}, // 0x56
        {"LD D,A", 1, 4},    // 0x57
        {"LD E,B", 1, 4},    // 0x58
        {"LD E,C", 1, 4},    // 0x59
        {"LD E,D", 1, 4},    // 0x5A
        {"LD E,E", 1, 4},    // 0x5B
        {"LD E,H", 1, 4},    // 0x5C
        {"LD E,L", 1, 4},    // 0x5D
        {"LD E,(HL)", 1, 7}, // 0x5E
        {"LD E,A", 1, 4},    // 0x5F

        // 0x60-0x6F - LD r,r' instructions (continued)
        {"LD H,B", 1, 4},    // 0x60
        {"LD H,C", 1, 4},    // 0x61
        {"LD H,D", 1, 4},    // 0x62
        {"LD H,E", 1, 4},    // 0x63
        {"LD H,H", 1, 4},    // 0x64
        {"LD H,L", 1, 4},    // 0x65
        {"LD H,(HL)", 1, 7}, // 0x66
        {"LD H,A", 1, 4},    // 0x67
        {"LD L,B", 1, 4},    // 0x68
        {"LD L,C", 1, 4},    // 0x69
        {"LD L,D", 1, 4},    // 0x6A
        {"LD L,E", 1, 4},    // 0x6B
        {"LD L,H", 1, 4},    // 0x6C
        {"LD L,L", 1, 4},    // 0x6D
        {"LD L,(HL)", 1, 7}, // 0x6E
        {"LD L,A", 1, 4},    // 0x6F

        // 0x70-0x7F - LD (HL),r and LD A,r instructions
        {"LD (HL),B", 1, 7}, // 0x70
        {"LD (HL),C", 1, 7}, // 0x71
        {"LD (HL),D", 1, 7}, // 0x72
        {"LD (HL),E", 1, 7}, // 0x73
        {"LD (HL),H", 1, 7}, // 0x74
        {"LD (HL),L", 1, 7}, // 0x75
        {"HALT", 1, 4},      // 0x76
        {"LD (HL),A", 1, 7}, // 0x77
        {"LD A,B", 1, 4},    // 0x78
        {"LD A,C", 1, 4},    // 0x79
        {"LD A,D", 1, 4},    // 0x7A
        {"LD A,E", 1, 4},    // 0x7B
        {"LD A,H", 1, 4},    // 0x7C
        {"LD A,L", 1, 4},    // 0x7D
        {"LD A,(HL)", 1, 7}, // 0x7E
        {"LD A,A", 1, 4},    // 0x7F

        // 0x80-0x8F - ADD A,r instructions
        {"ADD A,B", 1, 4},    // 0x80
        {"ADD A,C", 1, 4},    // 0x81
        {"ADD A,D", 1, 4},    // 0x82
        {"ADD A,E", 1, 4},    // 0x83
        {"ADD A,H", 1, 4},    // 0x84
        {"ADD A,L", 1, 4},    // 0x85
        {"ADD A,(HL)", 1, 7}, // 0x86
        {"ADD A,A", 1, 4},    // 0x87
        {"ADC A,B", 1, 4},    // 0x88
        {"ADC A,C", 1, 4},    // 0x89
        {"ADC A,D", 1, 4},    // 0x8A
        {"ADC A,E", 1, 4},    // 0x8B
        {"ADC A,H", 1, 4},    // 0x8C
        {"ADC A,L", 1, 4},    // 0x8D
        {"ADC A,(HL)", 1, 7}, // 0x8E
        {"ADC A,A", 1, 4},    // 0x8F

        // 0x90-0x9F - SUB r and SBC A,r instructions
        {"SUB B", 1, 4},      // 0x90
        {"SUB C", 1, 4},      // 0x91
        {"SUB D", 1, 4},      // 0x92
        {"SUB E", 1, 4},      // 0x93
        {"SUB H", 1, 4},      // 0x94
        {"SUB L", 1, 4},      // 0x95
        {"SUB (HL)", 1, 7},   // 0x96
        {"SUB A", 1, 4},      // 0x97
        {"SBC A,B", 1, 4},    // 0x98
        {"SBC A,C", 1, 4},    // 0x99
        {"SBC A,D", 1, 4},    // 0x9A
        {"SBC A,E", 1, 4},    // 0x9B
        {"SBC A,H", 1, 4},    // 0x9C
        {"SBC A,L", 1, 4},    // 0x9D
        {"SBC A,(HL)", 1, 7}, // 0x9E
        {"SBC A,A", 1, 4},    // 0x9F

        // 0xA0-0xAF - AND r, XOR r, OR r, CP r instructions
        {"AND B", 1, 4},    // 0xA0
        {"AND C", 1, 4},    // 0xA1
        {"AND D", 1, 4},    // 0xA2
        {"AND E", 1, 4},    // 0xA3
        {"AND H", 1, 4},    // 0xA4
        {"AND L", 1, 4},    // 0xA5
        {"AND (HL)", 1, 7}, // 0xA6
        {"AND A", 1, 4},    // 0xA7
        {"XOR B", 1, 4},    // 0xA8
        {"XOR C", 1, 4},    // 0xA9
        {"XOR D", 1, 4},    // 0xAA
        {"XOR E", 1, 4},    // 0xAB
        {"XOR H", 1, 4},    // 0xAC
        {"XOR L", 1, 4},    // 0xAD
        {"XOR (HL)", 1, 7}, // 0xAE
        {"XOR A", 1, 4},    // 0xAF

        // 0xB0-0xBF - OR r and CP r instructions
        {"OR B", 1, 4},    // 0xB0
        {"OR C", 1, 4},    // 0xB1
        {"OR D", 1, 4},    // 0xB2
        {"OR E", 1, 4},    // 0xB3
        {"OR H", 1, 4},    // 0xB4
        {"OR L", 1, 4},    // 0xB5
        {"OR (HL)", 1, 7}, // 0xB6
        {"OR A", 1, 4},    // 0xB7
        {"CP B", 1, 4},    // 0xB8
        {"CP C", 1, 4},    // 0xB9
        {"CP D", 1, 4},    // 0xBA
        {"CP E", 1, 4},    // 0xBB
        {"CP H", 1, 4},    // 0xBC
        {"CP L", 1, 4},    // 0xBD
        {"CP (HL)", 1, 7}, // 0xBE
        {"CP A", 1, 4},    // 0xBF

        // 0xC0-0xCF - Control flow and stack operations
        {"RET NZ", 1, 5},        // 0xC0 (5/11 cycles - conditional)
        {"POP BC", 1, 10},       // 0xC1
        {"JP NZ,%04X", 3, 10},   // 0xC2
        {"JP %04X", 3, 10},      // 0xC3
        {"CALL NZ,%04X", 3, 10}, // 0xC4 (10/17 cycles - conditional)
        {"PUSH BC", 1, 11},      // 0xC5
        {"ADD A,%02X", 2, 7},    // 0xC6
        {"RST 00H", 1, 11},      // 0xC7
        {"RET Z", 1, 5},         // 0xC8 (5/11 cycles - conditional)
        {"RET", 1, 10},          // 0xC9
        {"JP Z,%04X", 3, 10},    // 0xCA
        {"PREFIX CB", 2, 0},     // 0xCB (handled separately)
        {"CALL Z,%04X", 3, 10},  // 0xCC (10/17 cycles - conditional)
        {"CALL %04X", 3, 17},    // 0xCD
        {"ADC A,%02X", 2, 7},    // 0xCE
        {"RST 08H", 1, 11},      // 0xCF

        // 0xD0-0xDF - More control flow and operations
        {"RET NC", 1, 5},        // 0xD0 (5/11 cycles - conditional)
        {"POP DE", 1, 10},       // 0xD1
        {"JP NC,%04X", 3, 10},   // 0xD2
        {"OUT (%02X),A", 2, 11}, // 0xD3
        {"CALL NC,%04X", 3, 10}, // 0xD4 (10/17 cycles - conditional)
        {"PUSH DE", 1, 11},      // 0xD5
        {"SUB %02X", 2, 7},      // 0xD6
        {"RST 10H", 1, 11},      // 0xD7
        {"RET C", 1, 5},         // 0xD8 (5/11 cycles - conditional)
        {"EXX", 1, 4},           // 0xD9
        {"JP C,%04X", 3, 10},    // 0xDA
        {"IN A,(%02X)", 2, 11},  // 0xDB
        {"CALL C,%04X", 3, 10},  // 0xDC (10/17 cycles - conditional)
        {"PREFIX DD", 2, 0},     // 0xDD (IX prefix - handled separately)
        {"SBC A,%02X", 2, 7},    // 0xDE
        {"RST 18H", 1, 11},      // 0xDF

        // 0xE0-0xEF - More operations
        {"RET PO", 1, 5},        // 0xE0 (5/11 cycles - conditional)
        {"POP HL", 1, 10},       // 0xE1
        {"JP PO,%04X", 3, 10},   // 0xE2
        {"EX (SP),HL", 1, 19},   // 0xE3
        {"CALL PO,%04X", 3, 10}, // 0xE4 (10/17 cycles - conditional)
        {"PUSH HL", 1, 11},      // 0xE5
        {"AND %02X", 2, 7},      // 0xE6
        {"RST 20H", 1, 11},      // 0xE7
        {"RET PE", 1, 5},        // 0xE8 (5/11 cycles - conditional)
        {"JP (HL)", 1, 4},       // 0xE9
        {"JP PE,%04X", 3, 10},   // 0xEA
        {"EX DE,HL", 1, 4},      // 0xEB
        {"CALL PE,%04X", 3, 10}, // 0xEC (10/17 cycles - conditional)
        {"PREFIX ED", 2, 0},     // 0xED (extended instructions - handled separately)
        {"XOR %02X", 2, 7},      // 0xEE
        {"RST 28H", 1, 11},      // 0xEF

        // 0xF0-0xFF - Final operations
        {"RET P", 1, 5},        // 0xF0 (5/11 cycles - conditional)
        {"POP AF", 1, 10},      // 0xF1
        {"JP P,%04X", 3, 10},   // 0xF2
        {"DI", 1, 4},           // 0xF3
        {"CALL P,%04X", 3, 10}, // 0xF4 (10/17 cycles - conditional)
        {"PUSH AF", 1, 11},     // 0xF5
        {"OR %02X", 2, 7},      // 0xF6
        {"RST 30H", 1, 11},     // 0xF7
        {"RET M", 1, 5},        // 0xF8 (5/11 cycles - conditional)
        {"LD SP,HL", 1, 6},     // 0xF9
        {"JP M,%04X", 3, 10},   // 0xFA
        {"EI", 1, 4},           // 0xFB
        {"CALL M,%04X", 3, 10}, // 0xFC (10/17 cycles - conditional)
        {"PREFIX FD", 2, 0},    // 0xFD (IY prefix - handled separately)
        {"CP %02X", 2, 7},      // 0xFE
        {"RST 38H", 1, 11},     // 0xFF
};

// DD prefixed IX instructions (second byte opcodes 0x00-0xFF)
static const opcode_info_t dd_opcode_table[256] =
    {
        // 0x00-0x0F
        {"???", 2, 8},        // 0x00
        {"???", 2, 8},        // 0x01
        {"???", 2, 8},        // 0x02
        {"???", 2, 8},        // 0x03
        {"???", 2, 8},        // 0x04
        {"???", 2, 8},        // 0x05
        {"???", 2, 8},        // 0x06
        {"???", 2, 8},        // 0x07
        {"???", 2, 8},        // 0x08
        {"ADD IX,BC", 2, 15}, // 0x09
        {"???", 2, 8},        // 0x0A
        {"???", 2, 8},        // 0x0B
        {"???", 2, 8},        // 0x0C
        {"???", 2, 8},        // 0x0D
        {"???", 2, 8},        // 0x0E
        {"???", 2, 8},        // 0x0F

        // 0x10-0x1F
        {"???", 2, 8},        // 0x10
        {"???", 2, 8},        // 0x11
        {"???", 2, 8},        // 0x12
        {"???", 2, 8},        // 0x13
        {"???", 2, 8},        // 0x14
        {"???", 2, 8},        // 0x15
        {"???", 2, 8},        // 0x16
        {"???", 2, 8},        // 0x17
        {"???", 2, 8},        // 0x18
        {"ADD IX,DE", 2, 15}, // 0x19
        {"???", 2, 8},        // 0x1A
        {"???", 2, 8},        // 0x1B
        {"???", 2, 8},        // 0x1C
        {"???", 2, 8},        // 0x1D
        {"???", 2, 8},        // 0x1E
        {"???", 2, 8},        // 0x1F

        // 0x20-0x2F
        {"???", 2, 8},           // 0x20
        {"LD IX,%04X", 4, 14},   // 0x21
        {"LD (%04X),IX", 4, 20}, // 0x22
        {"INC IX", 2, 10},       // 0x23
        {"INC IXh", 2, 8},       // 0x24
        {"DEC IXh", 2, 8},       // 0x25
        {"LD IXh,%02X", 3, 11},  // 0x26
        {"???", 2, 8},           // 0x27
        {"???", 2, 8},           // 0x28
        {"ADD IX,IX", 2, 15},    // 0x29
        {"LD IX,(%04X)", 4, 20}, // 0x2A
        {"DEC IX", 2, 10},       // 0x2B
        {"INC IXl", 2, 8},       // 0x2C
        {"DEC IXl", 2, 8},       // 0x2D
        {"LD IXl,%02X", 3, 11},  // 0x2E
        {"???", 2, 8},           // 0x2F

        // 0x30-0x3F
        {"???", 2, 8},              // 0x30
        {"???", 2, 8},              // 0x31
        {"???", 2, 8},              // 0x32
        {"???", 2, 8},              // 0x33
        {"INC (IX+%d)", 3, 23},     // 0x34
        {"DEC (IX+%d)", 3, 23},     // 0x35
        {"LD (IX+%d),%02X", 4, 19}, // 0x36
        {"???", 2, 8},              // 0x37
        {"???", 2, 8},              // 0x38
        {"ADD IX,SP", 2, 15},       // 0x39
        {"???", 2, 8},              // 0x3A
        {"???", 2, 8},              // 0x3B
        {"???", 2, 8},              // 0x3C
        {"???", 2, 8},              // 0x3D
        {"???", 2, 8},              // 0x3E
        {"???", 2, 8},              // 0x3F

        // 0x40-0x4F
        {"???", 2, 8},           // 0x40
        {"???", 2, 8},           // 0x41
        {"???", 2, 8},           // 0x42
        {"???", 2, 8},           // 0x43
        {"LD B,IXh", 2, 8},      // 0x44
        {"LD B,IXl", 2, 8},      // 0x45
        {"LD B,(IX+%d)", 3, 19}, // 0x46
        {"???", 2, 8},           // 0x47
        {"???", 2, 8},           // 0x48
        {"???", 2, 8},           // 0x49
        {"???", 2, 8},           // 0x4A
        {"???", 2, 8},           // 0x4B
        {"LD C,IXh", 2, 8},      // 0x4C
        {"LD C,IXl", 2, 8},      // 0x4D
        {"LD C,(IX+%d)", 3, 19}, // 0x4E
        {"???", 2, 8},           // 0x4F

        // 0x50-0x5F
        {"???", 2, 8},           // 0x50
        {"???", 2, 8},           // 0x51
        {"???", 2, 8},           // 0x52
        {"???", 2, 8},           // 0x53
        {"LD D,IXh", 2, 8},      // 0x54
        {"LD D,IXl", 2, 8},      // 0x55
        {"LD D,(IX+%d)", 3, 19}, // 0x56
        {"???", 2, 8},           // 0x57
        {"???", 2, 8},           // 0x58
        {"???", 2, 8},           // 0x59
        {"???", 2, 8},           // 0x5A
        {"???", 2, 8},           // 0x5B
        {"LD E,IXh", 2, 8},      // 0x5C
        {"LD E,IXl", 2, 8},      // 0x5D
        {"LD E,(IX+%d)", 3, 19}, // 0x5E
        {"???", 2, 8},           // 0x5F

        // 0x60-0x6F
        {"LD IXh,B", 2, 8},      // 0x60
        {"LD IXh,C", 2, 8},      // 0x61
        {"LD IXh,D", 2, 8},      // 0x62
        {"LD IXh,E", 2, 8},      // 0x63
        {"LD IXh,IXh", 2, 8},    // 0x64
        {"LD IXh,IXl", 2, 8},    // 0x65
        {"LD H,(IX+%d)", 3, 19}, // 0x66
        {"LD IXh,A", 2, 8},      // 0x67
        {"LD IXl,B", 2, 8},      // 0x68
        {"LD IXl,C", 2, 8},      // 0x69
        {"LD IXl,D", 2, 8},      // 0x6A
        {"LD IXl,E", 2, 8},      // 0x6B
        {"LD IXl,IXh", 2, 8},    // 0x6C
        {"LD IXl,IXl", 2, 8},    // 0x6D
        {"LD L,(IX+%d)", 3, 19}, // 0x6E
        {"LD IXl,A", 2, 8},      // 0x6F

        // 0x70-0x7F
        {"LD (IX+%d),B", 3, 19}, // 0x70
        {"LD (IX+%d),C", 3, 19}, // 0x71
        {"LD (IX+%d),D", 3, 19}, // 0x72
        {"LD (IX+%d),E", 3, 19}, // 0x73
        {"LD (IX+%d),H", 3, 19}, // 0x74
        {"LD (IX+%d),L", 3, 19}, // 0x75
        {"???", 2, 8},           // 0x76
        {"LD (IX+%d),A", 3, 19}, // 0x77
        {"???", 2, 8},           // 0x78
        {"???", 2, 8},           // 0x79
        {"???", 2, 8},           // 0x7A
        {"???", 2, 8},           // 0x7B
        {"LD A,IXh", 2, 8},      // 0x7C
        {"LD A,IXl", 2, 8},      // 0x7D
        {"LD A,(IX+%d)", 3, 19}, // 0x7E
        {"???", 2, 8},           // 0x7F

        // 0x80-0x8F
        {"???", 2, 8},            // 0x80
        {"???", 2, 8},            // 0x81
        {"???", 2, 8},            // 0x82
        {"???", 2, 8},            // 0x83
        {"ADD A,IXh", 2, 8},      // 0x84
        {"ADD A,IXl", 2, 8},      // 0x85
        {"ADD A,(IX+%d)", 3, 19}, // 0x86
        {"???", 2, 8},            // 0x87
        {"???", 2, 8},            // 0x88
        {"???", 2, 8},            // 0x89
        {"???", 2, 8},            // 0x8A
        {"???", 2, 8},            // 0x8B
        {"ADC A,IXh", 2, 8},      // 0x8C
        {"ADC A,IXl", 2, 8},      // 0x8D
        {"ADC A,(IX+%d)", 3, 19}, // 0x8E
        {"???", 2, 8},            // 0x8F

        // 0x90-0x9F
        {"???", 2, 8},            // 0x90
        {"???", 2, 8},            // 0x91
        {"???", 2, 8},            // 0x92
        {"???", 2, 8},            // 0x93
        {"SUB IXh", 2, 8},        // 0x94
        {"SUB IXl", 2, 8},        // 0x95
        {"SUB (IX+%d)", 3, 19},   // 0x96
        {"???", 2, 8},            // 0x97
        {"???", 2, 8},            // 0x98
        {"???", 2, 8},            // 0x99
        {"???", 2, 8},            // 0x9A
        {"???", 2, 8},            // 0x9B
        {"SBC A,IXh", 2, 8},      // 0x9C
        {"SBC A,IXl", 2, 8},      // 0x9D
        {"SBC A,(IX+%d)", 3, 19}, // 0x9E
        {"???", 2, 8},            // 0x9F

        // 0xA0-0xAF
        {"???", 2, 8},          // 0xA0
        {"???", 2, 8},          // 0xA1
        {"???", 2, 8},          // 0xA2
        {"???", 2, 8},          // 0xA3
        {"AND IXh", 2, 8},      // 0xA4
        {"AND IXl", 2, 8},      // 0xA5
        {"AND (IX+%d)", 3, 19}, // 0xA6
        {"???", 2, 8},          // 0xA7
        {"???", 2, 8},          // 0xA8
        {"???", 2, 8},          // 0xA9
        {"???", 2, 8},          // 0xAA
        {"???", 2, 8},          // 0xAB
        {"XOR IXh", 2, 8},      // 0xAC
        {"XOR IXl", 2, 8},      // 0xAD
        {"XOR (IX+%d)", 3, 19}, // 0xAE
        {"???", 2, 8},          // 0xAF

        // 0xB0-0xBF
        {"???", 2, 8},         // 0xB0
        {"???", 2, 8},         // 0xB1
        {"???", 2, 8},         // 0xB2
        {"???", 2, 8},         // 0xB3
        {"OR IXh", 2, 8},      // 0xB4
        {"OR IXl", 2, 8},      // 0xB5
        {"OR (IX+%d)", 3, 19}, // 0xB6
        {"???", 2, 8},         // 0xB7
        {"???", 2, 8},         // 0xB8
        {"???", 2, 8},         // 0xB9
        {"???", 2, 8},         // 0xBA
        {"???", 2, 8},         // 0xBB
        {"CP IXh", 2, 8},      // 0xBC
        {"CP IXl", 2, 8},      // 0xBD
        {"CP (IX+%d)", 3, 19}, // 0xBE
        {"???", 2, 8},         // 0xBF

        // 0xC0-0xCF
        {"???", 2, 8}, // 0xC0
        {"???", 2, 8}, // 0xC1
        {"???", 2, 8}, // 0xC2
        {"???", 2, 8}, // 0xC3
        {"???", 2, 8}, // 0xC4
        {"???", 2, 8}, // 0xC5
        {"???", 2, 8}, // 0xC6
        {"???", 2, 8}, // 0xC7
        {"???", 2, 8}, // 0xC8
        {"???", 2, 8}, // 0xC9
        {"???", 2, 8}, // 0xCA
        {"???", 2, 8}, // 0xCB (DD CB handled separately)
        {"???", 2, 8}, // 0xCC
        {"???", 2, 8}, // 0xCD
        {"???", 2, 8}, // 0xCE
        {"???", 2, 8}, // 0xCF

        // 0xD0-0xDF
        {"???", 2, 8}, // 0xD0
        {"???", 2, 8}, // 0xD1
        {"???", 2, 8}, // 0xD2
        {"???", 2, 8}, // 0xD3
        {"???", 2, 8}, // 0xD4
        {"???", 2, 8}, // 0xD5
        {"???", 2, 8}, // 0xD6
        {"???", 2, 8}, // 0xD7
        {"???", 2, 8}, // 0xD8
        {"???", 2, 8}, // 0xD9
        {"???", 2, 8}, // 0xDA
        {"???", 2, 8}, // 0xDB
        {"???", 2, 8}, // 0xDC
        {"???", 2, 8}, // 0xDD (DD prefix - undefined)
        {"???", 2, 8}, // 0xDE
        {"???", 2, 8}, // 0xDF

        // 0xE0-0xEF
        {"???", 2, 8},         // 0xE0
        {"POP IX", 2, 14},     // 0xE1
        {"???", 2, 8},         // 0xE2
        {"EX (SP),IX", 2, 23}, // 0xE3
        {"???", 2, 8},         // 0xE4
        {"PUSH IX", 2, 15},    // 0xE5
        {"???", 2, 8},         // 0xE6
        {"???", 2, 8},         // 0xE7
        {"???", 2, 8},         // 0xE8
        {"JP (IX)", 2, 8},     // 0xE9
        {"???", 2, 8},         // 0xEA
        {"???", 2, 8},         // 0xEB
        {"???", 2, 8},         // 0xEC
        {"???", 2, 8},         // 0xED (ED prefix - undefined in DD)
        {"???", 2, 8},         // 0xEE
        {"???", 2, 8},         // 0xEF

        // 0xF0-0xFF
        {"???", 2, 8},       // 0xF0
        {"???", 2, 8},       // 0xF1
        {"???", 2, 8},       // 0xF2
        {"???", 2, 8},       // 0xF3
        {"???", 2, 8},       // 0xF4
        {"???", 2, 8},       // 0xF5
        {"???", 2, 8},       // 0xF6
        {"???", 2, 8},       // 0xF7
        {"???", 2, 8},       // 0xF8
        {"LD SP,IX", 2, 10}, // 0xF9
        {"???", 2, 8},       // 0xFA
        {"???", 2, 8},       // 0xFB
        {"???", 2, 8},       // 0xFC
        {"???", 2, 8},       // 0xFD (FD prefix - undefined)
        {"???", 2, 8},       // 0xFE
        {"???", 2, 8}        // 0xFF
};

static const opcode_info_t ed_opcode_table[256] = // 0xED prefix opcodes
    {
        // 0x00-0x3F: Mostly invalid/undocumented
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8}, // 0x00-0x03
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8}, // 0x04-0x07
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8}, // 0x08-0x0B
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8}, // 0x0C-0x0F
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8}, // 0x10-0x13
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8}, // 0x14-0x17
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8}, // 0x18-0x1B
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8}, // 0x1C-0x1F
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8}, // 0x20-0x23
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8}, // 0x24-0x27
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8}, // 0x28-0x2B
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8}, // 0x2C-0x2F
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8}, // 0x30-0x33
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8}, // 0x34-0x37
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8}, // 0x38-0x3B
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8}, // 0x3C-0x3F

        // 0x40-0x7F: I/O and register operations
        {"IN B,(C)", 2, 12},     // 0x40
        {"OUT (C),B", 2, 12},    // 0x41
        {"SBC HL,BC", 2, 15},    // 0x42
        {"LD (%04X),BC", 4, 20}, // 0x43
        {"NEG", 2, 8},           // 0x44
        {"RETN", 2, 14},         // 0x45
        {"IM 0", 2, 8},          // 0x46
        {"LD I,A", 2, 9},        // 0x47
        {"IN C,(C)", 2, 12},     // 0x48
        {"OUT (C),C", 2, 12},    // 0x49
        {"ADC HL,BC", 2, 15},    // 0x4A
        {"LD BC,(%04X)", 4, 20}, // 0x4B
        {"NEG", 2, 8},           // 0x4C (duplicate)
        {"RETI", 2, 14},         // 0x4D
        {"IM 0/1", 2, 8},        // 0x4E (undocumented, acts like IM 0)
        {"LD R,A", 2, 9},        // 0x4F

        {"IN D,(C)", 2, 12},     // 0x50
        {"OUT (C),D", 2, 12},    // 0x51
        {"SBC HL,DE", 2, 15},    // 0x52
        {"LD (%04X),DE", 4, 20}, // 0x53
        {"NEG", 2, 8},           // 0x54 (duplicate)
        {"RETN", 2, 14},         // 0x55 (duplicate)
        {"IM 1", 2, 8},          // 0x56
        {"LD A,I", 2, 9},        // 0x57
        {"IN E,(C)", 2, 12},     // 0x58
        {"OUT (C),E", 2, 12},    // 0x59
        {"ADC HL,DE", 2, 15},    // 0x5A
        {"LD DE,(%04X)", 4, 20}, // 0x5B
        {"NEG", 2, 8},           // 0x5C (duplicate)
        {"RETN", 2, 14},         // 0x5D (duplicate)
        {"IM 2", 2, 8},          // 0x5E
        {"LD A,R", 2, 9},        // 0x5F

        {"IN H,(C)", 2, 12},     // 0x60
        {"OUT (C),H", 2, 12},    // 0x61
        {"SBC HL,HL", 2, 15},    // 0x62
        {"LD (%04X),HL", 4, 20}, // 0x63 (duplicate of standard LD)
        {"NEG", 2, 8},           // 0x64 (duplicate)
        {"RETN", 2, 14},         // 0x65 (duplicate)
        {"IM 0", 2, 8},          // 0x66 (duplicate)
        {"RRD", 2, 18},          // 0x67
        {"IN L,(C)", 2, 12},     // 0x68
        {"OUT (C),L", 2, 12},    // 0x69
        {"ADC HL,HL", 2, 15},    // 0x6A
        {"LD HL,(%04X)", 4, 20}, // 0x6B (duplicate of standard LD)
        {"NEG", 2, 8},           // 0x6C (duplicate)
        {"RETN", 2, 14},         // 0x6D (duplicate)
        {"IM 0/1", 2, 8},        // 0x6E (undocumented, acts like IM 0)
        {"RLD", 2, 18},          // 0x6F

        {"IN (C)", 2, 12},       // 0x70 (flags set, no register stored)
        {"OUT (C),0", 2, 12},    // 0x71 (outputs 0)
        {"SBC HL,SP", 2, 15},    // 0x72
        {"LD (%04X),SP", 4, 20}, // 0x73
        {"NEG", 2, 8},           // 0x74 (duplicate)
        {"RETN", 2, 14},         // 0x75 (duplicate)
        {"IM 1", 2, 8},          // 0x76 (duplicate)
        {"INVALID", 2, 8},       // 0x77
        {"IN A,(C)", 2, 12},     // 0x78
        {"OUT (C),A", 2, 12},    // 0x79
        {"ADC HL,SP", 2, 15},    // 0x7A
        {"LD SP,(%04X)", 4, 20}, // 0x7B
        {"NEG", 2, 8},           // 0x7C (duplicate)
        {"RETN", 2, 14},         // 0x7D (duplicate)
        {"IM 2", 2, 8},          // 0x7E (duplicate)
        {"INVALID", 2, 8},       // 0x7F

        // 0x80-0x9F: Invalid/undocumented
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8}, // 0x80-0x83
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8}, // 0x84-0x87
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8}, // 0x88-0x8B
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8}, // 0x8C-0x8F
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8}, // 0x90-0x93
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8}, // 0x94-0x97
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8}, // 0x98-0x9B
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8}, // 0x9C-0x9F

        // 0xA0-0xBF: Block operations (variable cycles!)
        {"LDI", 2, 16},    // 0xA0 - always 16 cycles
        {"CPI", 2, 16},    // 0xA1 - always 16 cycles
        {"INI", 2, 16},    // 0xA2 - always 16 cycles
        {"OUTI", 2, 16},   // 0xA3 - always 16 cycles
        {"INVALID", 2, 8}, // 0xA4
        {"INVALID", 2, 8}, // 0xA5
        {"INVALID", 2, 8}, // 0xA6
        {"INVALID", 2, 8}, // 0xA7
        {"LDD", 2, 16},    // 0xA8 - always 16 cycles
        {"CPD", 2, 16},    // 0xA9 - always 16 cycles
        {"IND", 2, 16},    // 0xAA - always 16 cycles
        {"OUTD", 2, 16},   // 0xAB - always 16 cycles
        {"INVALID", 2, 8}, // 0xAC
        {"INVALID", 2, 8}, // 0xAD
        {"INVALID", 2, 8}, // 0xAE
        {"INVALID", 2, 8}, // 0xAF

        {"LDIR", 2, 21},   // 0xB0 - 21 if repeating, 16 if done
        {"CPIR", 2, 21},   // 0xB1 - 21 if repeating, 16 if done
        {"INIR", 2, 21},   // 0xB2 - 21 if repeating, 16 if done
        {"OTIR", 2, 21},   // 0xB3 - 21 if repeating, 16 if done
        {"INVALID", 2, 8}, // 0xB4
        {"INVALID", 2, 8}, // 0xB5
        {"INVALID", 2, 8}, // 0xB6
        {"INVALID", 2, 8}, // 0xB7
        {"LDDR", 2, 21},   // 0xB8 - 21 if repeating, 16 if done
        {"CPDR", 2, 21},   // 0xB9 - 21 if repeating, 16 if done
        {"INDR", 2, 21},   // 0xBA - 21 if repeating, 16 if done
        {"OTDR", 2, 21},   // 0xBB - 21 if repeating, 16 if done
        {"INVALID", 2, 8}, // 0xBC
        {"INVALID", 2, 8}, // 0xBD
        {"INVALID", 2, 8}, // 0xBE
        {"INVALID", 2, 8}, // 0xBF

        // 0xC0-0xFF: Invalid/undocumented
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8}, // 0xC0-0xC3
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8}, // 0xC4-0xC7
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8}, // 0xC8-0xCB
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8}, // 0xCC-0xCF
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8}, // 0xD0-0xD3
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8}, // 0xD4-0xD7
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8}, // 0xD8-0xDB
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8}, // 0xDC-0xDF
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8}, // 0xE0-0xE3
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8}, // 0xE4-0xE7
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8}, // 0xE8-0xEB
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8}, // 0xEC-0xEF
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8}, // 0xF0-0xF3
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8}, // 0xF4-0xF7
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8}, // 0xF8-0xFB
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8},
        {"INVALID", 2, 8} // 0xFC-0xFF
};

// FD prefixed IY instructions (second byte opcodes 0x00-0xFF)
static const opcode_info_t fd_opcode_table[256] =
    {
        // 0x00-0x0F
        {"???", 2, 8},        // 0x00
        {"???", 2, 8},        // 0x01
        {"???", 2, 8},        // 0x02
        {"???", 2, 8},        // 0x03
        {"???", 2, 8},        // 0x04
        {"???", 2, 8},        // 0x05
        {"???", 2, 8},        // 0x06
        {"???", 2, 8},        // 0x07
        {"???", 2, 8},        // 0x08
        {"ADD IY,BC", 2, 15}, // 0x09
        {"???", 2, 8},        // 0x0A
        {"???", 2, 8},        // 0x0B
        {"???", 2, 8},        // 0x0C
        {"???", 2, 8},        // 0x0D
        {"???", 2, 8},        // 0x0E
        {"???", 2, 8},        // 0x0F

        // 0x10-0x1F
        {"???", 2, 8},        // 0x10
        {"???", 2, 8},        // 0x11
        {"???", 2, 8},        // 0x12
        {"???", 2, 8},        // 0x13
        {"???", 2, 8},        // 0x14
        {"???", 2, 8},        // 0x15
        {"???", 2, 8},        // 0x16
        {"???", 2, 8},        // 0x17
        {"???", 2, 8},        // 0x18
        {"ADD IY,DE", 2, 15}, // 0x19
        {"???", 2, 8},        // 0x1A
        {"???", 2, 8},        // 0x1B
        {"???", 2, 8},        // 0x1C
        {"???", 2, 8},        // 0x1D
        {"???", 2, 8},        // 0x1E
        {"???", 2, 8},        // 0x1F

        // 0x20-0x2F
        {"???", 2, 8},           // 0x20
        {"LD IY,%04X", 4, 14},   // 0x21
        {"LD (%04X),IY", 4, 20}, // 0x22
        {"INC IY", 2, 10},       // 0x23
        {"INC IYh", 2, 8},       // 0x24
        {"DEC IYh", 2, 8},       // 0x25
        {"LD IYh,%02X", 3, 11},  // 0x26
        {"???", 2, 8},           // 0x27
        {"???", 2, 8},           // 0x28
        {"ADD IY,IY", 2, 15},    // 0x29
        {"LD IY,(%04X)", 4, 20}, // 0x2A
        {"DEC IY", 2, 10},       // 0x2B
        {"INC IYl", 2, 8},       // 0x2C
        {"DEC IYl", 2, 8},       // 0x2D
        {"LD IYl,%02X", 3, 11},  // 0x2E
        {"???", 2, 8},           // 0x2F

        // 0x30-0x3F
        {"???", 2, 8},              // 0x30
        {"???", 2, 8},              // 0x31
        {"???", 2, 8},              // 0x32
        {"???", 2, 8},              // 0x33
        {"INC (IY+%d)", 3, 23},     // 0x34
        {"DEC (IY+%d)", 3, 23},     // 0x35
        {"LD (IY+%d),%02X", 4, 19}, // 0x36
        {"???", 2, 8},              // 0x37
        {"???", 2, 8},              // 0x38
        {"ADD IY,SP", 2, 15},       // 0x39
        {"???", 2, 8},              // 0x3A
        {"???", 2, 8},              // 0x3B
        {"???", 2, 8},              // 0x3C
        {"???", 2, 8},              // 0x3D
        {"???", 2, 8},              // 0x3E
        {"???", 2, 8},              // 0x3F

        // 0x40-0x4F
        {"???", 2, 8},           // 0x40
        {"???", 2, 8},           // 0x41
        {"???", 2, 8},           // 0x42
        {"???", 2, 8},           // 0x43
        {"LD B,IYh", 2, 8},      // 0x44
        {"LD B,IYl", 2, 8},      // 0x45
        {"LD B,(IY+%d)", 3, 19}, // 0x46
        {"???", 2, 8},           // 0x47
        {"???", 2, 8},           // 0x48
        {"???", 2, 8},           // 0x49
        {"???", 2, 8},           // 0x4A
        {"???", 2, 8},           // 0x4B
        {"LD C,IYh", 2, 8},      // 0x4C
        {"LD C,IYl", 2, 8},      // 0x4D
        {"LD C,(IY+%d)", 3, 19}, // 0x4E
        {"???", 2, 8},           // 0x4F

        // 0x50-0x5F
        {"???", 2, 8},           // 0x50
        {"???", 2, 8},           // 0x51
        {"???", 2, 8},           // 0x52
        {"???", 2, 8},           // 0x53
        {"LD D,IYh", 2, 8},      // 0x54
        {"LD D,IYl", 2, 8},      // 0x55
        {"LD D,(IY+%d)", 3, 19}, // 0x56
        {"???", 2, 8},           // 0x57
        {"???", 2, 8},           // 0x58
        {"???", 2, 8},           // 0x59
        {"???", 2, 8},           // 0x5A
        {"???", 2, 8},           // 0x5B
        {"LD E,IYh", 2, 8},      // 0x5C
        {"LD E,IYl", 2, 8},      // 0x5D
        {"LD E,(IY+%d)", 3, 19}, // 0x5E
        {"???", 2, 8},           // 0x5F

        // 0x60-0x6F
        {"LD IYh,B", 2, 8},      // 0x60
        {"LD IYh,C", 2, 8},      // 0x61
        {"LD IYh,D", 2, 8},      // 0x62
        {"LD IYh,E", 2, 8},      // 0x63
        {"LD IYh,IYh", 2, 8},    // 0x64
        {"LD IYh,IYl", 2, 8},    // 0x65
        {"LD H,(IY+%d)", 3, 19}, // 0x66
        {"LD IYh,A", 2, 8},      // 0x67
        {"LD IYl,B", 2, 8},      // 0x68
        {"LD IYl,C", 2, 8},      // 0x69
        {"LD IYl,D", 2, 8},      // 0x6A
        {"LD IYl,E", 2, 8},      // 0x6B
        {"LD IYl,IYh", 2, 8},    // 0x6C
        {"LD IYl,IYl", 2, 8},    // 0x6D
        {"LD L,(IY+%d)", 3, 19}, // 0x6E
        {"LD IYl,A", 2, 8},      // 0x6F

        // 0x70-0x7F
        {"LD (IY+%d),B", 3, 19}, // 0x70
        {"LD (IY+%d),C", 3, 19}, // 0x71
        {"LD (IY+%d),D", 3, 19}, // 0x72
        {"LD (IY+%d),E", 3, 19}, // 0x73
        {"LD (IY+%d),H", 3, 19}, // 0x74
        {"LD (IY+%d),L", 3, 19}, // 0x75
        {"???", 2, 8},           // 0x76
        {"LD (IY+%d),A", 3, 19}, // 0x77
        {"???", 2, 8},           // 0x78
        {"???", 2, 8},           // 0x79
        {"???", 2, 8},           // 0x7A
        {"???", 2, 8},           // 0x7B
        {"LD A,IYh", 2, 8},      // 0x7C
        {"LD A,IYl", 2, 8},      // 0x7D
        {"LD A,(IY+%d)", 3, 19}, // 0x7E
        {"???", 2, 8},           // 0x7F

        // 0x80-0x8F
        {"???", 2, 8},            // 0x80
        {"???", 2, 8},            // 0x81
        {"???", 2, 8},            // 0x82
        {"???", 2, 8},            // 0x83
        {"ADD A,IYh", 2, 8},      // 0x84
        {"ADD A,IYl", 2, 8},      // 0x85
        {"ADD A,(IY+%d)", 3, 19}, // 0x86
        {"???", 2, 8},            // 0x87
        {"???", 2, 8},            // 0x88
        {"???", 2, 8},            // 0x89
        {"???", 2, 8},            // 0x8A
        {"???", 2, 8},            // 0x8B
        {"ADC A,IYh", 2, 8},      // 0x8C
        {"ADC A,IYl", 2, 8},      // 0x8D
        {"ADC A,(IY+%d)", 3, 19}, // 0x8E
        {"???", 2, 8},            // 0x8F

        // 0x90-0x9F
        {"???", 2, 8},            // 0x90
        {"???", 2, 8},            // 0x91
        {"???", 2, 8},            // 0x92
        {"???", 2, 8},            // 0x93
        {"SUB IYh", 2, 8},        // 0x94
        {"SUB IYl", 2, 8},        // 0x95
        {"SUB (IY+%d)", 3, 19},   // 0x96
        {"???", 2, 8},            // 0x97
        {"???", 2, 8},            // 0x98
        {"???", 2, 8},            // 0x99
        {"???", 2, 8},            // 0x9A
        {"???", 2, 8},            // 0x9B
        {"SBC A,IYh", 2, 8},      // 0x9C
        {"SBC A,IYl", 2, 8},      // 0x9D
        {"SBC A,(IY+%d)", 3, 19}, // 0x9E
        {"???", 2, 8},            // 0x9F

        // 0xA0-0xAF
        {"???", 2, 8},          // 0xA0
        {"???", 2, 8},          // 0xA1
        {"???", 2, 8},          // 0xA2
        {"???", 2, 8},          // 0xA3
        {"AND IYh", 2, 8},      // 0xA4
        {"AND IYl", 2, 8},      // 0xA5
        {"AND (IY+%d)", 3, 19}, // 0xA6
        {"???", 2, 8},          // 0xA7
        {"???", 2, 8},          // 0xA8
        {"???", 2, 8},          // 0xA9
        {"???", 2, 8},          // 0xAA
        {"???", 2, 8},          // 0xAB
        {"XOR IYh", 2, 8},      // 0xAC
        {"XOR IYl", 2, 8},      // 0xAD
        {"XOR (IY+%d)", 3, 19}, // 0xAE
        {"???", 2, 8},          // 0xAF

        // 0xB0-0xBF
        {"???", 2, 8},         // 0xB0
        {"???", 2, 8},         // 0xB1
        {"???", 2, 8},         // 0xB2
        {"???", 2, 8},         // 0xB3
        {"OR IYh", 2, 8},      // 0xB4
        {"OR IYl", 2, 8},      // 0xB5
        {"OR (IY+%d)", 3, 19}, // 0xB6
        {"???", 2, 8},         // 0xB7
        {"???", 2, 8},         // 0xB8
        {"???", 2, 8},         // 0xB9
        {"???", 2, 8},         // 0xBA
        {"???", 2, 8},         // 0xBB
        {"CP IYh", 2, 8},      // 0xBC
        {"CP IYl", 2, 8},      // 0xBD
        {"CP (IY+%d)", 3, 19}, // 0xBE
        {"???", 2, 8},         // 0xBF

        // 0xC0-0xCF
        {"???", 2, 8}, // 0xC0
        {"???", 2, 8}, // 0xC1
        {"???", 2, 8}, // 0xC2
        {"???", 2, 8}, // 0xC3
        {"???", 2, 8}, // 0xC4
        {"???", 2, 8}, // 0xC5
        {"???", 2, 8}, // 0xC6
        {"???", 2, 8}, // 0xC7
        {"???", 2, 8}, // 0xC8
        {"???", 2, 8}, // 0xC9
        {"???", 2, 8}, // 0xCA
        {"???", 2, 8}, // 0xCB (FD CB handled separately)
        {"???", 2, 8}, // 0xCC
        {"???", 2, 8}, // 0xCD
        {"???", 2, 8}, // 0xCE
        {"???", 2, 8}, // 0xCF

        // 0xD0-0xDF
        {"???", 2, 8}, // 0xD0
        {"???", 2, 8}, // 0xD1
        {"???", 2, 8}, // 0xD2
        {"???", 2, 8}, // 0xD3
        {"???", 2, 8}, // 0xD4
        {"???", 2, 8}, // 0xD5
        {"???", 2, 8}, // 0xD6
        {"???", 2, 8}, // 0xD7
        {"???", 2, 8}, // 0xD8
        {"???", 2, 8}, // 0xD9
        {"???", 2, 8}, // 0xDA
        {"???", 2, 8}, // 0xDB
        {"???", 2, 8}, // 0xDC
        {"???", 2, 8}, // 0xDD (IX prefix - undefined in FD)
        {"???", 2, 8}, // 0xDE
        {"???", 2, 8}, // 0xDF

        // 0xE0-0xEF
        {"???", 2, 8},         // 0xE0
        {"POP IY", 2, 14},     // 0xE1
        {"???", 2, 8},         // 0xE2
        {"EX (SP),IY", 2, 23}, // 0xE3
        {"???", 2, 8},         // 0xE4
        {"PUSH IY", 2, 15},    // 0xE5
        {"???", 2, 8},         // 0xE6
        {"???", 2, 8},         // 0xE7
        {"???", 2, 8},         // 0xE8
        {"JP (IY)", 2, 8},     // 0xE9
        {"???", 2, 8},         // 0xEA
        {"???", 2, 8},         // 0xEB
        {"???", 2, 8},         // 0xEC
        {"???", 2, 8},         // 0xED (ED prefix - undefined in FD)
        {"???", 2, 8},         // 0xEE
        {"???", 2, 8},         // 0xEF

        // 0xF0-0xFF
        {"???", 2, 8},       // 0xF0
        {"???", 2, 8},       // 0xF1
        {"???", 2, 8},       // 0xF2
        {"???", 2, 8},       // 0xF3
        {"???", 2, 8},       // 0xF4
        {"???", 2, 8},       // 0xF5
        {"???", 2, 8},       // 0xF6
        {"???", 2, 8},       // 0xF7
        {"???", 2, 8},       // 0xF8
        {"LD SP,IY", 2, 10}, // 0xF9
        {"???", 2, 8},       // 0xFA
        {"???", 2, 8},       // 0xFB
        {"???", 2, 8},       // 0xFC
        {"???", 2, 8},       // 0xFD (FD prefix - undefined)
        {"???", 2, 8},       // 0xFE
        {"???", 2, 8}        // 0xFF
};

#endif