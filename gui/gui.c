#include "gui.h"
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_SDL_RENDERER_IMPLEMENTATION
#include "nuklear.h"
#include "nuklear_sdl_renderer.h"
#include "gui_styles.h"
#include "gui_cpu_state.h"
#include "gui_menubar.h"
#include "sms.h"

static struct gui_state gui;
static struct nk_context *ctx;

bool gui_init(SDL_Window *window, SDL_Renderer *renderer)
{
    memset(&gui, 0, sizeof(struct gui_state));

    ctx = nk_sdl_init(window, renderer);
    if (!ctx)
    {
        SDL_Log("Failed to initialize Nuklear context");
        return false;
    }

    // Load default font
    struct nk_font_atlas *atlas;
    nk_sdl_font_stash_begin(&atlas);
    nk_sdl_font_stash_end();

    gui.show_debugger = true;
    gui.show_memory_viewer = false;
    gui.show_vdp_viewer = false;
    gui.show_nametable = true;
    gui.emulator_running = false;
    gui.emulator_paused = false;
    gui.memory_address = 0x0000;

    strcpy(gui.rom_path, "");

    return true;
}

void gui_cleanup()
{
    nk_sdl_shutdown();
}

// Function to get a specific flag bit
int get_flag_bit(unsigned char flags, int bit_pos) {
    return (flags >> bit_pos) & 0x01;
}

void gui_render(struct sms_t *sms, SDL_Renderer *renderer, SDL_Texture *emulator_texture)
{
// Define the exact size for the emulator content
    const float EMU_WIDTH = 256.0f;
    const float EMU_HEIGHT = 192.0f;
    
    // Account for window decorations
    const float TITLE_BAR_HEIGHT = 40.f;  // Typical title bar height
    const float BORDER_WIDTH = 0.f;       // Border padding (left + right)
    const float BORDER_HEIGHT = 0.f;      // Border padding (top + bottom)
    
    // Calculate window size to get desired content size
    const float WINDOW_WIDTH = EMU_WIDTH + BORDER_WIDTH;
    const float WINDOW_HEIGHT = EMU_HEIGHT + TITLE_BAR_HEIGHT + BORDER_HEIGHT;

    /*
     * Create a movable, closable window with calculated size
     * so that the content region is exactly EMU_WIDTH x EMU_HEIGHT
     */
    if (nk_begin(ctx, "Emulator", nk_rect(50, 50, WINDOW_WIDTH, WINDOW_HEIGHT),
                 NK_WINDOW_MOVABLE     |
                 NK_WINDOW_MINIMIZABLE |
                 NK_WINDOW_TITLE       |
                 NK_WINDOW_BORDER |
                 NK_WINDOW_NO_SCROLLBAR))
    {
        /*
         * Use static layout with exact emulator dimensions
         * This ensures the image displays at exactly 256x192
         */
        nk_layout_row_static(ctx, EMU_HEIGHT, (int)EMU_WIDTH, 1);

        // Convert the SDL_Texture to a Nuklear image and draw it
        struct nk_image image = nk_image_ptr(emulator_texture);
        nk_image(ctx, image);
    }
    nk_end(ctx);

    // MenuBar
    gui_render_menubar(ctx, sdl.win, sms);

    gui_render_cpu_state_window(ctx, &sms->cpu);

    // Memory viewer window
    if (gui.show_memory_viewer && nk_begin(ctx, "Memory Viewer", nk_rect(400, 200, 600, 400),
                                            NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE | NK_WINDOW_TITLE))
    {

        nk_layout_row_dynamic(ctx, 25, 2);
        nk_label(ctx, "Address:", NK_TEXT_LEFT);
        nk_property_int(ctx, "#Address", 0, (int *)&gui.memory_address, 0xFFFF, 1, 1);

        nk_layout_row_dynamic(ctx, 300, 1);
        if (nk_group_begin(ctx, "Memory Data", NK_WINDOW_BORDER))
        {
            for (int row = 0; row < 16; row++)
            {
                nk_layout_row_begin(ctx, NK_STATIC, 20, 17);

                // Address column
                nk_layout_row_push(ctx, 60);
                nk_labelf(ctx, NK_TEXT_LEFT, "%04X:", gui.memory_address + (row * 16));

                // Hex data columns
                for (int col = 0; col < 16; col++)
                {
                    nk_layout_row_push(ctx, 30);
                    nk_labelf(ctx, NK_TEXT_CENTERED, "%02X", gui.memory_data[row * 16 + col]);
                }

                nk_layout_row_end(ctx);
            }
            nk_group_end(ctx);
        }
        nk_end(ctx);
    }


    // VDP viewer window
    if (gui.show_vdp_viewer && nk_begin(ctx, "VDP Viewer", nk_rect(100, 300, 400, 300),
                                         NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE | NK_WINDOW_TITLE))
    {

        if (nk_tree_push(ctx, NK_TREE_TAB, "VDP Registers", NK_MAXIMIZED))
        {
            nk_layout_row_dynamic(ctx, 20, 2);
            for (int i = 0; i < 8; i++)
            {
                nk_labelf(ctx, NK_TEXT_LEFT, "R%d: 0x%02X", i, 0); // Replace with actual VDP register reads
            }
            nk_tree_pop(ctx);
        }

        if (nk_tree_push(ctx, NK_TREE_TAB, "VRAM", NK_MINIMIZED))
        {
            nk_layout_row_dynamic(ctx, 25, 1);
            nk_label(ctx, "VRAM Contents (TODO)", NK_TEXT_LEFT);
            nk_tree_pop(ctx);
        }

        if (nk_tree_push(ctx, NK_TREE_TAB, "CRAM", NK_MINIMIZED))
        {
            nk_layout_row_dynamic(ctx, 25, 1);
            nk_label(ctx, "CRAM Contents (TODO)", NK_TEXT_LEFT);
            nk_tree_pop(ctx);
        }
        nk_end(ctx);
    }

    // Render the GUI
    nk_sdl_render(NK_ANTI_ALIASING_ON);
}

void gui_input_begin()
{
    nk_input_begin(ctx);
}

void gui_input_end()
{
    nk_input_end(ctx);
}

void gui_handle_event(SDL_Event *event)
{
    nk_sdl_handle_event(event);
}

void gui_handle_grab()
{
    nk_sdl_handle_grab();
}