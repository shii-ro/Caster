#ifndef BIT_UTILS_H
#define BIT_UTILS_H_

#include <stdint.h>

void byte_to_binary(uint8_t byte, char *binary_str);
void byte_to_nibbles(uint8_t byte, char *high_nibble, char *low_nibble);
void nibble_to_binary(uint8_t nibble, char *binary_str);

uint8_t set_bit(uint8_t byte, uint8_t pos);
uint8_t clear_bit(uint8_t byte, uint8_t pos);
uint8_t get_bit(uint8_t byte, uint8_t pos);

#endif