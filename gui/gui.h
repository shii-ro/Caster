#ifndef GUI_H_
#define GUI_H_

#include <stdbool.h>
#include <stdint.h>
#include <SDL3/SDL.h>

struct gui_state 
{
    bool show_debugger;
    bool show_memory_viewer;
    bool show_vdp_viewer;
    bool show_nametable;
    bool emulator_running;
    bool emulator_paused;
    char rom_path[512];
    
    // Debug info
    uint16_t pc;
    uint16_t sp;
    uint8_t a, b, c, d, e, h, l;
    uint16_t af, bc, de, hl;
    uint8_t flags;
    
    // Memory viewer
    uint16_t memory_address;
    uint8_t memory_data[256];
    
    // VDP viewer
    bool vdp_registers_open;
    bool vdp_vram_open;
    bool vdp_cram_open;
};

struct sms_t;

bool gui_init(SDL_Window *window, SDL_Renderer *renderer);
void gui_cleanup();
void gui_handle_event(SDL_Event *event);
// void gui_update(, struct sms_t *sms);
void gui_render(struct sms_t *sms, SDL_Renderer *renderer, SDL_Texture *emulator_texture);
void gui_handle_grab();
void gui_input_begin();
void gui_input_end();
#endif