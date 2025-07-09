#include "vdp.h"
#include "SDL3/SDL.h"

// VDP pixel clock = master clock / 2
#define VDP_PIXEL_CLOCK_HZ        (SMS_SYSTEM_CLOCK_HZ / 2) 
#define VDP_NTSC_SCANLINE_INTERRUPT_LINE 192

void vdp_init(struct vdp_t *vdp)
{

}

static uint32_t vdp_get_color(struct vdp_t *vdp, uint8_t color_index)
{
    uint8_t color6 = vdp->cram[color_index & 0x0F]; // Only 16 colors
    
    // Master System color format: BBGGRR (6-bit)
    uint8_t r = (color6 & 0x03) * 85;        // Bits 1-0: Red
    uint8_t g = ((color6 >> 2) & 0x03) * 85; // Bits 3-2: Green  
    uint8_t b = ((color6 >> 4) & 0x03) * 85; // Bits 5-4: Blue

    return (0xFF000000 | (r << 16) | (g << 8) | b);
}

void vdp_step(struct vdp_t *vdp, uint8_t cycles)
{
    vdp->total_cycles += cycles;
}


static void vdp_draw_scanline(struct vdp_t *vdp, uint32_t *framebuffer, uint16_t scanline)
{
    // First, draw the background layer
    
    // Calculate which tile row we're on
    uint8_t scroll_x = vdp->bg_x_scroll;
    uint8_t scroll_y = vdp->bg_y_scroll;
    uint8_t effective_y = (scanline + scroll_y) % 224;  // Wrap around screen height
    uint8_t tile_row = effective_y / 8;
    uint8_t row_in_tile = effective_y % 8;

    // Draw 32 tiles across the screen (256 pixels / 8 pixels per tile)
    for (uint16_t screen_tile_x = 0; screen_tile_x < 32; screen_tile_x++)
    {
        // Calculate which tile column to fetch (with horizontal scrolling)
        uint8_t tile_column = (screen_tile_x + (scroll_x / 8)) % 32;
        
        // Get name table entry (2 bytes per entry)
        uint16_t name_table_base = 0x3800;  // Typical name table address
        uint16_t name_table_offset = (tile_row * 32 + tile_column) * 2;
        uint16_t name_table_address = name_table_base + name_table_offset;
        
        // Read tile data from name table
        uint8_t tile_index = vdp->vram[name_table_address];
        uint8_t tile_attributes = vdp->vram[name_table_address + 1];
        
        // Extract attributes
        uint8_t palette_select = (tile_attributes >> 3) & 1;  // Bit 3: palette selection
        uint8_t priority = (tile_attributes >> 4) & 1;        // Bit 4: priority
        uint8_t flip_x = (tile_attributes >> 1) & 1;          // Bit 1: horizontal flip
        uint8_t flip_y = (tile_attributes >> 2) & 1;          // Bit 2: vertical flip
        
        // Calculate pattern table address
        uint16_t pattern_table_base = 0x0000;  // Typical pattern table address
        uint16_t tile_address = pattern_table_base + (tile_index * 32);
        
        // Handle vertical flip
        uint8_t actual_row = flip_y ? (7 - row_in_tile) : row_in_tile;
        
        // Read the four bitplanes for this row
        uint8_t byte1 = vdp->vram[tile_address + actual_row * 4 + 0];
        uint8_t byte2 = vdp->vram[tile_address + actual_row * 4 + 1];
        uint8_t byte3 = vdp->vram[tile_address + actual_row * 4 + 2];
        uint8_t byte4 = vdp->vram[tile_address + actual_row * 4 + 3];
        
        // Decode 8 pixels from the 4 bitplanes
        for (int pixel = 0; pixel < 8; pixel++)
        {
            uint8_t bit_pos = flip_x ? pixel : (7 - pixel);
            
            // Extract 4-bit color index from 4 bitplanes
            uint8_t color_index = 0;
            color_index |= ((byte1 >> bit_pos) & 1) << 0;  // Bitplane 0
            color_index |= ((byte2 >> bit_pos) & 1) << 1;  // Bitplane 1
            color_index |= ((byte3 >> bit_pos) & 1) << 2;  // Bitplane 2
            color_index |= ((byte4 >> bit_pos) & 1) << 3;  // Bitplane 3
            
            // Add palette offset (16 colors per palette)
            if (palette_select && color_index != 0) {
                color_index += 16;
            }
            
            // Calculate screen position with horizontal scrolling
            uint16_t screen_x = (screen_tile_x * 8 + pixel - (scroll_x % 8) + 256) % 256;
            
            // Get final color and write to framebuffer
            uint32_t rgb = vdp_get_color(vdp, color_index);
            framebuffer[(scanline * 256) + screen_x] = rgb;
        }
    }
    
    // // Now draw sprites on top of the background
    // // Master System supports up to 64 sprites, but only 8 per scanline
    // uint8_t sprites_on_line = 0;
    
    // // Get sprite size setting from register
    // uint8_t sprite_height = (vdp->registers[1] & 0x02) ? 16 : 8;
    
    // // Get sprite pattern generator base address from register 6
    // uint16_t sprite_pattern_base = (vdp->registers[6] & 0x04) ? 0x2000 : 0x0000;
    
    // for (uint16_t sprite_idx = 0; sprite_idx < 64; sprite_idx++)
    // {
    //     // Check sprite limit per scanline
    //     if (sprites_on_line >= 8) {
    //         // Set sprite overflow flag if your VDP struct supports it
    //         // vdp->status |= VDP_STATUS_SPRITE_OVERFLOW;
    //         break;
    //     }
        
    //     // Read sprite Y coordinate from SAT
    //     uint8_t sprite_y = vdp->sat[sprite_idx];
        
    //     // Check for end-of-sprite-list marker
    //     if (sprite_y == 0xD0) {
    //         break;
    //     }
        
    //     // Check if sprite is on this scanline
    //     // Master System sprite Y coordinate system: Y=0 means sprite starts at line 1
    //     int16_t sprite_line = (int16_t)scanline - (sprite_y + 1);
    //     if (sprite_line < 0 || sprite_line >= sprite_height) {
    //         continue;
    //     }
        
    //     sprites_on_line++;
        
    //     // Read sprite X position and pattern index from SAT
    //     uint16_t sat_x_offset = 0x80 + (sprite_idx * 2);
    //     uint8_t sprite_x = vdp->sat[sat_x_offset];
    //     uint8_t sprite_pattern = vdp->sat[sat_x_offset + 1];
        
    //     // Handle left column masking (sprites with X < 8 are shifted left by 8)
    //     bool left_shift = false;
    //     if (sprite_x < 8) {
    //         left_shift = true;
    //     }
        
    //     // Handle 8x16 sprites
    //     uint8_t current_sprite_line = sprite_line;
    //     if (sprite_height == 16) {
    //         sprite_pattern &= 0xFE;  // Use even pattern numbers for 8x16 sprites
    //         if (sprite_line >= 8) {
    //             sprite_pattern |= 0x01;  // Use next pattern for bottom half
    //             current_sprite_line = sprite_line - 8;
    //         }
    //     }
        
    //     // Calculate sprite tile address in pattern table
    //     uint16_t sprite_tile_address = sprite_pattern_base + (sprite_pattern * 32);
        
    //     // Read the four bitplanes for this sprite row
    //     uint8_t sbyte1 = vdp->vram[sprite_tile_address + current_sprite_line * 4 + 0];
    //     uint8_t sbyte2 = vdp->vram[sprite_tile_address + current_sprite_line * 4 + 1];
    //     uint8_t sbyte3 = vdp->vram[sprite_tile_address + current_sprite_line * 4 + 2];
    //     uint8_t sbyte4 = vdp->vram[sprite_tile_address + current_sprite_line * 4 + 3];
        
    //     // Draw sprite pixels
    //     for (int pixel = 0; pixel < 8; pixel++)
    //     {
    //         // Calculate screen position
    //         int16_t screen_x = sprite_x + pixel;
            
    //         // Apply left column shift if needed
    //         if (left_shift) {
    //             screen_x -= 8;
    //         }
            
    //         // Check horizontal bounds
    //         if (screen_x < 0 || screen_x >= 256) {
    //             continue;
    //         }
            
    //         // Extract sprite pixel color
    //         uint8_t sprite_color = 0;
    //         sprite_color |= ((sbyte1 >> (7 - pixel)) & 1) << 0;
    //         sprite_color |= ((sbyte2 >> (7 - pixel)) & 1) << 1;
    //         sprite_color |= ((sbyte3 >> (7 - pixel)) & 1) << 2;
    //         sprite_color |= ((sbyte4 >> (7 - pixel)) & 1) << 3;
            
    //         // Skip transparent pixels (color 0)
    //         if (sprite_color == 0) 
    //         {
    //             continue;
    //         }
            
    //         // Sprites use palette 1 (colors 16-31)
    //         sprite_color += 32;
            
    //         // Check for sprite collision (if enabled)
    //         uint32_t current_pixel = framebuffer[scanline * 256 + screen_x];
    //         uint32_t bg_color = vdp_get_color(vdp, 0);
    //         if (current_pixel != bg_color) {  // If not background color
    //             // Set sprite collision flag if your VDP struct supports it
    //             // vdp->status |= VDP_STATUS_SPRITE_COLLISION;
    //         }
            
    //         // Draw sprite pixel (sprites have priority over background)
    //         uint32_t sprite_rgb = vdp_get_color(vdp, sprite_color);
    //         framebuffer[scanline * 256 + screen_x] = sprite_rgb;
    //     }
    // }
}

