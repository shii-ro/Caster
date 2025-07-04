#ifndef Z80_TEST_H_
#define Z80_TEST_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "z80.h"

// Test result structure
typedef struct
{
    bool passed;
    char error_msg[256];
    int line;
} test_result_t;

// Test context structure
typedef struct
{
    char name[64];
    struct z80_t cpu;
    struct registers initial_state;
    struct registers expected_state;
    uint8_t memory[65536];
    uint8_t io_ports[256];
    uint64_t expected_cycles;
    bool check_cycles;
    bool check_memory;
    uint16_t memory_check_start;
    uint16_t memory_check_end;
    uint8_t expected_memory[65536];
} test_context_t;


// Initialize test context
void z80_test_init(test_context_t *ctx, const char *test_name);
// Set initial CPU register state
void z80_test_set_registers(test_context_t *ctx, struct registers *regs);

// Set expected CPU register state
void z80_test_expect_registers(test_context_t *ctx, struct registers *regs);

// Set memory content
void z80_test_set_memory(test_context_t *ctx, uint16_t addr, uint8_t *data, size_t size);
// Set single memory byte
void z80_test_set_memory_byte(test_context_t *ctx, uint16_t addr, uint8_t value);
// Set expected memory content
void z80_test_expect_memory(test_context_t *ctx, uint16_t start_addr, uint16_t end_addr, uint8_t *expected_data);

// Set expected memory byte
void z80_test_expect_memory_byte(test_context_t *ctx, uint16_t addr, uint8_t expected_value);

// Set expected cycle count
void z80_test_expect_cycles(test_context_t *ctx, uint64_t cycles);

// Set I/O port value
void z80_test_set_io_port(test_context_t *ctx, uint8_t port, uint8_t value);
// Compare register states
bool z80_test_compare_registers(struct registers *actual, struct registers *expected, char *error_msg);
// Execute test and compare results
test_result_t z80_test_run(test_context_t *ctx);
// Print test results
void z80_test_print_result(test_context_t *ctx, test_result_t *result);
// Print test summary
void z80_test_print_summary(void);
// Helper function to create register state
struct registers z80_test_make_registers(uint16_t pc, uint16_t sp, uint8_t a, uint8_t f,
                                         uint8_t b, uint8_t c, uint8_t d, uint8_t e,
                                         uint8_t h, uint8_t l);

                                         // Sets the initial memory value for tests involving the (HL) address.
void setup_hl_indirect(test_context_t *ctx);
// Checks the expected memory value after an INC (HL) instruction.
void check_inc_hl_indirect(test_context_t *ctx);
// Checks the expected memory value after a DEC (HL) instruction.
void check_dec_hl_indirect(test_context_t *ctx);

void z80_perform_test(const char* name,
                      uint8_t opcode,
                       struct registers* initial,
                       struct registers* expected,
                      void (*setup_extra)(test_context_t* ctx),
                      void (*check_extra)(test_context_t* ctx));
#endif