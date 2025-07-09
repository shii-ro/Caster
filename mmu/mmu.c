#include "mmu.h"
#include <string.h>
#include <stdlib.h>

#define SMS_MEM_SIZE 0x10000       // 64KB
#define SMS_PAGE_SIZE 0x4000       // 16KB
#define SMS_SYSTEM_RAM_SIZE 0x2000 // 8KB

static void mmu_load_page(struct mmu_t *mem, uint8_t page, uint8_t slot)
{
    uint32_t cart_offset = page * SMS_PAGE_SIZE;
    uint16_t memory_offset = slot * SMS_PAGE_SIZE;

    // Check if page exists in cartridge
    if (cart_offset >= mem->cartridge_size)
    {
        // Page doesn't exist, fill with 0xFF or handle as needed
        memset(mem->memory + memory_offset, 0xFF, SMS_PAGE_SIZE);
        return;
    }

    if (slot == 0)
    {
        // For slot 0, only load bytes 0x400-0x3FFF (preserve first 1KB)
        uint32_t copy_size = SMS_PAGE_SIZE - 0x400;
        if (cart_offset + 0x400 + copy_size > mem->cartridge_size)
        {
            copy_size = mem->cartridge_size - (cart_offset + 0x400);
        }

        memcpy(mem->memory + memory_offset + 0x400,
               mem->cartridge + cart_offset + 0x400,
               copy_size);
    }
    else
    {
        // For other slots, load the entire page
        uint32_t copy_size = SMS_PAGE_SIZE;
        if (cart_offset + copy_size > mem->cartridge_size)
        {
            copy_size = mem->cartridge_size - cart_offset;
        }

        memcpy(mem->memory + memory_offset,
               mem->cartridge + cart_offset,
               copy_size);
    }
}

void mmu_init(struct mmu_t *mem)
{
    memset(mem->memory, 0, SMS_MEM_SIZE);
    memset(mem->system_ram, 0, SMS_SYSTEM_RAM_SIZE);

    mem->cartridge = NULL;
    mem->cartridge_size = 0;
    mem->control_register = 0;
    mem->cartridge_ram_enabled = false;
    mem->cartridge_ram_page = 0;
    mem->page_registers[0] = 0;
    mem->page_registers[1] = 1;
    mem->page_registers[2] = 2;
}

void mmu_deinit(struct mmu_t *mem)
{
    if (mem->cartridge)
    {
        free(mem->cartridge);
        mem->cartridge = NULL;
    }
}

void mmu_load_rom(struct mmu_t *mem, const uint8_t *data, size_t len)
{
    if (mem->cartridge)
    {
        free(mem->cartridge);
    }

    mem->cartridge = malloc(len);
    memcpy(mem->cartridge, data, len);
    mem->cartridge_size = len;

    // Load first 1KB (fixed area - interrupt vectors, etc.)
    memcpy(mem->memory, mem->cartridge, 0x400);

    // Load the first three pages
    mmu_load_page(mem, 0, 0);
    mmu_load_page(mem, 1, 1);
    mmu_load_page(mem, 2, 2);
}

uint8_t mmu_read8(struct mmu_t *mem, uint16_t addr)
{
    switch (addr & 0xE000)
    {
    case 0x0000: // 0x0000-0x1FFF: ROM Page 0
    case 0x2000: // 0x2000-0x3FFF: ROM Page 0
    case 0x4000: // 0x4000-0x5FFF: ROM Page 1
    case 0x6000: // 0x6000-0x7FFF: ROM Page 1
        return mem->memory[addr];
    case 0x8000: // 0x8000-0x9FFF: ROM Page 2 or Cartridge RAM
    case 0xA000: // 0xA000-0xBFFF: ROM Page 2 or Cartridge RAM
        return mem->memory[addr];

    case 0xC000: // 0xC000-0xDFFF: System RAM
        return mem->system_ram[addr & 0x1FFF];

    case 0xE000: // 0xE000-0xFFFF: Mirror of System RAM
        return mem->system_ram[addr & 0x1FFF];

    default:
        return 0xFF;
    }
}

uint16_t mmu_read16(struct mmu_t *mem, uint16_t addr)
{
    uint8_t low = mmu_read8(mem, addr);
    uint8_t high = mmu_read8(mem, addr + 1);
    return (high << 8) | low;
}

void mmu_write8(struct mmu_t *mem, uint16_t addr, uint8_t data)
{
    switch (addr & 0xE000)
    {
    case 0x0000: // 0x0000-0x1FFF: ROM Page 0 (read-only)
    case 0x2000: // 0x2000-0x3FFF: ROM Page 0 (read-only)
    case 0x4000: // 0x4000-0x5FFF: ROM Page 1 (read-only)
    case 0x6000: // 0x6000-0x7FFF: ROM Page 1 (read-only)
        // ROM is read-only, ignore writes
        return;

    case 0x8000: // 0x8000-0x9FFF: ROM Page 2 or Cartridge RAM
    case 0xA000: // 0xA000-0xBFFF: ROM Page 2 or Cartridge RAM
        if (mem->cartridge_ram_enabled)
        {
            uint16_t ram_offset = (addr & 0x1FFF); // Offset within 8KB window (0x0000-0x1FFF)
            if (mem->cartridge_ram_page == 0)
            { // If cartridge_ram_page selects 0x8000-0x9FFF
                if ((addr & 0xE000) == 0x8000)
                { // Only if address is within 0x8000-0x9FFF
                    mem->cartridge_ram[ram_offset] = data;
                }
                // Else, if ram_slot is 0xA000-0xBFFF, do nothing (it's unmapped RAM or ROM)
            }
            else
            { // If cartridge_ram_page selects 0xA000-0xBFFF
                if ((addr & 0xE000) == 0xA000)
                { // Only if address is within 0xA000-0xBFFF
                    mem->cartridge_ram[ram_offset] = data;
                }
                // Else, if ram_slot is 0x8000-0x9FFF, do nothing
            }
        }
        // If cartridge RAM is not enabled, this is ROM and write is ignored
        return;

    case 0xC000: // 0xC000-0xDFFF: System RAM
        mem->system_ram[addr & 0x1FFF] = data;
        return;

    case 0xE000: // 0xE000-0xFFFF: Mirror of System RAM or control registers
        if (addr >= 0xFFFC && addr <= 0xFFFF)
        {
            // Memory mapper control registers
            switch (addr)
            {
            case 0xFFFC:
                mem->control_register = data;
                mem->cartridge_ram_enabled = (data & 0x08) != 0;
                mem->cartridge_ram_page = (data & 0x04) != 0;
                break;

            case 0xFFFD:
                mem->page_registers[0] = data;
                mmu_load_page(mem, data, 0);
                break;

            case 0xFFFE:
                mem->page_registers[1] = data;
                mmu_load_page(mem, data, 1);
                break;

            case 0xFFFF:
                mem->page_registers[2] = data;
                mmu_load_page(mem, data, 2);
                break;
            }
        }
        else
        {
            // Mirror of system RAM
            mem->system_ram[addr & 0x1FFF] = data;
        }
        return;

    default:
        return;
    }
}

void mmu_write16(struct mmu_t *mem, uint16_t addr, uint16_t data)
{
    mmu_write8(mem, addr, data & 0xFF);            // Low byte
    mmu_write8(mem, addr + 1, (data >> 8) & 0xFF); // High byte
}