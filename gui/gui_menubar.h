#ifndef GUI_MENUBAR_H
#define GUI_MENU_H

#include <stdbool.h>
#include "SDL3/SDL.h"

struct sms_t;
struct nk_context;

void gui_render_menubar(struct nk_context *ctx, SDL_Window *win, struct sms_t *sms);
bool gui_is_paused(void);

#endif // GUI_MENU_H
