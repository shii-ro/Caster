#ifndef INPUT_H_
#define INPUT_H_

#include <SDL3/SDL.h>
#include "../core/sms.h"

// Input system initialization and cleanup
void input_init(void);
void input_cleanup(void);
void input_handle_event(struct sms_t *sms, SDL_Keycode key, bool pressed);
uint8_t input_port_read(struct sms_t *sms, uint8_t port);

// // Input mapping functions
// void input_map_controller_buttons(struct sms_t *sms);
// void input_update_controller_state(struct sms_t *sms);

#endif