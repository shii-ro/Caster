#include <stdlib.h>
#include <stdio.h>
#include "sms.h"

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Texture *texture = NULL;

static SDL_Window *nametable_window = NULL;
static SDL_Renderer *nametable_renderer = NULL;
static SDL_Texture *nametable_texture = NULL;

SDL_AppResult sms_init()
{
    if (SDL_Init(SDL_INIT_VIDEO) == false)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Couldn't initialize SDL!", SDL_GetError(), NULL);
        return SDL_APP_FAILURE;
    }

    // 800x450 is 16:9
    if (SDL_CreateWindowAndRenderer("Master System", 256 * 3, 192 * 3, 0, &window, &renderer) == false)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Couldn't create window/renderer!", SDL_GetError(), NULL);
        return SDL_APP_FAILURE;
    }

    if (SDL_CreateWindowAndRenderer("Master System : Nametable", 256, 224, 0, &nametable_window, &nametable_renderer) == false)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Couldn't create window/renderer!", SDL_GetError(), NULL);
        return SDL_APP_FAILURE;
    }

    // SDL_SetRenderVSync(renderer, 1);

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 256, 192);
    if(texture == NULL)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Couldn't create surface", SDL_GetError(), NULL);
        return SDL_APP_FAILURE;
    }

    nametable_texture = SDL_CreateTexture(nametable_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 256, 224);
    if(nametable_texture == NULL)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Couldn't create surface", SDL_GetError(), NULL);
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

struct sms_t *sms_create(struct sms_t *core)
{
    mmu_init(&core->mem);
    z80_init(&core->cpu);
    vdp_init(&core->vdp);

    core->cpu.read8   = (uint8_t (*)(void*, uint16_t)) mmu_read8;
    core->cpu.write8  = (void (*)(void*, uint16_t, uint8_t)) mmu_write8;
    core->cpu.read16  = (uint16_t (*)(void *, uint16_t)) mmu_read16;
    core->cpu.write16 = (void (*)(void*, uint16_t, uint16_t)) mmu_write16;
    core->cpu.io_read8 = (uint8_t (*)(void *, uint8_t)) sms_port_read;
    core->cpu.io_write8 = (void (*)(void *, uint8_t, uint8_t)) sms_port_write;

    core->cpu.memory_ctx = &core->mem;
    core->cpu.io_ctx = core;

    // mmu_write8(&core->mem, 0x07, 0xC9);

    return core;
}


void sms_destroy(struct sms_t *core)
{
    mmu_deinit(&core->mem);
}

bool sms_load_rom(struct sms_t *sms, const uint8_t *rom_data, size_t size)
{
    if (!sms || !rom_data)
    {
        return false;
    }

    // Load ROM into memory starting at address 0x0000
    // for (size_t i = 0; i < size; i++)
    // {
    //     mmu_write8(&sms->mem, (uint16_t)i + 0x100, rom_data[i]);
    // }

    mmu_load_rom(&sms->mem, rom_data, size);

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

    if (size == 0)
    {
        printf("ROM file is empty\n");
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

uint8_t sms_port_read(struct sms_t *sms, uint8_t port)
{
    switch (port & 0xFF) 
    {
        case 0x7E:
        case 0x7F:
        case 0xBE:
        case 0xBF:
            return vdp_port_read(&sms->vdp, port);
            
        // case 0x7F & 0xC0: // PSG ports (0x7F with some bits masked)
        //     return psg_read_port(sms->psg, port);
            
        // case 0xDC:
        // case 0xDD:
        // case 0xDE:
        //     return controller_read_port(sms->controller, port);
            
        default:
            return 0xFF;
    }
}

void sms_port_write(struct sms_t *sms, uint8_t port, uint8_t value)
{
    switch (port & 0xFF) {
        case 0xBE:
        case 0xBF:
            vdp_port_write(&sms->vdp, port, value);
            break;
            
        // case 0x7F:
        //     psg_write_port(sms->psg, port, value);
        //     break;
            
        // case 0x3E: // Memory Control (SMS-specific)
        //     sms->memory_control = value;
        //     mmu_set_banking(sms->mmu, value);
        //     break;
            
        default:
            break;
    }
}

void sms_run_frame(struct sms_t *sms)
{
    static uint32_t framebuffer[256 * 192];
    static uint32_t nametable_framebuffer[256 * 224];
    const int64_t target_frame_time_ns = 16666666;  // 60 FPS

    SDL_Time start_time;
    SDL_Time end_time;
    SDL_Time elapsed_time;

    SDL_GetCurrentTime(&start_time);

    for (uint16_t scanline = 0; scanline < 262; scanline++)
    {
        z80_run_cycles(&sms->cpu, 228);
        vdp_process_scanline(&sms->vdp, framebuffer);

        if (vdp_interrupt_pending(&sms->vdp))
        {
            z80_set_interrupt_line(&sms->cpu, true);
        }
    }    

    vdp_draw_nametable(&sms->vdp, nametable_framebuffer);

    SDL_UpdateTexture(texture, NULL, framebuffer, 256 * sizeof(uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

    SDL_UpdateTexture(nametable_texture, NULL, nametable_framebuffer, 256 * sizeof(uint32_t));
    SDL_RenderClear(nametable_renderer);
    SDL_RenderTexture(nametable_renderer, nametable_texture, NULL, NULL);
    SDL_RenderPresent(nametable_renderer);

    SDL_GetCurrentTime(&end_time);

    elapsed_time = end_time - start_time;
    int64_t time_to_sleep = target_frame_time_ns - elapsed_time;

    if (elapsed_time < target_frame_time_ns)
    {
        SDL_DelayNS(target_frame_time_ns - elapsed_time);
    }
}