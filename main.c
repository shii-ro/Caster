#include <stdint.h>
#include <stdio.h>
#include <stdint.h>
#include "core/sms.h"
#include "cpu/z80_test.h"
#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

struct sms_t sms;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char* argv[])
{
    sms_init();
    if (argc < 2)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Usage: %s <filename>\n", argv[0], NULL);
        return SDL_APP_FAILURE;
    }

    const char *filename = argv[1];

    sms_create(&sms);
    sms_load_rom_file(&sms, filename);

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
        else if(event->key.key == SDLK_D)
        {
            sms.cpu.debug = !sms.cpu.debug;
            return SDL_APP_CONTINUE;
        }
    
    default:
        break;
    }

    // return continue to continue
    return SDL_APP_CONTINUE;
}

// This function runs once per frame, and is the heart of the program
SDL_AppResult SDL_AppIterate(void *app_state)
{
    sms_run_frame(&sms);
    return SDL_APP_CONTINUE;
}

// This function runs once at shutdown
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    sms_destroy(&sms);
}