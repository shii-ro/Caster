#ifndef VDP_H_
#define VDP_H_

#include <stdint.h>
#include <stdbool.h>

// VDP Register definitions
#define VDP_REGISTER_COUNT 16
#define VDP_SCANLINES_NTSC      262
#define VDP_SCANLINES_PAL       313

typedef enum {
    /**
     * @brief Register $00: Mode Control Register #1
     * Controls various display modes, scrolling locks, and interrupt enables.
     */
    VDP_REG_MODE_CONTROL_1 = 0x00,

    /**
     * @brief Register $01: Mode Control Register #2
     * Controls display enable, sprite size, and VRAM size configuration.
     */
    VDP_REG_MODE_CONTROL_2 = 0x01,

    /**
     * @brief Register $02: Name Table Base Address
     * Sets the base address in VRAM for the background tile map (the name table).
     */
    VDP_REG_NAME_TABLE_BASE = 0x02,

    /**
     * @brief Register $03: Color Table Base Address
     * Unused on Master System and Game Gear.
     */
    VDP_REG_COLOR_TABLE_BASE = 0x03,

    /**
     * @brief Register $04: Pattern Generator Table Base Address
     * Unused on Master System and Game Gear.
     */
    VDP_REG_PATTERN_GENERATOR_BASE = 0x04,

    /**
     * @brief Register $05: Sprite Attribute Table Base Address
     * Sets the base address in VRAM for the sprite attribute table.
     */
    VDP_REG_SPRITE_ATTRIBUTE_TABLE_BASE = 0x05,

    /**
     * @brief Register $06: Sprite Pattern Generator Table Base Address
     * Sets the base address in VRAM for sprite tile patterns.
     */
    VDP_REG_SPRITE_PATTERN_GENERATOR_BASE = 0x06,

    /**
     * @brief Register $07: Backdrop Color & Text Color
     * Sets the backdrop color (palette index 0-15) and text color for TMS9918 modes.
     */
    VDP_REG_BACKDROP_COLOR = 0x07,

    /**
     * @brief Register $08: Horizontal Scroll Register
     * Sets the fine horizontal scroll value for the background layer.
     */
    VDP_REG_HORIZONTAL_SCROLL = 0x08,

    /**
     * @brief Register $09: Vertical Scroll Register
     * Sets the vertical scroll value for the background layer.
     */
    VDP_REG_VERTICAL_SCROLL = 0x09,

    /**
     * @brief Register $0A: Line Interrupt Counter
     * Sets the scanline at which a line interrupt will be triggered.
     */
    VDP_REG_LINE_INTERRUPT_COUNTER = 0x0A

} vdp_register_t;

struct vdp_t
{
    union
    {
        uint8_t vram[0x4000];
        struct
        {
            uint8_t gfx_tile_255[0x2000];   // $0000-$1FFF: Graphics for tiles 0-255
            uint8_t gfx_tile_447[0x1800];   // $2000-$37FF: Graphics for tiles 256-447
            union
            {
                uint8_t tilemap[0x700]; // $3800-$3EFF: Tilemap
                struct
                {
                    uint8_t tile_index;
                    uint8_t tile_info;
                } tile_data[0x380];
            };
            uint8_t sat[0x100];             // $3F00-$3FFF: Sprite Attribute Table
        };
    };
    uint8_t registers[VDP_REGISTER_COUNT];
    uint8_t cram[64];

    // Register 0 - Mode Control 1
    bool vertical_scroll_lock;
    bool horizontal_scroll_lock;
    bool hide_left_column;
    bool line_interrupt_enable;
    bool shift_sprites_left;
    bool mode4_enable;
    bool extra_height_enable;
    bool sync_disable;

    // Register 1 - Mode Control 2
    bool display_enable;
    bool frame_interrupt_enable;
    bool mode1_enable;
    bool mode3_enable;
    bool sprite_shift;
    bool sprite_size;               // Mode 4: Large (tiled, 8x16) sprites, TMS99918a: 	Large (16x16) sprites
    bool sprite_doubled;
 
    uint8_t name_table_base;        // Register 2 - Name Table Base Address
    uint8_t color_table_base;       // Register 3 - Color Table Base Address (not used in Mode 4)
    uint8_t pattern_gen_base;       // Register 4 - Pattern Generator Base Address (not used in Mode 4)
    uint8_t sprite_attr_base;       // Register 5 - Sprite Attribute Table Base Address
    uint8_t sprite_pattern_base;    // Register 6 - Sprite Pattern Generator Base Address
    uint8_t backdrop_color;         // Register 7 - Overscan/Backdrop Color
    uint8_t bg_x_scroll;            // Register 8 - Background X Scroll
    uint8_t bg_y_scroll;            // Register 9 - Background Y Scroll
    uint8_t line_counter_reload_value;           // Register 10 - Line Counter

    // Control Port
    // Control port state
    bool second_write;           // Command/address latch flag
    uint16_t address;           // Current VRAM/CRAM address
    uint8_t code;               // Current operation code
    uint16_t control_word;
    uint8_t read_buffer;        // Read buffer for VRAM reads

    uint16_t v_counter;
    uint16_t h_counter;
    uint8_t line_counter;

    // Status
    uint8_t vblank_flag;
    uint8_t sprite_overflow_flag;
    uint8_t sprite_collision_flag;
    uint8_t fifth_sprite;
    uint8_t status_flag;
    
    uint64_t total_cycles;

    bool irq_pending;
};

void vdp_init(struct vdp_t *vdp);
void vdp_step(struct vdp_t *vdp, uint8_t cycles);
void vdp_write_register(struct vdp_t *vdp, vdp_register_t r, uint8_t value);
uint8_t vdp_port_read(struct vdp_t *vdp, uint8_t port);
void vdp_port_write(struct vdp_t *vdp, uint8_t port, uint8_t value);
void vdp_process_scanline(struct vdp_t *vdp, uint32_t *framebuffer);
void vdp_control_port_write(struct vdp_t *vdp, uint8_t value);
uint8_t vdp_control_port_read(struct vdp_t *vdp);
void vdp_data_port_write(struct vdp_t *vdp, uint8_t value);
uint8_t vdp_data_port_read(struct vdp_t *vdp);
bool vdp_interrupt_pending(struct vdp_t *vdp);

#endif