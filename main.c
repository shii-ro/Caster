#include <stdint.h>
#include <stdio.h>
#include <stdint.h>
#include "core/sms.h"
#include <SDL3/SDL.h>

struct sms_t sms;

int main(int argc, char* argv[])
{
    // Initialize SDL
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS)) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }

    // Check command line arguments
    if (argc < 2) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Usage", "Usage: program <filename>", NULL);
        SDL_Quit();
        return 1;
    }

    const char *filename = argv[1];

    // Initialize SMS emulator
    sms_init(&sms);
    sms_create(&sms);
    sms_load_rom_file(&sms, filename);

    // Main event loop
    SDL_Event event;
    int running = 1;
    
    while (running) {
        // Handle events
        while (SDL_PollEvent(&event))
        {
            gui_input_begin();
            switch (event.type)
            {
            case SDL_EVENT_QUIT:
                running = 0;
                break;

            case SDL_EVENT_KEY_DOWN:
                if (event.key.key == SDLK_ESCAPE)
                {
                    running = 0;
                }
                else if (event.key.key == SDLK_D)
                {
                    sms.cpu.debug = !sms.cpu.debug;
                }
                break;

            default:
                break;
            }
            gui_handle_event(&event);
        }
        gui_handle_grab();
        gui_input_end();

        sms_run_frame(&sms);
    }

    // Cleanup
    sms_destroy(&sms);
    SDL_Quit();
    
    return 0;
}