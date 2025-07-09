#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "z80.h"
#include "z80_flags.h"
#include "z80_opcode_table.h"

// Stack operations
void z80_stack_push8(struct z80_t *cpu, uint8_t value)
{
    cpu->registers.SP--; 
    z80_write8(cpu, cpu->registers.SP, value);
}

void z80_stack_push16(struct z80_t *cpu, uint16_t value)
{
    cpu->registers.SP--;
    z80_write8(cpu, cpu->registers.SP, (value >> 8) & 0xFF);
    cpu->registers.SP--;
    z80_write8(cpu, cpu->registers.SP, value & 0xFF);
}

uint8_t z80_stack_pop8(struct z80_t *cpu)
{
    uint8_t value = z80_read8(cpu, cpu->registers.SP);
    cpu->registers.SP++;
    return value;
}

uint16_t z80_stack_pop16(struct z80_t *cpu)
{
    uint8_t low = z80_read8(cpu, cpu->registers.SP);
    cpu->registers.SP++;
    uint8_t high = z80_read8(cpu, cpu->registers.SP);
    cpu->registers.SP++;
    
    return (high << 8) | low;
}

// Memory access functions
uint8_t z80_read8(struct z80_t *cpu, uint16_t addr)
{
    uint8_t byte = cpu->read8(cpu->memory_ctx, addr);
    cpu->cycle_count += 3; // Memory read cycle (3 T-states)
    return byte;
}

void z80_write8(struct z80_t *cpu, uint16_t addr, uint8_t value)
{
    cpu->write8(cpu->memory_ctx, addr, value);
    cpu->cycle_count += 3;
}

uint8_t z80_port_in(struct z80_t *cpu, uint8_t port)
{
    return cpu->io_read8(cpu->io_ctx, port);
}

void z80_port_out(struct z80_t *cpu, uint8_t port, uint8_t value)
{
    cpu->io_write8(cpu->io_ctx, port, value);
}

uint16_t z80_read16(struct z80_t *cpu, uint16_t addr)
{
    uint8_t low = z80_read8(cpu, addr);
    uint8_t high = z80_read8(cpu, addr + 1);
    return (high << 8) | low;
}

void z80_write16(struct z80_t *cpu, uint16_t addr, uint16_t value)
{
    z80_write8(cpu, addr, value & 0xFF);        // Low byte first
    z80_write8(cpu, addr + 1, (value >> 8) & 0xFF); // High byte second
}

uint8_t z80_fetch8(struct z80_t *cpu)
{
    uint8_t byte = z80_read8(cpu, cpu->registers.PC);
    cpu->registers.PC++;
    return byte;
}

uint16_t z80_fetch16(struct z80_t *cpu)
{
    uint16_t word = z80_read16(cpu, cpu->registers.PC);
    cpu->registers.PC += 2;
    return word;
}

uint8_t z80_fetch_opcode(struct z80_t *cpu)
{
    uint8_t opcode = z80_read8(cpu, cpu->registers.PC++);
    cpu->cycle_count += 1; // Extra T-state for opcode fetch
    return opcode;
}

// Z80 interrupt functions
void z80_set_interrupt_line(struct z80_t *cpu, bool state)
{
    cpu->interrupt_line = state;
}

void z80_handle_interrupt(struct z80_t *cpu)
{
    if (!cpu->iff1 || !cpu->interrupt_line) 
    {
        return;
    }
    
    // Handle interrupt based on interrupt mode
    switch (cpu->int_mode) {
        case 0:
            // IM 0 - not commonly used in Master System
            // Would need external device to provide instruction
            break;
            
        case 1:
            // IM 1 - Jump to $0038 (Master System default)
            cpu->halted = false;
            z80_stack_push16(cpu, cpu->registers.PC);
            cpu->registers.PC = 0x0038;
            cpu->iff1 = false;
            cpu->iff2 = false;
            cpu->cycles += 13;  // Interrupt acknowledge cycles
            break;
            
        case 2:
            // IM 2 - Vectored interrupt
            // Not typically used in Master System
            break;
    }
    
    // Clear interrupt line after acknowledgment
    cpu->interrupt_line = false;
}

void z80_check_interrupts(struct z80_t *cpu)
{
    z80_handle_interrupt(cpu);
}

