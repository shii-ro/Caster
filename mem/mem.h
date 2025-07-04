#ifndef MEM_H_
#define MEM_H_

#include <stdint.h>
#include "../cpu/z80.h"

struct sms_mem_t
{
    uint8_t memory[65535];
};

void sms_mem_init(struct sms_mem_t *mem);
uint8_t sms_mem_read8(struct sms_mem_t *mem, uint16_t addr);
uint16_t sms_mem_read16(struct sms_mem_t *mem, uint16_t addr);
void sms_mem_write8(struct sms_mem_t *mem, uint16_t addr, uint8_t data);
void sms_mem_write16(struct sms_mem_t *mem, uint16_t addr, uint16_t data);

#endif