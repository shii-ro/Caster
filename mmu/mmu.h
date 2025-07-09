#ifndef MMU_H_
#define MMU_H_

#include <stdint.h>
#include "../cpu/z80.h"

#define MMU_MEMORY_CONTROL_REGISTER 0xFFFC

#define MMU_ROM_SLOT_0_SIZE 0x4000
#define MMU_ROM_SLOT_1_SIZE 0x4000
#define MMU_RAM_SLOT_SIZE   0x4000
#define MMU_RAM_SIZE        0x2000

struct mmu_t
{
    uint8_t *cartridge;
    uint8_t cartridge_ram[16 * 1024];
    size_t cartridge_size;
    union
    {
        uint8_t memory[65535]; 
        struct
        {
            uint8_t rom_slot_0[MMU_ROM_SLOT_0_SIZE];
            uint8_t rom_slot_1[MMU_ROM_SLOT_1_SIZE];
            uint8_t ram_slot[MMU_RAM_SLOT_SIZE];
            uint8_t system_ram[MMU_RAM_SIZE];
        };
    };
    uint8_t page_registers[3];
    uint8_t control_register;
    uint8_t cartridge_ram_page;
    uint8_t cartridge_ram_enabled;
};

void mmu_init(struct mmu_t *mem);
void mmu_deinit(struct mmu_t *mem);
void mmu_load_rom(struct mmu_t *mem, const uint8_t *data, size_t len);
uint8_t mmu_read8(struct mmu_t *mem, uint16_t addr);
uint16_t mmu_read16(struct mmu_t *mem, uint16_t addr);
void mmu_write8(struct mmu_t *mem, uint16_t addr, uint8_t data);
void mmu_write16(struct mmu_t *mem, uint16_t addr, uint16_t data);
uint8_t mmu_port_read(struct mmu_t *mem, uint8_t port);
void mmu_port_write(struct mmu_t *mem, uint8_t port, uint8_t value);

#endif