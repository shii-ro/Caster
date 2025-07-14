#include "gui_styles.h"
#include "nuklear.h"

struct nk_color default_bg_color = {.a = 0x00, .r = 0x00, .g = 0x00, .b = 0x00};
struct nk_color default_text_color = {.a = 0xFF, .r = 0xFF, .g = 0xFF, .b = 0xFF};
struct nk_color register_text_color = {.a = 0xFF, .r = 0x32, .g = 0xA8, .b = 0xA4};
struct nk_color shadow_register_text_color = {.a = 0xFF, .r = 0xFF, .g = 0xFF, .b = 0x00};
struct nk_color register_value_color = {.a = 0xFF, .r = 0x00, .g = 0xFF, .b = 0x00};