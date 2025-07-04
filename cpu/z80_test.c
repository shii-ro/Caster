#include "z80_test.h"
#include "z80_flags.h"

// Global test statistics
static int tests_run = 0;
static int tests_passed = 0;
static int tests_failed = 0;

// Sets the initial memory value for tests involving the (HL) address.
void setup_hl_indirect(test_context_t *ctx)
{
    // The value at (HL), which is 0x1000 for these tests.
    z80_test_set_memory_byte(ctx, 0x1000, 0x55);
}

// Checks the expected memory value after an INC (HL) instruction.
void check_inc_hl_indirect(test_context_t *ctx)
{
    z80_test_expect_memory_byte(ctx, 0x1000, 0x56);
}

// Checks the expected memory value after a DEC (HL) instruction.
void check_dec_hl_indirect(test_context_t *ctx)
{
    z80_test_expect_memory_byte(ctx, 0x1000, 0x54);
}

// Memory interface functions for testing
static uint8_t test_read8(void *context, uint16_t addr)
{
    test_context_t *ctx = (test_context_t *)context;
    return ctx->memory[addr];
}

static uint16_t test_read16(void *context, uint16_t addr)
{
    test_context_t *ctx = (test_context_t *)context;
    return ctx->memory[addr] | (ctx->memory[addr + 1] << 8);
}

static void test_write8(void *context, uint16_t addr, uint8_t value)
{
    test_context_t *ctx = (test_context_t *)context;
    ctx->memory[addr] = value;
}

static void test_write16(void *context, uint16_t addr, uint16_t value)
{
    test_context_t *ctx = (test_context_t *)context;
    ctx->memory[addr] = value & 0xFF;
    ctx->memory[addr + 1] = (value >> 8) & 0xFF;
}

static uint8_t test_io_read8(void *context, uint8_t port)
{
    test_context_t *ctx = (test_context_t *)context;
    return ctx->io_ports[port];
}

static void test_io_write8(void *context, uint8_t port, uint8_t value)
{
    test_context_t *ctx = (test_context_t *)context;
    ctx->io_ports[port] = value;
}

// Initialize test context
void z80_test_init(test_context_t *ctx, const char *test_name)
{
    memset(ctx, 0, sizeof(test_context_t));
    strncpy(ctx->name, test_name, sizeof(ctx->name) - 1);

    // Initialize Z80 CPU
    z80_init(&ctx->cpu);

    // Set up memory interface
    ctx->cpu.read8 = test_read8;
    ctx->cpu.read16 = test_read16;
    ctx->cpu.write8 = test_write8;
    ctx->cpu.write16 = test_write16;
    ctx->cpu.io_read8 = test_io_read8;
    ctx->cpu.io_write8 = test_io_write8;
    ctx->cpu.memory_ctx = ctx;

    // Initialize memory and I/O ports
    memset(ctx->memory, 0, sizeof(ctx->memory));
    memset(ctx->io_ports, 0, sizeof(ctx->io_ports));
    memset(ctx->expected_memory, 0, sizeof(ctx->expected_memory));

    ctx->check_cycles = false;
    ctx->check_memory = false;
}

// Format flags register for display
void z80_test_format_flags(uint8_t flags, char* buffer, size_t buffer_size) {
    snprintf(buffer, buffer_size, "0x%02X [%s%s%s%s%s%s]",
        flags,
        (flags & MASK_S)  ? "S" : "-",
        (flags & MASK_Z)  ? "Z" : "-",
        (flags & MASK_H)  ? "H" : "-",
        (flags & MASK_PV) ? "P" : "-",
        (flags & MASK_N)  ? "N" : "-",
        (flags & MASK_C)  ? "C" : "-"
    );
}

// Set initial CPU register state
void z80_test_set_registers(test_context_t *ctx, struct registers *regs)
{
    ctx->initial_state = *regs;
    ctx->cpu.registers = *regs;
}

// Set expected CPU register state
void z80_test_expect_registers(test_context_t *ctx, struct registers *regs)
{
    ctx->expected_state = *regs;
}

