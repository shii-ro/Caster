#include "gui_menubar.h"
#include <stdio.h>
#include <string.h>
#include "nuklear.h"
#include "sms.h"

static const SDL_DialogFileFilter filters[] =
{
    { "Sega Master System ROMs", "sms" },
    { "All Files", "*" }
};

static void on_rom_file_selected(void *userdata, const char * const *filelist, int filter)
{
    struct sms_t *sms = (struct sms_t *)userdata;

    // Check if the file list is valid and contains at least one file path.
    if (filelist && filelist[0])
    {
        const char* path = filelist[0];
        // Attempt to load the ROM file using the provided path.
        if (!sms_load_rom_file(sms, path))
        {
            // On failure, show a native SDL error message.
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                                     "ROM Load Error",
                                     "Failed to load the selected ROM file.",
                                     NULL); // Parent window (can be NULL)
        }
    }
    // If filelist is NULL, the user cancelled the dialog, so we do nothing.
}

void gui_render_menubar(struct nk_context *ctx, SDL_Window *win, struct sms_t *sms)
{
    int window_width = 0;
    int window_height = 0;

    SDL_GetWindowSize(win, &window_width, &window_height);

    if (nk_begin(ctx, "Menubar", nk_rect(0, 0, window_width, 30), NK_WINDOW_BACKGROUND | NK_WINDOW_NO_SCROLLBAR))
    {
        nk_menubar_begin(ctx);
        nk_layout_row_begin(ctx, NK_STATIC, 25, 1);
        nk_layout_row_push(ctx, 45);
        if(nk_menu_begin_label(ctx, "File", NK_TEXT_LEFT, nk_vec2(120, 200)))
        {
            nk_layout_row_dynamic(ctx, 25, 1);
            if(nk_menu_item_label(ctx, "Load ROM...", NK_TEXT_LEFT))
            {
                SDL_ShowOpenFileDialog
                (
                    on_rom_file_selected,
                    sms,
                    win,
                    filters,
                    SDL_arraysize(filters),
                    NULL,
                    false
                );
            }
            if(nk_menu_item_label(ctx, "Load Recent", NK_TEXT_LEFT))
            {

            }
            nk_menu_end(ctx);
        }
        nk_menubar_end(ctx);
        nk_end(ctx);
    }
}
