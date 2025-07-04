#include <stdint.h>
#include <stdio.h>
#include <stdint.h>
#include "core/sms.h"
#include "cpu/z80_test.h"

#define BUFFER_SIZE 16384
static uint8_t buffer[BUFFER_SIZE] = {0};
static char disasm_buffer[128];

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    struct sms_core_t sms;
    sms_create(&sms);
    sms_load_rom_file(&sms, filename);

    int instruction_count = 0;
    sms.cpu.debug = false;

    // // memory analysis 
    // for(uint8_t i = 0; i < 5; i++)
    // {
    //     uint8_t byte  = z80_read8(&sms.cpu, 0x1D45 + i);  
    //     printf("0x1D4%X : %02X\n", 5 + i, byte);
    // }

    while (sms.cpu.running && !sms.cpu.halted)
    {
        if(sms.cpu.debug)
        {
            z80_disassemble_instruction(&sms.cpu, disasm_buffer, sizeof(disasm_buffer));
            printf("%s\t", disasm_buffer);
            z80_print_state(&sms.cpu);
        }

        z80_step(&sms.cpu);
        instruction_count++;
    }
    z80_print_state(&sms.cpu);

//   // 8-bit register increment tests
//     z80_test_inc_a();
//     z80_test_inc_a_zero();
//     z80_test_inc_a_half_carry();
//     z80_test_inc_b();
//     z80_test_inc_c();

//     // 8-bit register decrement tests
//     z80_test_dec_a();
//     z80_test_dec_a_zero();
//     z80_test_dec_a_underflow();
//     z80_test_dec_b();
//     z80_test_dec_c();

//     // SBC A, reg tests
//     z80_test_sbc_a_b_simple();
//     z80_test_sbc_a_b_with_carry();
//     z80_test_sbc_a_b_borrow();
//     z80_test_sbc_a_b_borrow_with_carry();
//     z80_test_sbc_a_b_zero();
//     z80_test_sbc_a_b_half_borrow();
//     z80_test_sbc_a_b_overflow();

//     // SBC HL, reg pair tests
//     z80_test_sbc_hl_bc_simple();
//     z80_test_sbc_hl_bc_with_carry();
//     z80_test_sbc_hl_bc_borrow();
//     z80_test_sbc_hl_bc_zero();
//     z80_test_sbc_hl_bc_half_borrow();
//     z80_test_sbc_hl_bc_overflow();
    
//     // Memory increment/decrement tests
//     z80_test_inc_hl_indirect();
//     z80_test_dec_hl_indirect();

//     // 16-bit register increment/decrement tests
//     z80_test_inc_bc();
//     z80_test_dec_bc();
//     z80_test_inc_bc_carry();
    return 0;
}