void vdp_process_scanline(struct vdp_t *vdp, uint32_t *framebuffer)
{
    // Handle HBlank interrupt (line counter)
    if (vdp->v_counter < 192)
    { // During the active display
        if (vdp->line_counter == 0)
        {
            vdp->line_counter = vdp->line_counter_reload_value;
            if (vdp->line_interrupt_enable)
            {
                vdp->irq_pending = 1;
            }
        }
        else
        {
            vdp->line_counter--;
        }
    }
    else
    { // During VBlank
        vdp->line_counter = vdp->line_counter_reload_value;
    }

    vdp->v_counter++;
    if (vdp->v_counter < 192 && vdp->display_enable)
    {
        vdp_draw_scanline(vdp, framebuffer, vdp->v_counter);
    }

    // Check for VBlank interrupt
    if (vdp->v_counter == VDP_NTSC_SCANLINE_INTERRUPT_LINE)
    {
        // Trigger VBlank interrupt if enabled (IE0 bit in register 1)
        if (vdp->frame_interrupt_enable) // IE0 bit (bit 5)
        {
            vdp->status_flag |= 0x80; // Set VBlank interrupt flag in status
            vdp->vblank_flag = true;
            vdp->irq_pending = 1;
        }
    }

    if (vdp->v_counter > VDP_SCANLINES_NTSC)
    {
        vdp->v_counter -= VDP_SCANLINES_NTSC;
    }
}