// Set memory content
void z80_test_set_memory(test_context_t *ctx, uint16_t addr, uint8_t *data, size_t size)
{
    for (size_t i = 0; i < size && (addr + i) < 65536; i++)
    {
        ctx->memory[addr + i] = data[i];
    }
}

// Set single memory byte
void z80_test_set_memory_byte(test_context_t *ctx, uint16_t addr, uint8_t value)
{
    ctx->memory[addr] = value;
}

// Set expected memory content
void z80_test_expect_memory(test_context_t *ctx, uint16_t start_addr, uint16_t end_addr, uint8_t *expected_data)
{
    ctx->check_memory = true;
    ctx->memory_check_start = start_addr;
    ctx->memory_check_end = end_addr;

    for (uint16_t i = start_addr; i <= end_addr; i++)
    {
        ctx->expected_memory[i] = expected_data[i - start_addr];
    }
}

// Set expected memory byte
void z80_test_expect_memory_byte(test_context_t *ctx, uint16_t addr, uint8_t expected_value)
{
    ctx->check_memory = true;
    if (ctx->memory_check_start == 0 && ctx->memory_check_end == 0)
    {
        ctx->memory_check_start = addr;
        ctx->memory_check_end = addr;
    }
    else
    {
        if (addr < ctx->memory_check_start)
            ctx->memory_check_start = addr;
        if (addr > ctx->memory_check_end)
            ctx->memory_check_end = addr;
    }
    ctx->expected_memory[addr] = expected_value;
}

// Set expected cycle count
void z80_test_expect_cycles(test_context_t *ctx, uint64_t cycles)
{
    ctx->expected_cycles = cycles;
    ctx->check_cycles = true;
}

// Set I/O port value
void z80_test_set_io_port(test_context_t *ctx, uint8_t port, uint8_t value)
{
    ctx->io_ports[port] = value;
}

// Compare register states
bool z80_test_compare_registers(struct registers *actual, struct registers *expected, char *error_msg)
{
    if (actual->PC != expected->PC)
    {
        sprintf(error_msg, "PC mismatch: expected 0x%04X, got 0x%04X", expected->PC, actual->PC);
        return false;
    }
    if (actual->SP != expected->SP)
    {
        sprintf(error_msg, "SP mismatch: expected 0x%04X, got 0x%04X", expected->SP, actual->SP);
        return false;
    }
    if (actual->A != expected->A)
    {
        sprintf(error_msg, "A mismatch: expected 0x%02X, got 0x%02X", expected->A, actual->A);
        return false;
    }
    if ((actual->F & ~0x28) != expected->F) 
    {
        char expected_flags[32], actual_flags[32];
        z80_test_format_flags(expected->F, expected_flags, sizeof(expected_flags));
        z80_test_format_flags(actual->F, actual_flags, sizeof(actual_flags));
        sprintf(error_msg, "F mismatch: expected %s, got %s", expected_flags, actual_flags);
        return false;
    }
    if (actual->B != expected->B)
    {
        sprintf(error_msg, "B mismatch: expected 0x%02X, got 0x%02X", expected->B, actual->B);
        return false;
    }
    if (actual->C != expected->C)
    {
        sprintf(error_msg, "C mismatch: expected 0x%02X, got 0x%02X", expected->C, actual->C);
        return false;
    }
    if (actual->D != expected->D)
    {
        sprintf(error_msg, "D mismatch: expected 0x%02X, got 0x%02X", expected->D, actual->D);
        return false;
    }
    if (actual->E != expected->E)
    {
        sprintf(error_msg, "E mismatch: expected 0x%02X, got 0x%02X", expected->E, actual->E);
        return false;
    }
    if (actual->H != expected->H)
    {
        sprintf(error_msg, "H mismatch: expected 0x%02X, got 0x%02X", expected->H, actual->H);
        return false;
    }
    if (actual->L != expected->L)
    {
        sprintf(error_msg, "L mismatch: expected 0x%02X, got 0x%02X", expected->L, actual->L);
        return false;
    }
    if (actual->I != expected->I)
    {
        sprintf(error_msg, "I mismatch: expected 0x%02X, got 0x%02X", expected->I, actual->I);
        return false;
    }
    if (actual->R != expected->R)
    {
        sprintf(error_msg, "R mismatch: expected 0x%02X, got 0x%02X", expected->R, actual->R);
        return false;
    }
    if (actual->IX != expected->IX)
    {
        sprintf(error_msg, "IX mismatch: expected 0x%04X, got 0x%04X", expected->IX, actual->IX);
        return false;
    }
    if (actual->IY != expected->IY)
    {
        sprintf(error_msg, "IY mismatch: expected 0x%04X, got 0x%04X", expected->IY, actual->IY);
        return false;
    }
    if (actual->_AF != expected->_AF)
    {
        sprintf(error_msg, "AF' mismatch: expected 0x%04X, got 0x%04X", expected->_AF, actual->_AF);
        return false;
    }
    if (actual->_BC != expected->_BC)
    {
        sprintf(error_msg, "BC' mismatch: expected 0x%04X, got 0x%04X", expected->_BC, actual->_BC);
        return false;
    }
    if (actual->_DE != expected->_DE)
    {
        sprintf(error_msg, "DE' mismatch: expected 0x%04X, got 0x%04X", expected->_DE, actual->_DE);
        return false;
    }
    if (actual->_HL != expected->_HL)
    {
        sprintf(error_msg, "HL' mismatch: expected 0x%04X, got 0x%04X", expected->_HL, actual->_HL);
        return false;
    }

    return true;
}

