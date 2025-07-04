#ifndef SMS_H_
#define SMS_H_

#include "../cpu/z80.h"
#include "../mem/mem.h"

struct sms_core_t
{
    struct z80_t cpu;
    struct sms_mem_t mem;
    // sms_memory_t memory;
    // sms_vdp_t vdp;
    // sms_psg_t psg;
    // sms_io_t io;

    // System state
    bool powered_on;
    bool paused;
};

// System functions
struct sms_core_t *sms_create(struct sms_core_t *core);
void sms_destroy(struct sms_core_t *sms);
bool sms_load_rom(struct sms_core_t *sms, const uint8_t *rom_data, size_t size);
bool sms_load_rom_file(struct sms_core_t *sms, const char *filename);
void sms_reset(struct sms_core_t *sms);
void sms_power_on(struct sms_core_t *sms);
void sms_power_off(struct sms_core_t *sms);

#endif