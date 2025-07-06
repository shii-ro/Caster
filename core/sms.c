#include <stdlib.h>
#include <stdio.h>
#include "sms.h"

struct sms_core_t *sms_create(struct sms_core_t *core)
{
    sms_mem_init(&core->mem);
    z80_init(&core->cpu);

    core->cpu.read8   = (uint8_t (*)(void*, uint16_t)) sms_mem_read8;
    core->cpu.write8  = (void (*)(void*, uint16_t, uint8_t)) sms_mem_write8;
    core->cpu.read16  = (uint16_t (*)(void *, uint16_t)) sms_mem_read16;
    core->cpu.write16 = (void (*)(void*, uint16_t, uint16_t)) sms_mem_write16;
    core->cpu.memory_ctx = &core->mem;

    sms_mem_write8(&core->mem, 0x07, 0xC9);

    return core;
}

bool sms_load_rom(struct sms_core_t *sms, const uint8_t *rom_data, size_t size)
{
    if (!sms || !rom_data)
    {
        return false;
    }

    // SMS ROM typically starts at address 0x0000
    // Check if ROM fits in available space (32KB for SMS)
    const size_t MAX_ROM_SIZE = 32768; // 32KB
    if (size > MAX_ROM_SIZE)
    {
        printf("ROM too large: %zu bytes (max %zu bytes)\n", size, MAX_ROM_SIZE);
        return false;
    }

    // Load ROM into memory starting at address 0x0000
    for (size_t i = 0; i < size; i++)
    {
        sms_mem_write8(&sms->mem, (uint16_t)i + 0x100, rom_data[i]);
    }

    printf("ROM loaded successfully: %zu bytes\n", size);
    return true;
}

bool sms_load_rom_file(struct sms_core_t *sms, const char *filename)
{
    if (!sms || !filename)
    {
        return false;
    }

    FILE *file = fopen(filename, "rb");
    if (!file)
    {
        printf("Failed to open ROM file: %s\n", filename);
        return false;
    }

    // Determine file size
    if (fseek(file, 0, SEEK_END) != 0)
    {
        printf("Failed to seek to end of file\n");
        fclose(file);
        return false;
    }

    long size = ftell(file);
    if (size < 0)
    {
        printf("ftell failed\n");
        fclose(file);
        return false;
    }

    if (fseek(file, 0, SEEK_SET) != 0)
    {
        printf("Failed to seek to beginning of file\n");
        fclose(file);
        return false;
    }

    // Check file size constraints
    const size_t MAX_ROM_SIZE = 32768; // 32KB for SMS
    if (size == 0)
    {
        printf("ROM file is empty\n");
        fclose(file);
        return false;
    }

    if ((size_t)size > MAX_ROM_SIZE)
    {
        printf("ROM file too large: %ld bytes (max %zu bytes)\n", size, MAX_ROM_SIZE);
        fclose(file);
        return false;
    }

    // Allocate temporary buffer for ROM data
    uint8_t *rom_buffer = malloc((size_t)size);
    if (!rom_buffer)
    {
        printf("Failed to allocate memory for ROM buffer (%ld bytes)\n", size);
        fclose(file);
        return false;
    }

    // Read the entire file into buffer
    size_t bytes_read = fread(rom_buffer, 1, (size_t)size, file);
    fclose(file);

    if (bytes_read != (size_t)size)
    {
        printf("Failed to read ROM file: expected %ld bytes, read %zu bytes\n", size, bytes_read);
        free(rom_buffer);
        return false;
    }

    // Load ROM data into SMS memory
    bool success = sms_load_rom(sms, rom_buffer, bytes_read);
    free(rom_buffer);

    if (success)
    {
        printf("ROM file '%s' loaded successfully\n", filename);
    }

    return success;
}