int z80_step(struct z80_t *cpu)
{
    if (cpu->halted || !cpu->running)
    {
        return 0;
    }

    cpu->cycle_count = 0; // Reset cycle counter

    uint8_t opcode = z80_fetch_opcode(cpu); // 4 cycles (opcode fetch)

    z80_execute_instruction(cpu, opcode);

    cpu->cycles += cpu->cycle_count;
    return cpu->cycle_count;
}

void z80_init(struct z80_t *cpu)
{
    init_parity_table();
    // Initialize all registers to 0
    cpu->registers.PC = 0x0000;
    cpu->registers.SP = 0xFFFF; // Stack typically starts at top of memory
    cpu->registers.AF = 0x0000;
    cpu->registers.BC = 0x0000;
    cpu->registers.DE = 0x0000;
    cpu->registers.HL = 0x0000;
    cpu->registers.IX = 0x0000;
    cpu->registers.IY = 0x0000;
    cpu->registers.I = 0x00;
    cpu->registers.R = 0x00;

    // Initialize CPU state
    cpu->halted = false;
    cpu->iff1 = false;
    cpu->iff2 = false;
    cpu->int_mode = 0;
    cpu->cycles = 0;
    cpu->running = true;
    // cpu->debug = true;
}

void z80_reset(struct z80_t *cpu)
{
    cpu->registers.PC = 0x0000;
    cpu->registers.SP = 0xFFFF;
    cpu->halted = false;
    cpu->iff1 = false;
    cpu->iff2 = false;
    cpu->int_mode = 0;
    cpu->running = true;
}


void z80_run_cycles(struct z80_t *cpu, uint64_t target_cycles)
{
    uint64_t start_cycles = cpu->cycles;

    while ((cpu->cycles - start_cycles) < target_cycles && cpu->running && !cpu->halted)
    {
        z80_check_interrupts(cpu);
        if (cpu->debug)
        {
            z80_disassemble_instruction(cpu);
        }
        // Check for interrupts
        z80_step(cpu);
    }
}

void z80_print_state(struct z80_t *cpu)
{
    // printf("SP:%04X CYC:%llu AF:%04X BC:%04X DE:%04X HL:%04X "
    //        "IX:%04X IY:%04X I:%02X R:%02X FLAGS:%s/%s/%s INT:%d\n",
    //        cpu->registers.SP, cpu->cycles,
    //        cpu->registers.AF, cpu->registers.BC,
    //        cpu->registers.DE, cpu->registers.HL,
    //        cpu->registers.IX, cpu->registers.IY,
    //        cpu->registers.I, cpu->registers.R,
    //        cpu->halted ? "HALT" : "RUN",
    //        cpu->iff1 ? "EI" : "DI",
    //        cpu->running ? "ACTIVE" : "STOPPED",
    //        cpu->int_mode);

    printf("SP:%04X CYC:%llu AF:%04X BC:%04X DE:%04X HL:%04X "
        "IX:%04X IY:%04X I:%02X R:%02X\n",
        cpu->registers.SP, cpu->cycles,
        cpu->registers.AF, cpu->registers.BC,
        cpu->registers.DE, cpu->registers.HL,
        cpu->registers.IX, cpu->registers.IY,
        cpu->registers.I, cpu->registers.R);
}

// void z80_print_state(struct z80_t *cpu)
// {
//     printf("Z80 CPU State:\n");
//     printf("PC: %04X  SP: %04X  Cycles: %llu\n",
//            cpu->registers.PC, cpu->registers.SP, cpu->cycles);
//     printf("AF: %04X  BC: %04X  DE: %04X  HL: %04X\n",
//            cpu->registers.AF, cpu->registers.BC,
//            cpu->registers.DE, cpu->registers.HL);
//     printf("IX: %04X  IY: %04X  I: %02X  R: %02X\n",
//            cpu->registers.IX, cpu->registers.IY,
//            cpu->registers.I, cpu->registers.R);
//     printf("Flags: %s %s %s INT: %d\n",
//            cpu->halted ? "HALT" : "RUN",
//            cpu->iff1 ? "EI" : "DI",
//            cpu->running ? "ACTIVE" : "STOPPED",
//            cpu->int_mode);
// }