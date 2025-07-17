#include "input.h"

typedef enum
{
    SMS_BUTTON_UP = 0,
    SMS_BUTTON_DOWN,
    SMS_BUTTON_LEFT,
    SMS_BUTTON_RIGHT,
    SMS_BUTTON_1,
    SMS_BUTTON_2,
    SMS_BUTTON_START,
    SMS_BUTTON_COUNT
} sms_button_t;

typedef struct
{
    uint8_t button_up;
    uint8_t button_down;
    uint8_t button_left;
    uint8_t button_right;
    uint8_t button_1;
    uint8_t button_2;
    uint8_t button_start;
} sms_controller_t;

typedef struct
{
    SDL_Keycode key;
    sms_button_t button;
} key_mapping_t;

static const key_mapping_t default_key_map[] = 
{
    {SDLK_UP, SMS_BUTTON_UP},
    {SDLK_DOWN, SMS_BUTTON_DOWN},
    {SDLK_LEFT, SMS_BUTTON_LEFT},
    {SDLK_RIGHT, SMS_BUTTON_RIGHT},
    {SDLK_Z, SMS_BUTTON_1},          
    {SDLK_X, SMS_BUTTON_2},          
    {SDLK_RETURN, SMS_BUTTON_START}, 
};

static const size_t key_map_size = sizeof(default_key_map) / sizeof(default_key_map[0]);
static uint8_t button_states[SMS_BUTTON_COUNT] = {0};
static sms_controller_t controller[2] = {0xFF};

void input_init(void)
{
    for (int i = 0; i < SMS_BUTTON_COUNT; i++)
    {
        button_states[i] = 1;
    }
}

void input_handle_event(struct sms_t *sms, SDL_Keycode key, bool pressed)
{
    for (size_t i = 0; i < key_map_size; i++)
    {
        if (default_key_map[i].key == key)
        {
            button_states[default_key_map[i].button] = pressed ? 1 : 0;
            break;
        }
    }

    controller[0].button_up = !button_states[SMS_BUTTON_UP];
    controller[0].button_down = !button_states[SMS_BUTTON_DOWN];
    controller[0].button_left = !button_states[SMS_BUTTON_LEFT];
    controller[0].button_right = !button_states[SMS_BUTTON_RIGHT];
    controller[0].button_1 = !button_states[SMS_BUTTON_1];
    controller[0].button_2 = !button_states[SMS_BUTTON_2];
    controller[0].button_start = !button_states[SMS_BUTTON_START];
}

uint8_t input_port_read(struct sms_t *sms, uint8_t port)
{
    switch (port)
    {
    case SMS_PORT_IO_A:                           // 0xDE - Controller 1 + Controller 2 fire buttons
        return (controller[1].button_2 << 7 |     // Controller 2 Button 2
                controller[1].button_1 << 6 |     // Controller 2 Button 1
                controller[0].button_2 << 5 |     // Controller 1 Button 2
                controller[0].button_1 << 4 |     // Controller 1 Button 1
                controller[0].button_right << 3 | // Controller 1 Right
                controller[0].button_left << 2 |  // Controller 1 Left
                controller[0].button_down << 1 |  // Controller 1 Down
                controller[0].button_up << 0);    // Controller 1 Up

    case SMS_PORT_IO_B:                           // 0xDD - Controller 2 directions + system bits
        return (0x1 << 7 |                        
                controller[0].button_start << 6 | // Pause button
                0x1 << 5 |                        
                0x1 << 4 |                        // Reset bit
                controller[1].button_right << 3 | // Controller 2 Right
                controller[1].button_left << 2 |  // Controller 2 Left
                controller[1].button_down << 1 |  // Controller 2 Down
                controller[1].button_up << 0);    // Controller 2 Up

    default:
        return 0xFF;
    }
}