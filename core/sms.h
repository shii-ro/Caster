#ifndef SMS_H_
#define SMS_H_

#include "../cpu/z80.h"
#include "../vdp/vdp.h"
#include "../mmu/mmu.h"
#include "../gui/gui.h"
#include "SDL3/SDL.h"

#define SMS_MASTER_CLOCK_HZ       53693100u   // 53.6931 MHz
#define SMS_SYSTEM_CLOCK_HZ       (SMS_MASTER_CLOCK_HZ / 15)  // ≈ 3579540 Hz
#define NTSC_CHROMA_SUBCARRIER_HZ   3579545u    // 315 / 88 MHz (~3.579545 MHz)

// SMS I/O Port definitions
typedef enum
{
    // VDP (Video Display Processor) Ports
    SMS_PORT_VDP_V_COUNTER = 0x7E, // VDP V Counter (read-only)
    SMS_PORT_VDP_H_COUNTER = 0x7F, // VDP H Counter (read-only)
    SMS_PORT_VDP_DATA      = 0xBE, // VDP Data Port (read/write)
    SMS_PORT_VDP_CONTROL   = 0xBF, // VDP Control/Status Port (read/write)

    // Controller/Input Ports
    SMS_PORT_IO_A = 0xDC,    // I/O Port A (Controller 1)
    SMS_PORT_IO_B = 0xDD,    // I/O Port B (Controller 2)

    // PSG (Programmable Sound Generator) Port
    // SMS_PORT_PSG = 0x7F, // PSG Data Port (write-only, same as H Counter)


} sms_port_t;

struct sms_t
{
    struct z80_t cpu;
    struct mmu_t mem;
    struct vdp_t vdp;
    // System state
    bool powered_on;
    bool paused;
    bool rom_loaded;
};

// System functions
SDL_AppResult sms_init(struct sms_t *sms);
struct sms_t *sms_create(struct sms_t *sms);
void sms_destroy(struct sms_t *sms);
bool sms_load_rom(struct sms_t *sms, const uint8_t *rom_data, size_t size);
bool sms_load_rom_file(struct sms_t *sms, const char *filename);
void sms_reset(struct sms_t *sms);
void sms_power_on(struct sms_t *sms);
void sms_power_off(struct sms_t *sms);
uint8_t sms_port_read(struct sms_t *sms, uint8_t port);
void sms_port_write(struct sms_t *sms, uint8_t port, uint8_t value);
void sms_run_frame(struct sms_t *sms);
#endif