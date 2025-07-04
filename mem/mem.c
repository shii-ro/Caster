#include "mem.h"
#include <string.h>
#define SMS_MEM_SIZE 0x10000  // 64KB

void sms_mem_init(struct sms_mem_t *mem)
{
    memset(mem->memory, 0, SMS_MEM_SIZE);
}

uint8_t sms_mem_read8(struct sms_mem_t *mem, uint16_t addr)
{
    return mem->memory[addr];
}

uint16_t sms_mem_read16(struct sms_mem_t *mem, uint16_t addr)
{
    if (addr + 1 >= SMS_MEM_SIZE)
        return 0; // prevent out-of-bounds
    return (mem->memory[addr + 1] << 8) | mem->memory[addr];
}

void sms_mem_write8(struct sms_mem_t *mem, uint16_t addr, uint8_t data)
{
    mem->memory[addr] = data;
}

void sms_mem_write16(struct sms_mem_t *mem, uint16_t addr, uint16_t data)
{
    if (addr + 1 >= SMS_MEM_SIZE)
        return;
    mem->memory[addr] = data & 0xFF;
    mem->memory[addr + 1] = data >> 8;
}
