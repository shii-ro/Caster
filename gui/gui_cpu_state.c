#include <stdio.h>
#include <string.h>
#include "nuklear.h"
#include "z80.h"
#include "gui_cpu_state.h"
#include "gui_styles.h"
#include "bit_utils.h"

static const char z80_flag_char_lut[] = {'C', 'N', 'P', 'X', 'H', 'Y', 'Z', 'S'};

static void u8_to_hex_str(uint8_t value, char *buffer)
{
    buffer[0] = '$';
    const char hex_digits[] = "0123456789ABCDEF";
    buffer[1] = hex_digits[(value >> 4) & 0xF];
    buffer[2] = hex_digits[value & 0xF];
    buffer[3] = '\0';
}

static void u16_to_hex_str(uint16_t value, char *buffer)
{
    buffer[0] = '0';
    buffer[1] = 'x';
    const char hex_digits[] = "0123456789ABCDEF";
    buffer[2] = hex_digits[(value >> 12) & 0xF];
    buffer[3] = hex_digits[(value >> 8) & 0xF];
    buffer[4] = hex_digits[(value >> 4) & 0xF];
    buffer[5] = hex_digits[value & 0xF];
    buffer[6] = '\0';
}

// --- Nuklear Drawing Functions ---

// Draw a single 8-bit register in its own group
void draw_single_register(struct nk_context *ctx, const char *name, uint8_t value)
{
    const float row_height = 10.0f; // Adjusted for better visual balance

    char group_name[32]; // Max "RegisterName_reg\0" e.g., "A_reg", "F_reg"
    strcpy(group_name, name);
    strcat(group_name, "_reg");

    if (nk_group_begin(ctx, group_name, NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR))
    {
        // Row 1: Register name and hex value
        float ratio[] = {0.5f, 0.5f}; // Name | Hex
        nk_layout_row(ctx, NK_DYNAMIC, row_height, 2, ratio);

        ctx->style.text.color = register_text_color;
        nk_label(ctx, name, NK_TEXT_ALIGN_CENTERED);

        char hex_str[4]; // "$XX\0"
        u8_to_hex_str(value, hex_str);

        ctx->style.text.color = register_value_color;
        nk_label(ctx, hex_str, NK_TEXT_ALIGN_CENTERED);

        // Row 2: Binary nibbles
        nk_layout_row(ctx, NK_DYNAMIC, row_height, 2, ratio); // Use 2 columns for nibbles

        char high_nibble[5], low_nibble[5];              // "0000\0"
        byte_to_nibbles(value, high_nibble, low_nibble); // Assuming this function fills char arrays

        ctx->style.text.color = default_text_color;
        nk_label(ctx, high_nibble, NK_TEXT_ALIGN_RIGHT); // Align right for high nibble
        nk_label(ctx, low_nibble, NK_TEXT_ALIGN_LEFT);   // Align left for low nibble (creates "0000 0000" effect)

        nk_group_end(ctx);
    }
}

void draw_single_shadow_register(struct nk_context *ctx, const char *name, uint8_t value)
{
    const float row_height = 10.0f; // Adjusted for better visual balance

    char group_name[32]; // Max "RegisterName_reg\0" e.g., "A_reg", "F_reg"
    strcpy(group_name, name);
    strcat(group_name, "_reg");

    if (nk_group_begin(ctx, group_name, NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR))
    {

        // Row 1: Register name and hex value
        float ratio[] = {0.5f, 0.5f}; // Name | Hex
        nk_layout_row(ctx, NK_DYNAMIC, row_height, 2, ratio);

        ctx->style.text.color = shadow_register_text_color;
        nk_label(ctx, name, NK_TEXT_ALIGN_CENTERED);

        char hex_str[4]; // "$XX\0"
        u8_to_hex_str(value, hex_str);

        ctx->style.text.color = register_value_color;
        nk_label(ctx, hex_str, NK_TEXT_ALIGN_CENTERED);

        // Row 2: Binary nibbles
        nk_layout_row(ctx, NK_DYNAMIC, row_height, 2, ratio); // Use 2 columns for nibbles

        char high_nibble[5], low_nibble[5];              // "0000\0"
        byte_to_nibbles(value, high_nibble, low_nibble); // Assuming this function fills char arrays

        ctx->style.text.color = default_text_color;
        nk_label(ctx, high_nibble, NK_TEXT_ALIGN_RIGHT); // Align right for high nibble
        nk_label(ctx, low_nibble, NK_TEXT_ALIGN_LEFT);   // Align left for low nibble (creates "0000 0000" effect)

        nk_group_end(ctx);
    }
}

