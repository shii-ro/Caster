
#include "bit_utils.h"

void nibble_to_binary(uint8_t nibble, char *binary_str)
{
    for (int i = 3; i >= 0; --i)
        binary_str[3 - i] = (nibble & (1 << i)) ? '1' : '0';
    binary_str[4] = '\0';
}

// Helper function to convert byte to binary string
void byte_to_binary(uint8_t byte, char *binary_str)
{
    for (int i = 7; i >= 0; i--)
    {
        binary_str[7 - i] = (byte & (1 << i)) ? '1' : '0';
    }
    binary_str[8] = '\0';
}

// Helper function to get nibbles as binary strings
void byte_to_nibbles(uint8_t byte, char *high_nibble, char *low_nibble)
{
    // High nibble (bits 7-4)
    for (int i = 3; i >= 0; i--)
    {
        high_nibble[3 - i] = ((byte >> 4) & (1 << i)) ? '1' : '0';
    }
    high_nibble[4] = '\0';

    // Low nibble (bits 3-0)
    for (int i = 3; i >= 0; i--)
    {
        low_nibble[3 - i] = (byte & (1 << i)) ? '1' : '0';
    }
    low_nibble[4] = '\0';
}

// Sets the bit at position `pos` (0–7) to 1
uint8_t set_bit(uint8_t byte, uint8_t pos)
{
    return byte | (1 << pos);
}

// Clears the bit at position `pos` (0–7) to 0
uint8_t clear_bit(uint8_t byte, uint8_t pos)
{
    return byte & ~(1 << pos);
}

// Returns the value (0 or 1) of the bit at position `pos` (0–7)
uint8_t get_bit(uint8_t byte, uint8_t pos)
{
    return (byte >> pos) & 0x01;
}