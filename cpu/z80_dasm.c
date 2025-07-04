#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "z80.h"
#include "z80_opcode_table.h"

// Table lookup function
const opcode_info_t *get_opcode_info(struct z80_t *cpu, uint16_t pc, int *total_length, char *prefix_bytes, size_t prefix_size)
{
    uint8_t opcode = z80_read8(cpu, pc);
    const opcode_info_t *info;

    // Clear prefix bytes buffer
    memset(prefix_bytes, 0, prefix_size);

    switch (opcode)
    {
    case 0xED: // Extended instructions
    {
        if (prefix_size >= 2)
        {
            prefix_bytes[0] = opcode;
            uint8_t sub_opcode = z80_read8(cpu, pc + 1);
            prefix_bytes[1] = sub_opcode;
        }
        info = &ed_opcode_table[z80_read8(cpu, pc + 1)];
        break;
    }

    case 0xCB: // Bit operations (for future implementation)
    {
        if (prefix_size >= 2)
        {
            prefix_bytes[0] = opcode;
            uint8_t sub_opcode = z80_read8(cpu, pc + 1);
            prefix_bytes[1] = sub_opcode;
        }
        // info = &bit_opcode_table[z80_read8(cpu, pc + 1)];  // Uncomment when CB table is ready
        info = &opcode_table[opcode]; // Fallback for now
        break;
    }

    case 0xDD: // IX prefix
    {
        if (prefix_size >= 2)
        {
            prefix_bytes[0] = opcode;
            uint8_t sub_opcode = z80_read8(cpu, pc + 1);
            prefix_bytes[1] = sub_opcode;
        }
        info = &dd_opcode_table[z80_read8(cpu, pc + 1)]; // Use the sub-opcode
        break;
    }
    case 0xFD: // IY prefix
    {
        if (prefix_size >= 2)
        {
            prefix_bytes[0] = opcode;
            uint8_t sub_opcode = z80_read8(cpu, pc + 1);
            prefix_bytes[1] = sub_opcode;
        }
        info = &fd_opcode_table[z80_read8(cpu, pc + 1)]; // Use the sub-opcode
        break;
    }

    default: // Standard instructions
        if (prefix_size >= 1)
        {
            prefix_bytes[0] = opcode;
        }
        info = &opcode_table[opcode];
        break;
    }

    *total_length = info->length;
    return info;
}

void z80_disassemble_instruction(struct z80_t *cpu, char *buffer, size_t buffer_size)
{
    uint16_t start_pc = cpu->registers.PC;
    int      total_length;
    char     prefix_bytes[4] = {0}; // Support up to 4 prefix bytes
    
    // Get the appropriate opcode info
    const opcode_info_t *info = get_opcode_info(cpu, start_pc, &total_length, 
                                              prefix_bytes, sizeof(prefix_bytes));
    
    const char *mnemonic = info->name;
    char        formatted_mnemonic[64];
    char        hex_bytes[32] = "";
    
    // Build hex byte representation with fixed width
    for (int i = 0; i < total_length && i < 4; i++) {
        char    byte_str[4];
        uint8_t byte_val = z80_read8(cpu, start_pc + i);
        snprintf(byte_str, sizeof(byte_str), "%02X ", byte_val);
        strcat(hex_bytes, byte_str);
    }
    
    // Remove trailing space and pad to exactly 11 characters for alignment
    if (strlen(hex_bytes) > 0) {
        hex_bytes[strlen(hex_bytes) - 1] = '\0'; // Remove trailing space
    }
    
    // Format hex bytes with consistent width (11 chars total: "XX XX XX XX")
    char aligned_hex[16];
    snprintf(aligned_hex, sizeof(aligned_hex), "%-11s", hex_bytes);
    
    // Format the instruction based on length and operand requirements
    switch (total_length) {
        case 1: {
            snprintf(formatted_mnemonic, sizeof(formatted_mnemonic), "%s", mnemonic);
            break;
        }
        
        case 2: {
            if (prefix_bytes[0] == 0xED || prefix_bytes[0] == 0xCB) {
                // For 2-byte prefixed instructions, the mnemonic is already complete
                snprintf(formatted_mnemonic, sizeof(formatted_mnemonic), "%s", mnemonic);
            } else {
                // Standard 2-byte instruction with 8-bit operand
                uint8_t operand = z80_read8(cpu, start_pc + 1);
                snprintf(formatted_mnemonic, sizeof(formatted_mnemonic), mnemonic, operand);
            }
            break;
        }
        
        case 3: {
            if (prefix_bytes[0] == 0xED) {
                // 3-byte ED instruction - mnemonic is complete, no operand formatting needed
                snprintf(formatted_mnemonic, sizeof(formatted_mnemonic), "%s", mnemonic);
            } else {
                // Standard 3-byte instruction with 16-bit operand
                uint16_t operand = z80_read16(cpu, start_pc + 1);
                snprintf(formatted_mnemonic, sizeof(formatted_mnemonic), mnemonic, operand);
            }
            break;
        }
        
        case 4: {
            if (prefix_bytes[0] == 0xED) {
                // 4-byte ED prefixed instruction with 16-bit operand
                // The operand is at bytes 2-3 (after ED prefix and opcode)
                uint16_t operand = z80_read16(cpu, start_pc + 2);
                snprintf(formatted_mnemonic, sizeof(formatted_mnemonic), mnemonic, operand);
            } else {
                // Other 4-byte instructions (if any)
                uint16_t operand = z80_read16(cpu, start_pc + 2);
                snprintf(formatted_mnemonic, sizeof(formatted_mnemonic), mnemonic, operand);
            }
            break;
        }
        
        default: {
            snprintf(formatted_mnemonic, sizeof(formatted_mnemonic), "UNKNOWN");
            break;
        }
    }
    
    // Final output formatting with proper alignment
    snprintf(buffer, buffer_size, "PC: %04X: %s %s", start_pc, aligned_hex, formatted_mnemonic);
}

// Helper function for debugging - shows which table was used
void z80_disassemble_instruction_verbose(struct z80_t *cpu, char *buffer, size_t buffer_size)
{
    uint16_t start_pc = cpu->registers.PC;
    uint8_t first_opcode = z80_read8(cpu, start_pc);
    const char *table_name = "MAIN";

    if (first_opcode == 0xED)
    {
        table_name = "ED";
    }
    else if (first_opcode == 0xCB)
    {
        table_name = "CB";
    }
    else if (first_opcode == 0xDD)
    {
        table_name = "DD(IX)";
    }
    else if (first_opcode == 0xFD)
    {
        table_name = "FD(IY)";
    }

    // Get regular disassembly
    z80_disassemble_instruction(cpu, buffer, buffer_size);

    // Append table info
    size_t len = strlen(buffer);
    snprintf(buffer + len, buffer_size - len, " [%s]", table_name);
}