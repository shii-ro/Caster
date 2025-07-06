#include <stdint.h>
#include <stdio.h>
#include <stdint.h>
#include "core/sms.h"
#include "cpu/z80_test.h"
#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#define BUFFER_SIZE 16384
static uint8_t buffer[BUFFER_SIZE] = {0};
static char disasm_buffer[128];

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

struct sms_core_t sms;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char* argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    if (SDL_Init(SDL_INIT_VIDEO) == false)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Couldn't initialize SDL!", SDL_GetError(), NULL);
        return SDL_APP_FAILURE;
    }

    // 800x450 is 16:9
    if (SDL_CreateWindowAndRenderer("Master System", 800, 450, 0, &window, &renderer) == false)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Couldn't create window/renderer!", SDL_GetError(), NULL);
        return SDL_APP_FAILURE;
    }

    const char *filename = argv[1];

    sms_create(&sms);
    sms_load_rom_file(&sms, filename);

    SDL_Log("I HATE NIGGERS\n");
    fflush(stdout);

    return SDL_APP_CONTINUE;
}

// This function runs when a new event occurs
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    switch (event->type)
    {
    case SDL_EVENT_QUIT:
        // end the program, reporting success to the OS
        return SDL_APP_SUCCESS;
    case SDL_EVENT_KEY_DOWN:
        if (event->key.key == SDLK_ESCAPE)
        {
            // end the program on ESC key,
            // returning success to the OS
            return SDL_APP_SUCCESS;
        }
    default:
        break;
    }

    // return continue to continue
    return SDL_APP_CONTINUE;
}

// This function runs once per frame, and is the heart of the program
SDL_AppResult SDL_AppIterate(void *appstate)
{
    int instruction_count = 0;
    sms.cpu.debug = false;

    while (sms.cpu.running && !sms.cpu.halted && instruction_count < 1000)
    {
        if(sms.cpu.debug)
        {
            z80_disassemble_instruction(&sms.cpu, disasm_buffer, sizeof(disasm_buffer));
            printf("%s\t", disasm_buffer);
            z80_print_state(&sms.cpu);
        }

        z80_step(&sms.cpu);
        instruction_count++;
    }
    // z80_print_state(&sms.cpu);
    // SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255);
    // SDL_RenderClear(renderer);
    // SDL_RenderPresent(renderer);

    // return continue to continue
    return SDL_APP_CONTINUE;
}

// This function runs once at shutdown
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    // SDL will clean up the window/renderer for us.
}