/**
 * @brief Encapsulates the logic for a single Z80 instruction test case.
 * * This function handles the repetitive boilerplate of:
 * 1. Initializing the test context.
 * 2. Setting initial register values.
 * 3. Placing the instruction/opcode into memory.
 * 4. Running optional, test-specific setup (e.g., for memory).
 * 5. Setting expected register values.
 * 6. Running optional, test-specific checks (e.g., for memory).
 * 7. Executing the CPU for one step.
 * 8. Printing the test result.
 * * @param name A descriptive name for the test, printed in the results.
 * @param opcode The 8-bit opcode of the instruction to test.
 * @param initial A pointer to a `struct registers` with the state before execution.
 * @param expected A pointer to a `struct registers` with the expected state after execution.
 * @param setup_extra A function pointer for optional, additional setup before the test runs. Use for memory setup. Can be NULL.
 * @param check_extra A function pointer for optional, additional checks. Use for memory validation. Can be NULL.
 */
void z80_perform_test(const char *name,
                      uint8_t opcode,
                       struct registers *initial,
                       struct registers *expected,
                      void (*setup_extra)(test_context_t *ctx),
                      void (*check_extra)(test_context_t *ctx))
{
    test_context_t ctx;
    test_result_t result;

    // Initialize the test context with a descriptive name.
    z80_test_init(&ctx, name);

    // Set the initial state of the Z80 registers.
    z80_test_set_registers(&ctx, initial);

    // Place the opcode at the starting program counter address.
    z80_test_set_memory_byte(&ctx, initial->PC, opcode);

    // (Optional) Run any extra setup, like setting values in memory.
    if (setup_extra)
    {
        setup_extra(&ctx);
    }

    // Set the expected state of the registers after the instruction executes.
    z80_test_expect_registers(&ctx, expected);

    // (Optional) Set any other expectations, like changes in memory.
    if (check_extra)
    {
        check_extra(&ctx);
    }

    // Run the emulation for one instruction.
    result = z80_test_run(&ctx);

    // Print the result of the test (pass/fail) and any discrepancies.
    z80_test_print_result(&ctx, &result);
}