uint8_t vdp_port_read(struct vdp_t *vdp, uint8_t port)
{
    switch (port)
    {
    case 0x7E: // V-Counter
        return vdp->v_counter;

    case 0x7F: // H-Counter
        return vdp->h_counter;

    case 0xBE: // VDP Data Port
        vdp->second_write = false;
        return vdp_data_port_read(vdp);

    case 0xBF: // VDP Control/Status Port
        return vdp_control_port_read(vdp);

    default:
        return 0xFF;
    }
}

void vdp_port_write(struct vdp_t *vdp, uint8_t port, uint8_t value)
{
    switch (port) {
        case 0xBE: // VDP Data Port
            vdp->second_write = false;
            vdp_data_port_write(vdp, value);
            break;
            
        case 0xBF: // VDP Control Port
            vdp_control_port_write(vdp, value);
            break;
            
        default:
            // Other ports are read-only or ignored
            break;
    }
}

/*
The VDP control port is a read/write port allowing the VDP registers to be written;
the VRAM/CRAM read/write address to be set; and the status flags to be read. 
The control port address is $BF (used in outa/ina instructions).
*/

void vdp_control_port_write(struct vdp_t *vdp, uint8_t value)
{
    if (!vdp->second_write)
    {
        vdp->control_word = (vdp->control_word & ~0xFF);
        vdp->control_word = vdp->control_word| value;
        vdp->second_write = true;
    }
    else
    {
        vdp->control_word = (vdp->control_word & ~0xFF00);
        vdp->control_word = vdp->control_word | (value << 8);
        vdp->address = vdp->control_word & 0x3FFF;
        vdp->code = (vdp->control_word >> 14) & 0x3;
        vdp->second_write = false;
        
        switch (vdp->code)
        {
        case 0x00:
            vdp->read_buffer = vdp->vram[vdp->address];
            vdp->address++; // Increment address after initial fill
            if (vdp->address > 0x3FFF)
                vdp->address = 0x0;
            break;
        case 0x01: break;
        case 0x02: vdp_write_register(vdp, (vdp->control_word >> 8) & 0x0F, vdp->control_word & 0xFF); break;
        case 0x03: break; 
        }
    }
}