void draw_single_16bit_register(struct nk_context *ctx, const char *name, uint16_t value)
{
    const float row_height = 10.0f;

    char group_name[32]; // Max "RegisterName_reg\0" e.g., "IX_reg"
    strcpy(group_name, name);
    strcat(group_name, "_reg");

    if (nk_group_begin(ctx, group_name, NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR))
    {
        // Row 1: Register name and hex value
        float ratio[] = {0.5f, 0.5f}; // Name | Hex
        nk_layout_row(ctx, NK_DYNAMIC, row_height, 2, ratio);
        ctx->style.text.color = register_text_color;
        nk_label(ctx, name, NK_TEXT_ALIGN_CENTERED);

        char hex_str[7]; // "0xXXXX\0"
        u16_to_hex_str(value, hex_str);
        ctx->style.text.color = register_value_color;
        nk_label(ctx, hex_str, NK_TEXT_ALIGN_CENTERED);

        // Row 2: All four nibbles (4 columns)
        ctx->style.text.color = default_text_color; // Reset color
        float nibble_ratio[] = {0.25f, 0.25f, 0.25f, 0.25f};
        nk_layout_row(ctx, NK_DYNAMIC, row_height, 4, nibble_ratio);

        // Extract and display each nibble (from MSB to LSB)
        for (int shift = 12; shift >= 0; shift -= 4) {
            uint8_t nibble = (value >> shift) & 0xF;
            char nibble_str[5]; // "XXXX\0"
            nibble_to_binary(nibble, nibble_str);
            nk_label(ctx, nibble_str, NK_TEXT_ALIGN_CENTERED);
        }

        nk_group_end(ctx);
    }
}

void draw_z80_registers(struct nk_context *ctx, struct z80_t *cpu)
{
    // 8-bit registers in pairs layout
    struct
    {
        const char *name1;
        const char *name2;
        uint8_t *reg1;
        uint8_t *reg2;
    } reg_pairs[] = {
        {"A", "F", &cpu->registers.A, &cpu->registers.F},
        {"B", "C", &cpu->registers.B, &cpu->registers.C},
        {"D", "E", &cpu->registers.D, &cpu->registers.E},
        {"H", "L", &cpu->registers.H, &cpu->registers.L}};

    int num_pairs = sizeof(reg_pairs) / sizeof(reg_pairs[0]);

    for (int pair = 0; pair < num_pairs; pair++)
    {
        nk_layout_row_dynamic(ctx, 35.0f, 2); // Adjusted based on `draw_single_register` calc.

        draw_single_register(ctx, reg_pairs[pair].name1, *reg_pairs[pair].reg1);
        draw_single_register(ctx, reg_pairs[pair].name2, *reg_pairs[pair].reg2);
    }

    // 8-bit registers in pairs layout
    struct
    {
        const char *name1;
        const char *name2;
        uint8_t *reg1;
        uint8_t *reg2;
    } s_reg_pairs[] = {
        {"A'", "F'", &cpu->registers._A, &cpu->registers._F},
        {"B'", "C'", &cpu->registers._B, &cpu->registers._C},
        {"D'", "E'", &cpu->registers._D, &cpu->registers._E},
        {"H'", "L'", &cpu->registers._H, &cpu->registers._L}};

    for (int pair = 0; pair < num_pairs; pair++)
    {
        nk_layout_row_dynamic(ctx, 35.0f, 2); 

        draw_single_shadow_register(ctx, s_reg_pairs[pair].name1, *s_reg_pairs[pair].reg1);
        draw_single_shadow_register(ctx, s_reg_pairs[pair].name2, *s_reg_pairs[pair].reg2);
    }

    nk_layout_row_dynamic(ctx, 35.0f, 2); 
    draw_single_register(ctx, "I", cpu->registers.I);
    draw_single_register(ctx, "R", cpu->registers.R);

    // 16-bit registers
    struct
    {
        const char *name;
        uint16_t *reg;
    } special_regs[] = {
        {"IX", &cpu->registers.IX},
        {"IY", &cpu->registers.IY},
        {"SP", &cpu->registers.SP},
        {"PC", &cpu->registers.PC}};

    // Draw 16-bit registers in pairs, each register in its own group
    for (int i = 0; i < 4; i += 2)
    {
        // Explicitly set layout for each pair
        nk_layout_row_dynamic(ctx, 35.0f, 1);
        draw_single_16bit_register(ctx, special_regs[i].name, *special_regs[i].reg);

        // Check if we have a second register in this pair
        if (i + 1 < 4)
        {
            draw_single_16bit_register(ctx, special_regs[i + 1].name, *special_regs[i + 1].reg);
        }
    }
}

void gui_render_cpu_state_window(struct nk_context *ctx, struct z80_t *cpu)
{
    if (nk_begin(ctx,
                 "Z80 State",
                 nk_rect(600, 20, 200, 650),
                 NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_TITLE | NK_WINDOW_NO_SCROLLBAR))
    {
        float flags_height = 60.0f;
        nk_layout_row_dynamic(ctx, flags_height, 1);

        if (nk_group_begin(ctx, "flags_group", NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR))
        {
            float row_height = 20.0f;

            // Flag Labels Row
            nk_layout_row_dynamic(ctx, row_height, 8); // 8 columns, dynamic width
            ctx->style.text.color = nk_rgb(0x00, 0xFF, 0x00);

            for (int i = 7; i >= 0; --i)
            {
                nk_label(ctx,
                         (char[]){z80_flag_char_lut[i], '\0'},
                         NK_TEXT_ALIGN_CENTERED);
            }

            ctx->style.text.color = default_text_color;

            // Binary Values Row
            nk_layout_row_dynamic(ctx, row_height, 8);
            for (int i = 7; i >= 0; --i)
            {
                nk_label(ctx,
                         (char[]){'0' + get_bit(cpu->registers.F, i), '\0'},
                         NK_TEXT_ALIGN_CENTERED);
            }

            nk_group_end(ctx);
        }

        // Add some spacing between sections
        nk_layout_row_dynamic(ctx, 1.f, 1);
        nk_spacing(ctx, 1);

        draw_z80_registers(ctx, cpu);
    }
    nk_end(ctx);
}