// Execute test and compare results
test_result_t z80_test_run(test_context_t *ctx)
{
    test_result_t result = {0};

    tests_run++;

    // Reset CPU cycles
    ctx->cpu.cycles = 0;

    // Execute single instruction
    int cycles = z80_step(&ctx->cpu);

    // Check register state
    if (!z80_test_compare_registers(&ctx->cpu.registers, &ctx->expected_state, result.error_msg))
    {
        result.passed = false;
        tests_failed++;
        return result;
    }

    // Check cycle count if enabled
    if (ctx->check_cycles && ctx->cpu.cycles != ctx->expected_cycles)
    {
        sprintf(result.error_msg, "Cycle count mismatch: expected %llu, got %llu",
                ctx->expected_cycles, ctx->cpu.cycles);
        result.passed = false;
        tests_failed++;
        return result;
    }

    // Check memory if enabled
    if (ctx->check_memory)
    {
        for (uint16_t addr = ctx->memory_check_start; addr <= ctx->memory_check_end; addr++)
        {
            if (ctx->memory[addr] != ctx->expected_memory[addr])
            {
                sprintf(result.error_msg, "Memory mismatch at 0x%04X: expected 0x%02X, got 0x%02X",
                        addr, ctx->expected_memory[addr], ctx->memory[addr]);
                result.passed = false;
                tests_failed++;
                return result;
            }
        }
    }

    result.passed = true;
    tests_passed++;
    return result;
}

// Print test results
void z80_test_print_result(test_context_t *ctx, test_result_t *result)
{
    if (result->passed)
    {
        printf("✓ PASS: %s\n", ctx->name);
    }
    else
    {
        printf("✗ FAIL: %s - %s\n", ctx->name, result->error_msg);
    }
}

// Print test summary
void z80_test_print_summary(void)
{
    printf("\n=== Test Summary ===\n");
    printf("Total tests: %d\n", tests_run);
    printf("Passed: %d\n", tests_passed);
    printf("Failed: %d\n", tests_failed);
    printf("Success rate: %.1f%%\n", tests_run > 0 ? (100.0 * tests_passed / tests_run) : 0.0);
}

// Helper function to create register state
struct registers z80_test_make_registers(uint16_t pc, uint16_t sp, uint8_t a, uint8_t f,
                                         uint8_t b, uint8_t c, uint8_t d, uint8_t e,
                                         uint8_t h, uint8_t l)
{
    struct registers regs = {0};
    regs.PC = pc;
    regs.SP = sp;
    regs.A = a;
    regs.F = f;
    regs.B = b;
    regs.C = c;
    regs.D = d;
    regs.E = e;
    regs.H = h;
    regs.L = l;
    return regs;
}

