#ifndef GUI_CPU_STATE_H_
#define GUI_CPU_STATE_H_

struct z80_t;
struct nk_context;

void gui_render_cpu_state_window(struct nk_context* ctx, struct z80_t *cpu);


#endif