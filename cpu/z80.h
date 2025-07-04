#ifndef Z80_H_
#define Z80_H_

#include <stdint.h>
#include <stdbool.h>

struct registers
{
    uint16_t PC; // Program Counter
    uint16_t SP; // Stack Pointer
    uint8_t I;
    uint8_t R;
    
    union
    {
        uint16_t AF;
        struct
        {
            uint8_t F;
            uint8_t A;
        };
    };
    union
    {
        uint16_t BC;
        struct
        {
            uint8_t C;
            uint8_t B;
        };
    };
    union
    {
        uint16_t DE;
        struct
        {
            uint8_t E;
            uint8_t D;
        };
    };
    union
    {
        uint16_t HL;
        struct
        {
            uint8_t L;
            uint8_t H;
        };
    };
    union
    {
        uint16_t IX;
        struct
        {
            uint8_t IXL;
            uint8_t IXH;
        };
    };
    union
    {
        uint16_t IY;
        struct
        {
            uint8_t IYL;
            uint8_t IYH;
        };
    };
    union
    {
        uint16_t _AF;
        struct
        {
            uint8_t _F;
            uint8_t _A;
        };
    };
    union
    {
        uint16_t _BC;
        struct
        {
            uint8_t _C;
            uint8_t _B;
        };
    };
    union
    {
        uint16_t _DE;
        struct
        {
            uint8_t _E;
            uint8_t _D;
        };
    };
    union
    {
        uint16_t _HL;
        struct
        {
            uint8_t _L;
            uint8_t _H;
        };
    };
};

struct z80_t
{
    struct registers registers;
    // CPU state
    bool halted;
    bool iff1, iff2;
    uint8_t int_mode;
    // Execution
    uint64_t cycles;
    uint8_t cycle_count;
    bool running;
    bool debug;

    // Memory interface function pointers
    uint8_t (*read8)(void* context, uint16_t addr);
    uint16_t (*read16)(void* context, uint16_t addr);
    void (*write8)(void* context, uint16_t addr, uint8_t value);
    void (*write16)(void* context, uint16_t addr, uint16_t value);
    uint8_t (*io_read8)(void* context, uint8_t port);
    void (*io_write8)(void* context, uint8_t port, uint8_t value);
    void *memory_ctx;
};

void z80_init(struct z80_t* cpu);
void z80_reset(struct z80_t* cpu);
int z80_step(struct z80_t* cpu);
void z80_execute_instruction(struct z80_t *cpu, uint8_t opcode);
void z80_execute_cb_instruction(struct z80_t *cpu, uint8_t opcode);
void z80_execute_ed_instruction(struct z80_t *cpu, uint8_t opcode);
void z80_execute_fd_instruction(struct z80_t *cpu, uint8_t opcode);
void z80_execute_dd_instruction(struct z80_t *cpu, uint8_t opcode);
void z80_execute_dd_cb_instruction(struct z80_t *cpu, int8_t d);
void z80_execute_fd_cb_instruction(struct z80_t *cpu, int8_t d);
void z80_run_cycles(struct z80_t* cpu, uint64_t target_cycles);
void z80_print_state(struct z80_t* cpu);
void z80_write8(struct z80_t *cpu, uint16_t addr, uint8_t  data);
void z80_write16(struct z80_t *cpu, uint16_t addr, uint16_t data);
uint8_t z80_read8(struct z80_t* cpu, uint16_t addr);
uint16_t z80_read16(struct z80_t* cpu, uint16_t addr);
void z80_disassemble_instruction(struct z80_t* cpu, char* buffer, size_t buffer_size);
void z80_disassemble_instruction_verbose(struct z80_t *cpu, char *buffer, size_t buffer_size);
void z80_stack_push8(struct z80_t *cpu, uint8_t value);
void z80_stack_push16(struct z80_t *cpu, uint16_t value);
uint8_t z80_stack_pop8(struct z80_t *cpu);
uint16_t z80_stack_pop16(struct z80_t *cpu);
uint8_t z80_read8(struct z80_t *cpu, uint16_t addr);
uint16_t z80_read16(struct z80_t *cpu, uint16_t addr);
void z80_write8(struct z80_t *cpu, uint16_t addr, uint8_t value);
void z80_write16(struct z80_t *cpu, uint16_t addr, uint16_t value);
uint8_t z80_fetch8(struct z80_t *cpu);
uint16_t z80_fetch16(struct z80_t *cpu);
uint8_t z80_fetch_opcode(struct z80_t *cpu);
#endif