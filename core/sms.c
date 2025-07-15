#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sms.h"

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Texture *texture = NULL;

static uint32_t framebuffer[256 * 192];
static uint32_t nametable_framebuffer[256 * 224];

static const int64_t TARGET_FRAME_TIME_NS = 16666666; // 60 FPS
static const uint16_t SCANLINES_PER_FRAME = 262;
static const uint16_t CYCLES_PER_SCANLINE = 228;

SDL_AppResult sms_init(struct sms_t *sms)
{
    if (SDL_Init(SDL_INIT_VIDEO) == false)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Couldn't initialize SDL!", SDL_GetError(), NULL);
        return SDL_APP_FAILURE;
    }

    if (SDL_CreateWindowAndRenderer("Master System", 1200, 800, SDL_WINDOW_MAXIMIZED, &window, &renderer) == false)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Couldn't create window/renderer!", SDL_GetError(), NULL);
        return SDL_APP_FAILURE;
    }

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 256, 192);
    if (texture == NULL)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Couldn't create surface", SDL_GetError(), NULL);
        return SDL_APP_FAILURE;
    }

    gui_init(window, renderer);

    memset(framebuffer, 0, sizeof(framebuffer));

    return SDL_APP_CONTINUE;
}

struct sms_t *sms_create(struct sms_t *core)
{
    mmu_init(&core->mem);
    z80_init(&core->cpu);
    vdp_init(&core->vdp);

    core->cpu.read8 = (uint8_t (*)(void *, uint16_t))mmu_read8;
    core->cpu.write8 = (void (*)(void *, uint16_t, uint8_t))mmu_write8;
    core->cpu.read16 = (uint16_t (*)(void *, uint16_t))mmu_read16;
    core->cpu.write16 = (void (*)(void *, uint16_t, uint16_t))mmu_write16;
    core->cpu.io_read8 = (uint8_t (*)(void *, uint8_t))sms_port_read;
    core->cpu.io_write8 = (void (*)(void *, uint8_t, uint8_t))sms_port_write;

    core->cpu.memory_ctx = &core->mem;
    core->cpu.io_ctx = core;

    core->rom_loaded = false;
    core->paused = false;

    return core;
}

void sms_destroy(struct sms_t *core)
{
    mmu_deinit(&core->mem);

    if (texture)
    {
        SDL_DestroyTexture(texture);
    }

    if (renderer)
    {
        SDL_DestroyRenderer(renderer);
    }

    if (window)
    {
        SDL_DestroyWindow(window);
    }

    gui_cleanup();
}

bool sms_load_rom(struct sms_t *sms, const uint8_t *rom_data, size_t size)
{
    if (!sms || !rom_data)
    {
        return false;
    }

    mmu_load_rom(&sms->mem, rom_data, size);
    sms->rom_loaded = true;

    printf("ROM loaded successfully: %zu bytes\n", size);
    return true;
}

bool sms_load_rom_file(struct sms_t *sms, const char *filename)
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

    // Get file size more efficiently
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (size <= 0)
    {
        printf("Invalid ROM file size: %ld\n", size);
        fclose(file);
        return false;
    }

    // Allocate buffer
    uint8_t *rom_buffer = malloc((size_t)size);
    if (!rom_buffer)
    {
        printf("Failed to allocate memory for ROM buffer (%ld bytes)\n", size);
        fclose(file);
        return false;
    }

    // Read file in one go
    size_t bytes_read = fread(rom_buffer, 1, (size_t)size, file);
    fclose(file);

    if (bytes_read != (size_t)size)
    {
        printf("Failed to read ROM file completely\n");
        free(rom_buffer);
        return false;
    }

    bool success = sms_load_rom(sms, rom_buffer, bytes_read);
    free(rom_buffer);

    if (success)
    {
        printf("ROM file '%s' loaded successfully\n", filename);
    }

    return success;
}

void sms_reset(struct sms_t *sms)
{
    if (!sms)
        return;

    z80_reset(&sms->cpu);
    sms->paused = false;
}

uint8_t sms_port_read(struct sms_t *sms, uint8_t port)
{
    switch (port & 0xFF)
    {
    case 0x7E:
    case 0x7F:
    case 0xBE:
    case 0xBF:
        return vdp_port_read(&sms->vdp, port);

    default:
        return 0xFF;
    }
}

void sms_port_write(struct sms_t *sms, uint8_t port, uint8_t value)
{
    switch (port & 0xFF)
    {
    case 0xBE:
    case 0xBF:
        vdp_port_write(&sms->vdp, port, value);
        break;

    default:
        break;
    }
}

void sms_run_frame(struct sms_t *sms)
{
    // Early exit if no ROM loaded
    if (!sms->rom_loaded || sms->paused)
    {
        SDL_RenderClear(renderer);
        gui_render(sms, renderer, texture);
        SDL_RenderPresent(renderer);
        SDL_DelayNS(TARGET_FRAME_TIME_NS);
        return;
    }

    SDL_Time start_time;
    SDL_GetCurrentTime(&start_time);

    struct z80_t *cpu = &sms->cpu;
    struct vdp_t *vdp = &sms->vdp;

    for (uint16_t scanline = 0; scanline < SCANLINES_PER_FRAME; scanline++)
    {
        z80_run_cycles(cpu, CYCLES_PER_SCANLINE);
        vdp_process_scanline(vdp, framebuffer);

        if (vdp_interrupt_pending(vdp))
        {
            z80_set_interrupt_line(cpu, true);
        }
    }

    // Update texture and render
    SDL_UpdateTexture(texture, NULL, framebuffer, 256 * sizeof(uint32_t));
    SDL_RenderClear(renderer);
    gui_render(sms, renderer, texture);
    SDL_RenderPresent(renderer);

    SDL_Time end_time;
    SDL_GetCurrentTime(&end_time);

    int64_t elapsed_time = end_time - start_time;
    if (elapsed_time < TARGET_FRAME_TIME_NS)
    {
        SDL_DelayNS(TARGET_FRAME_TIME_NS - elapsed_time);
    }
}