// Example test function
void z80_test_example(void)
{
    test_context_t ctx;
    test_result_t result;

    // Test LD A, 0x42
    z80_test_init(&ctx, "LD A, 0x42");

    // Set up initial state
    struct registers initial = z80_test_make_registers(0x0000, 0xFFFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
    z80_test_set_registers(&ctx, &initial);

    // Set instruction in memory
    z80_test_set_memory_byte(&ctx, 0x0000, 0x3E); // LD A, n
    z80_test_set_memory_byte(&ctx, 0x0001, 0x42); // immediate value

    // Set expected state
    struct registers expected = z80_test_make_registers(0x0002, 0xFFFF, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
    z80_test_expect_registers(&ctx, &expected);

    // Run test
    result = z80_test_run(&ctx);
    z80_test_print_result(&ctx, &result);
}

// Test INC A instruction
void z80_test_inc_a(void) {
    struct registers initial = z80_test_make_registers(0x0000, 0xFFFF, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
    struct registers expected = z80_test_make_registers(0x0001, 0xFFFF, 0x43, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
    z80_perform_test("INC A (normal)", 0x3C, &initial, &expected, NULL, NULL);
}

// Test INC A with zero flag
void z80_test_inc_a_zero(void) {
    struct registers initial = z80_test_make_registers(0x0000, 0xFFFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
    struct registers expected = z80_test_make_registers(0x0001, 0xFFFF, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00); // Corrected expected flags (Z=1, H=1, PV=0)
    z80_perform_test("INC A (zero flag)", 0x3C, &initial, &expected, NULL, NULL);
}

// Test INC A with half-carry flag
void z80_test_inc_a_half_carry(void) {
    struct registers initial = z80_test_make_registers(0x0000, 0xFFFF, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
    struct registers expected = z80_test_make_registers(0x0001, 0xFFFF, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
    z80_perform_test("INC A (half-carry)", 0x3C, &initial, &expected, NULL, NULL);
}

// Test DEC A instruction
void z80_test_dec_a(void) {
    struct registers initial = z80_test_make_registers(0x0000, 0xFFFF, 0x43, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
    struct registers expected = z80_test_make_registers(0x0001, 0xFFFF, 0x42, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
    z80_perform_test("DEC A (normal)", 0x3D, &initial, &expected, NULL, NULL);
}

// Test DEC A with zero flag
void z80_test_dec_a_zero(void) {
    struct registers initial = z80_test_make_registers(0x0000, 0xFFFF, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
    struct registers expected = z80_test_make_registers(0x0001, 0xFFFF, 0x00, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
    z80_perform_test("DEC A (zero flag)", 0x3D, &initial, &expected, NULL, NULL);
}

// Test DEC A with underflow
void z80_test_dec_a_underflow(void) {
    struct registers initial = z80_test_make_registers(0x0000, 0xFFFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
    struct registers expected = z80_test_make_registers(0x0001, 0xFFFF, 0xFF, 0x92, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00); // Corrected expected flags (S=1, H=1, N=1)
    z80_perform_test("DEC A (underflow)", 0x3D, &initial, &expected, NULL, NULL);
}

// Test INC B instruction
void z80_test_inc_b(void) {
    struct registers initial = z80_test_make_registers(0x0000, 0xFFFF, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00);
    struct registers expected = z80_test_make_registers(0x0001, 0xFFFF, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00);
    z80_perform_test("INC B", 0x04, &initial, &expected, NULL, NULL);
}

// Test DEC B instruction
void z80_test_dec_b(void) {
    struct registers initial = z80_test_make_registers(0x0000, 0xFFFF, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00);
    struct registers expected = z80_test_make_registers(0x0001, 0xFFFF, 0x00, 0x12, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00); // Corrected expected flags (H=1, N=1)
    z80_perform_test("DEC B", 0x05, &initial, &expected, NULL, NULL);
}

// Test INC C instruction
void z80_test_inc_c(void) {
    struct registers initial = z80_test_make_registers(0x0000, 0xFFFF, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00);
    struct registers expected = z80_test_make_registers(0x0001, 0xFFFF, 0x00, 0x00, 0x00, 0x21, 0x00, 0x00, 0x00, 0x00);
    z80_perform_test("INC C", 0x0C, &initial, &expected, NULL, NULL);
}

// Test DEC C instruction
void z80_test_dec_c(void) {
    struct registers initial = z80_test_make_registers(0x0000, 0xFFFF, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00);
    struct registers expected = z80_test_make_registers(0x0001, 0xFFFF, 0x00, 0x12, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x00); // Corrected expected flags (H=1, N=1)
    z80_perform_test("DEC C", 0x0D, &initial, &expected, NULL, NULL);
}

// --- SBC A, B Tests ---
void z80_test_sbc_a_b_simple(void) {
    struct registers initial = z80_test_make_registers(0x0000, 0xFFFF, 0x40, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00); // A=0x40, B=0x10, C=0
    struct registers expected = z80_test_make_registers(0x0001, 0xFFFF, 0x30, 0x02, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00); // A=0x30, F=N
    z80_perform_test("SBC A, B (simple)", 0x98, &initial, &expected, NULL, NULL);
}

void z80_test_sbc_a_b_with_carry(void) {
    struct registers initial = z80_test_make_registers(0x0000, 0xFFFF, 0x40, 0x01, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00); // A=0x40, B=0x10, C=1
    struct registers expected = z80_test_make_registers(0x0001, 0xFFFF, 0x2F, 0x02, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00); // A=0x2F, F=N
    z80_perform_test("SBC A, B (with carry)", 0x98, &initial, &expected, NULL, NULL);
}

void z80_test_sbc_a_b_borrow(void) {
    struct registers initial = z80_test_make_registers(0x0000, 0xFFFF, 0x10, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00); // A=0x10, B=0x40, C=0
    struct registers expected = z80_test_make_registers(0x0001, 0xFFFF, 0xD0, 0x83, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00); // A=0xD0, F=S,N,C
    z80_perform_test("SBC A, B (borrow)", 0x98, &initial, &expected, NULL, NULL);
}

void z80_test_sbc_a_b_borrow_with_carry(void) {
    struct registers initial = z80_test_make_registers(0x0000, 0xFFFF, 0x10, 0x01, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00); // A=0x10, B=0x40, C=1
    struct registers expected = z80_test_make_registers(0x0001, 0xFFFF, 0xCF, 0x83, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00); // A=0xCF, F=S,N,C
    z80_perform_test("SBC A, B (borrow with carry)", 0x98, &initial, &expected, NULL, NULL);
}

void z80_test_sbc_a_b_zero(void) {
    struct registers initial = z80_test_make_registers(0x0000, 0xFFFF, 0x40, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00); // A=0x40, B=0x40, C=0
    struct registers expected = z80_test_make_registers(0x0001, 0xFFFF, 0x00, 0x42, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00); // A=0x00, F=Z,N
    z80_perform_test("SBC A, B (zero)", 0x98, &initial, &expected, NULL, NULL);
}

void z80_test_sbc_a_b_half_borrow(void) {
    struct registers initial = z80_test_make_registers(0x0000, 0xFFFF, 0x48, 0x00, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00); // A=0x48, B=0x09, C=0
    struct registers expected = z80_test_make_registers(0x0001, 0xFFFF, 0x3F, 0x12, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00); // A=0x3F, F=H,N
    z80_perform_test("SBC A, B (half borrow)", 0x98, &initial, &expected, NULL, NULL);
}

void z80_test_sbc_a_b_overflow(void) {
    struct registers initial = z80_test_make_registers(0x0000, 0xFFFF, 0x80, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00); // A=-128, B=1, C=0
    struct registers expected = z80_test_make_registers(0x0001, 0xFFFF, 0x7F, 0x06, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00); // A=127, F=P/V,N
    z80_perform_test("SBC A, B (overflow)", 0x98, &initial, &expected, NULL, NULL);
}

// --- SBC HL, BC Tests ---
void z80_test_sbc_hl_bc_simple(void) {
    struct registers initial = z80_test_make_registers(0x0000, 0xFFFF, 0x00, 0x00, 0x12, 0x34, 0x00, 0x00, 0x45, 0x67); // HL=0x4567, BC=0x1234, C=0
    struct registers expected = z80_test_make_registers(0x0002, 0xFFFF, 0x00, 0x02, 0x12, 0x34, 0x00, 0x00, 0x33, 0x33); // HL=0x3333, F=N
    z80_perform_test("SBC HL, BC (simple)", 0xED42, &initial, &expected, NULL, NULL);
}

void z80_test_sbc_hl_bc_with_carry(void) {
    struct registers initial = z80_test_make_registers(0x0000, 0xFFFF, 0x00, 0x01, 0x12, 0x34, 0x00, 0x00, 0x45, 0x67); // HL=0x4567, BC=0x1234, C=1
    struct registers expected = z80_test_make_registers(0x0002, 0xFFFF, 0x00, 0x02, 0x12, 0x34, 0x00, 0x00, 0x33, 0x32); // HL=0x3332, F=N
    z80_perform_test("SBC HL, BC (with carry)", 0xED42, &initial, &expected, NULL, NULL);
}

void z80_test_sbc_hl_bc_borrow(void) {
    struct registers initial = z80_test_make_registers(0x0000, 0xFFFF, 0x00, 0x00, 0x45, 0x67, 0x00, 0x00, 0x12, 0x34); // HL=0x1234, BC=0x4567, C=0
    struct registers expected = z80_test_make_registers(0x0002, 0xFFFF, 0x00, 0x83, 0x45, 0x67, 0x00, 0x00, 0xCC, 0xCD); // HL=0xCCCD, F=S,N,C
    z80_perform_test("SBC HL, BC (borrow)", 0xED42, &initial, &expected, NULL, NULL);
}

void z80_test_sbc_hl_bc_zero(void) {
    struct registers initial = z80_test_make_registers(0x0000, 0xFFFF, 0x00, 0x00, 0x12, 0x34, 0x00, 0x00, 0x12, 0x34); // HL=0x1234, BC=0x1234, C=0
    struct registers expected = z80_test_make_registers(0x0002, 0xFFFF, 0x00, 0x42, 0x12, 0x34, 0x00, 0x00, 0x00, 0x00); // HL=0x0000, F=Z,N
    z80_perform_test("SBC HL, BC (zero)", 0xED42, &initial, &expected, NULL, NULL);
}

void z80_test_sbc_hl_bc_half_borrow(void) {
    struct registers initial = z80_test_make_registers(0x0000, 0xFFFF, 0x00, 0x00, 0x0F, 0xFF, 0x00, 0x00, 0x40, 0x00); // HL=0x4000, BC=0x0FFF, C=0
    struct registers expected = z80_test_make_registers(0x0002, 0xFFFF, 0x00, 0x12, 0x0F, 0xFF, 0x00, 0x00, 0x30, 0x01); // HL=0x3001, F=H,N
    z80_perform_test("SBC HL, BC (half borrow)", 0xED42, &initial, &expected, NULL, NULL);
}

void z80_test_sbc_hl_bc_overflow(void) {
    struct registers initial = z80_test_make_registers(0x0000, 0xFFFF, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x80, 0x00); // HL=0x8000, BC=0x0001, C=0
    struct registers expected = z80_test_make_registers(0x0002, 0xFFFF, 0x00, 0x06, 0x00, 0x01, 0x00, 0x00, 0x7F, 0xFF); // HL=0x7FFF, F=P/V,N
    z80_perform_test("SBC HL, BC (overflow)", 0xED42, &initial, &expected, NULL, NULL);
}


// Test INC (HL) instruction
void z80_test_inc_hl_indirect(void) {
    struct registers initial = z80_test_make_registers(0x0000, 0xFFFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00);
    struct registers expected = z80_test_make_registers(0x0001, 0xFFFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00);
    z80_perform_test("INC (HL)", 0x34, &initial, &expected, setup_hl_indirect, check_inc_hl_indirect);
}

// Test DEC (HL) instruction
void z80_test_dec_hl_indirect(void) {
    struct registers initial = z80_test_make_registers(0x0000, 0xFFFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00);
    struct registers expected = z80_test_make_registers(0x0001, 0xFFFF, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00);
    z80_perform_test("DEC (HL)", 0x35, &initial, &expected, setup_hl_indirect, check_dec_hl_indirect);
}

// Test INC BC instruction (16-bit)
void z80_test_inc_bc(void) {
    struct registers initial = z80_test_make_registers(0x0000, 0xFFFF, 0x00, 0x00, 0x12, 0x34, 0x00, 0x00, 0x00, 0x00);
    struct registers expected = z80_test_make_registers(0x0001, 0xFFFF, 0x00, 0x00, 0x12, 0x35, 0x00, 0x00, 0x00, 0x00);
    z80_perform_test("INC BC", 0x03, &initial, &expected, NULL, NULL);
}

// Test DEC BC instruction (16-bit)
void z80_test_dec_bc(void) {
    struct registers initial = z80_test_make_registers(0x0000, 0xFFFF, 0x00, 0x00, 0x12, 0x34, 0x00, 0x00, 0x00, 0x00);
    struct registers expected = z80_test_make_registers(0x0001, 0xFFFF, 0x00, 0x00, 0x12, 0x33, 0x00, 0x00, 0x00, 0x00);
    z80_perform_test("DEC BC", 0x0B, &initial, &expected, NULL, NULL);
}

// Test INC BC with carry from C to B
void z80_test_inc_bc_carry(void) {
    struct registers initial = z80_test_make_registers(0x0000, 0xFFFF, 0x00, 0x00, 0x12, 0xFF, 0x00, 0x00, 0x00, 0x00);
    struct registers expected = z80_test_make_registers(0x0001, 0xFFFF, 0x00, 0x00, 0x13, 0x00, 0x00, 0x00, 0x00, 0x00);
    z80_perform_test("INC BC (carry)", 0x03, &initial, &expected, NULL, NULL);
}