uint8_t vdp_control_port_read(struct vdp_t *vdp)
{
    vdp->status_flag = vdp->vblank_flag << 7 | vdp->sprite_overflow_flag << 6 | vdp->sprite_collision_flag << 5 | vdp->fifth_sprite;
    vdp->second_write = false;
    vdp->vblank_flag = false;
    vdp->irq_pending = 0; 
    vdp->sprite_collision_flag = false;
    vdp->fifth_sprite = 0x0;
    return vdp->status_flag; 
}

uint8_t vdp_data_port_read(struct vdp_t *vdp)
{
    uint8_t value_to_return = vdp->read_buffer; // Return the *previously* buffered value
    vdp->read_buffer = vdp->vram[vdp->address]; // Load the *next* value into the buffer
    vdp->address++;
    if (vdp->address > 0x3FFF)
        vdp->address = 0x0;
    return value_to_return;
}

bool vdp_interrupt_pending(struct vdp_t *vdp)
{
    return vdp->irq_pending;
}

void vdp_data_port_write(struct vdp_t *vdp, uint8_t value)
{
    if(vdp->code == 0x1)    // Vram Write Address
    {
        vdp->vram[vdp->address] = value; // Write to VRAM
        vdp->read_buffer = value;         // Copy to buffer as per text
        vdp->address++;                   // Increment address
        if (vdp->address > 0x3FFF)
            vdp->address = 0x0;
    }
    else if(vdp->code == 0x3) // Cram Write address
    {
        vdp->cram[vdp->address & 0x3f] = value; // Write to CRAM
        vdp->read_buffer = value;         // Copy to buffer as per text
        vdp->address++;                   // Increment address
        if (vdp->address > 0x3FFF)
            vdp->address = 0x0;
    }

}

void vdp_write_register(struct vdp_t *vdp, vdp_register_t r, uint8_t value)
{
    if (r > VDP_REGISTER_COUNT)
        return;
    vdp->registers[r] = value;

    switch (r)
    {
    case VDP_REG_MODE_CONTROL_1:
        vdp->vertical_scroll_lock   = (value & 0x80) != 0;
        vdp->horizontal_scroll_lock = (value & 0x40) != 0;
        vdp->hide_left_column       = (value & 0x20) != 0;
        vdp->line_interrupt_enable  = (value & 0x10) != 0;
        vdp->shift_sprites_left     = (value & 0x08) != 0;
        vdp->mode4_enable           = (value & 0x04) != 0;
        vdp->extra_height_enable    = (value & 0x02) != 0;
        vdp->sync_disable           = (value & 0x01) != 0;
        break;
    case VDP_REG_MODE_CONTROL_2:
        vdp->display_enable         = (value & 0x40) != 0;
        vdp->frame_interrupt_enable = (value & 0x20) != 0;
        vdp->mode1_enable           = (value & 0x10) != 0;
        vdp->mode3_enable           = (value & 0x08) != 0;
        vdp->sprite_size            = (value & 0x02) != 0;
        vdp->sprite_doubled         = (value & 0x01) != 0;
        break;
    case VDP_REG_NAME_TABLE_BASE:
        vdp->name_table_base = value;
        break;
    case VDP_REG_COLOR_TABLE_BASE:
        vdp->color_table_base = value;
        break;
    case VDP_REG_PATTERN_GENERATOR_BASE:
        vdp->pattern_gen_base = value;
        break;
    case VDP_REG_SPRITE_ATTRIBUTE_TABLE_BASE:
        vdp->sprite_attr_base = value;
        break;
    case VDP_REG_SPRITE_PATTERN_GENERATOR_BASE:
        vdp->sprite_pattern_base = value;
        break;
    case VDP_REG_BACKDROP_COLOR:
        vdp->backdrop_color = value;
        break;
    case VDP_REG_HORIZONTAL_SCROLL:
        vdp->bg_x_scroll = value;
        break;
    case VDP_REG_VERTICAL_SCROLL:
        vdp->bg_y_scroll = value;
        break;
    case VDP_REG_LINE_INTERRUPT_COUNTER:
        vdp->line_counter_reload_value = value;
        break;

    default:
        break;
    }
}