/*
 * Nuklear - 4.9.4 - public domain
 */
/*
 * ==============================================================
 *
 *                              API
 *
 * ===============================================================
 */
#ifndef NK_SDL_RENDERER_H_
#define NK_SDL_RENDERER_H_

#ifndef NK_SDL_RENDERER_SDL_H
#define NK_SDL_RENDERER_SDL_H "SDL3/SDL.h"
#endif
#include NK_SDL_RENDERER_SDL_H
NK_API struct nk_context*   nk_sdl_init(SDL_Window *win, SDL_Renderer *renderer);
NK_API void                 nk_sdl_font_stash_begin(struct nk_font_atlas **atlas);
NK_API void                 nk_sdl_font_stash_end(void);
NK_API int                  nk_sdl_handle_event(SDL_Event *evt);
NK_API void                 nk_sdl_render(enum nk_anti_aliasing);
NK_API void                 nk_sdl_shutdown(void);
NK_API void                 nk_sdl_handle_grab(void);

#if SDL_COMPILEDVERSION < SDL_VERSIONNUM(2, 0, 22)
/* Metal API does not support cliprects with negative coordinates or large
 * dimensions. The issue is fixed in SDL2 with version 2.0.22 but until
 * that version is released, the NK_SDL_CLAMP_CLIP_RECT flag can be used to
 * ensure the cliprect is itself clipped to the viewport.
 * See discussion at https://discourse.libsdl.org/t/rendergeometryraw-producing-different-results-in-metal-vs-opengl/34953
 */
#define NK_SDL_CLAMP_CLIP_RECT
#endif

#endif /* NK_SDL_RENDERER_H_ */

/*
 * ==============================================================
 *
 *                          IMPLEMENTATION
 *
 * ===============================================================
 */
#ifdef NK_SDL_RENDERER_IMPLEMENTATION
#include <string.h>
#include <stdlib.h>
#include <stddef.h>

struct nk_sdl_device {
    struct nk_buffer cmds;
    struct nk_draw_null_texture tex_null;
    SDL_Texture *font_tex;
};

struct nk_sdl_vertex {
    float position[2];
    float uv[2];
    float col[4];
};

static struct nk_sdl {
    SDL_Window *win;
    SDL_Renderer *renderer;
    struct nk_sdl_device ogl;
    struct nk_context ctx;
    struct nk_font_atlas atlas;
    Uint64 time_of_last_frame;
} sdl;

NK_INTERN void
nk_sdl_device_upload_atlas(const void *image, int width, int height)
{
    struct nk_sdl_device *dev = &sdl.ogl;

    SDL_Texture *g_SDLFontTexture = SDL_CreateTexture(sdl.renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, width, height);
    if (g_SDLFontTexture == NULL) {
        SDL_Log("error creating texture");
        return;
    }
    SDL_UpdateTexture(g_SDLFontTexture, NULL, image, 4 * width);
    SDL_SetTextureBlendMode(g_SDLFontTexture, SDL_BLENDMODE_BLEND);
    dev->font_tex = g_SDLFontTexture;
}

NK_API void
nk_sdl_render(enum nk_anti_aliasing AA)
{
    /* setup global state */
    struct nk_sdl_device *dev = &sdl.ogl;

    {
        SDL_Rect saved_clip;
#ifdef NK_SDL_CLAMP_CLIP_RECT
        SDL_Rect viewport;
#endif
        bool clipping_enabled;
        int vs = sizeof(struct nk_sdl_vertex);
        size_t vp = offsetof(struct nk_sdl_vertex, position);
        size_t vt = offsetof(struct nk_sdl_vertex, uv);
        size_t vc = offsetof(struct nk_sdl_vertex, col);

        /* convert from command queue into draw list and draw to screen */
        const struct nk_draw_command *cmd;
        const nk_draw_index *offset = NULL;
        struct nk_buffer vbuf, ebuf;

        /* fill converting configuration */
        struct nk_convert_config config;
        static const struct nk_draw_vertex_layout_element vertex_layout[] = {
            {NK_VERTEX_POSITION, NK_FORMAT_FLOAT, NK_OFFSETOF(struct nk_sdl_vertex, position)},
            {NK_VERTEX_TEXCOORD, NK_FORMAT_FLOAT, NK_OFFSETOF(struct nk_sdl_vertex, uv)},
            {NK_VERTEX_COLOR, NK_FORMAT_R32G32B32A32_FLOAT, NK_OFFSETOF(struct nk_sdl_vertex, col)},
            {NK_VERTEX_LAYOUT_END}
        };

        Uint64 now = SDL_GetTicks();
        sdl.ctx.delta_time_seconds = (float)(now - sdl.time_of_last_frame) / 1000;
        sdl.time_of_last_frame = now;

        NK_MEMSET(&config, 0, sizeof(config));
        config.vertex_layout = vertex_layout;
        config.vertex_size = sizeof(struct nk_sdl_vertex);
        config.vertex_alignment = NK_ALIGNOF(struct nk_sdl_vertex);
        config.tex_null = dev->tex_null;
        config.circle_segment_count = 22;
        config.curve_segment_count = 22;
        config.arc_segment_count = 22;
        config.global_alpha = 1.0f;
        config.shape_AA = AA;
        config.line_AA = AA;

        /* convert shapes into vertexes */
        nk_buffer_init_default(&vbuf);
        nk_buffer_init_default(&ebuf);
        nk_convert(&sdl.ctx, &dev->cmds, &vbuf, &ebuf, &config);

        /* iterate over and execute each draw command */
        offset = (const nk_draw_index*)nk_buffer_memory_const(&ebuf);

        clipping_enabled = SDL_RenderClipEnabled(sdl.renderer);
        SDL_GetRenderClipRect(sdl.renderer, &saved_clip);
#ifdef NK_SDL_CLAMP_CLIP_RECT
        SDL_GetRenderViewport(sdl.renderer, &viewport);
#endif

        nk_draw_foreach(cmd, &sdl.ctx, &dev->cmds)
        {
            if (!cmd->elem_count) continue;

            {
                SDL_Rect r;
                r.x = cmd->clip_rect.x;
                r.y = cmd->clip_rect.y;
                r.w = cmd->clip_rect.w;
                r.h = cmd->clip_rect.h;
#ifdef NK_SDL_CLAMP_CLIP_RECT
                if (r.x < 0) {
                    r.w += r.x;
                    r.x = 0;
                }
                if (r.y < 0) {
                    r.h += r.y;
                    r.y = 0;
                }
                if (r.h > viewport.h) {
                    r.h = viewport.h;
                }
                if (r.w > viewport.w) {
                    r.w = viewport.w;
                }
#endif
                SDL_SetRenderClipRect(sdl.renderer, &r);
            }

            {
                const void *vertices = nk_buffer_memory_const(&vbuf);

                SDL_RenderGeometryRaw(sdl.renderer,
                        (SDL_Texture *)cmd->texture.ptr,
                        (const float*)((const nk_byte*)vertices + vp), vs,
                        (const SDL_FColor*)((const nk_byte*)vertices + vc), vs,
                        (const float*)((const nk_byte*)vertices + vt), vs,
                        (vbuf.needed / vs),
                        (void *) offset, cmd->elem_count, 2);

                offset += cmd->elem_count;
            }
        }

        SDL_SetRenderClipRect(sdl.renderer, &saved_clip);
        if (!clipping_enabled) {
            SDL_SetRenderClipRect(sdl.renderer, NULL);
        }

        nk_clear(&sdl.ctx);
        nk_buffer_clear(&dev->cmds);
        nk_buffer_free(&vbuf);
        nk_buffer_free(&ebuf);
    }
}

static void
nk_sdl_clipboard_paste(nk_handle usr, struct nk_text_edit *edit)
{
    const char *text = SDL_GetClipboardText();
    if (text) {
        nk_textedit_paste(edit, text, nk_strlen(text));
        SDL_free((void *)text);
    }
    (void)usr;
}

static void
nk_sdl_clipboard_copy(nk_handle usr, const char *text, int len)
{
    char *str = 0;
    (void)usr;
    if (!len) return;
    str = (char*)malloc((size_t)len+1);
    if (!str) return;
    memcpy(str, text, (size_t)len);
    str[len] = '\0';
    SDL_SetClipboardText(str);
    free(str);
}

NK_API struct nk_context*
nk_sdl_init(SDL_Window *win, SDL_Renderer *renderer)
{
#ifndef NK_SDL_CLAMP_CLIP_RECT
    SDL_RendererInfo info;
    SDL_version runtimeVer;

    /* warn for cases where NK_SDL_CLAMP_CLIP_RECT should have been set but isn't */
    SDL_GetRendererInfo(renderer, &info);
    SDL_GetVersion(&runtimeVer);
    if (strncmp("metal", info.name, 5) == 0 &&
        SDL_VERSIONNUM(runtimeVer.major, runtimeVer.minor, runtimeVer.patch) < SDL_VERSIONNUM(2, 0, 22))
    {
        SDL_LogWarn(
            SDL_LOG_CATEGORY_APPLICATION,
            "renderer is using Metal API but runtime SDL version %d.%d.%d is older than compiled version %d.%d.%d, "
            "which may cause issues with rendering",
            runtimeVer.major, runtimeVer.minor, runtimeVer.patch,
            SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_PATCHLEVEL
        );
    }
#endif
    sdl.win = win;
    sdl.renderer = renderer;
    sdl.time_of_last_frame = SDL_GetTicks();
    nk_init_default(&sdl.ctx, 0);
    sdl.ctx.clip.copy = nk_sdl_clipboard_copy;
    sdl.ctx.clip.paste = nk_sdl_clipboard_paste;
    sdl.ctx.clip.userdata = nk_handle_ptr(0);
    nk_buffer_init_default(&sdl.ogl.cmds);
    return &sdl.ctx;
}

NK_API void
nk_sdl_font_stash_begin(struct nk_font_atlas **atlas)
{
    nk_font_atlas_init_default(&sdl.atlas);
    nk_font_atlas_begin(&sdl.atlas);
    *atlas = &sdl.atlas;
}

NK_API void
nk_sdl_font_stash_end(void)
{
    const void *image; int w, h;
    image = nk_font_atlas_bake(&sdl.atlas, &w, &h, NK_FONT_ATLAS_RGBA32);
    nk_sdl_device_upload_atlas(image, w, h);
    nk_font_atlas_end(&sdl.atlas, nk_handle_ptr(sdl.ogl.font_tex), &sdl.ogl.tex_null);
    if (sdl.atlas.default_font)
        nk_style_set_font(&sdl.ctx, &sdl.atlas.default_font->handle);
}

NK_API void
nk_sdl_handle_grab(void)
{
    struct nk_context *ctx = &sdl.ctx;
    if (ctx->input.mouse.grab)
    {
        SDL_SetWindowRelativeMouseMode(sdl.win, true);
    }
    else if (ctx->input.mouse.ungrab)
    {
        /* better support for older SDL by setting mode first; causes an extra mouse motion event */
        SDL_SetWindowRelativeMouseMode(sdl.win, false);
        SDL_WarpMouseInWindow(sdl.win, (int)ctx->input.mouse.prev.x, (int)ctx->input.mouse.prev.y);
    }
    else if (ctx->input.mouse.grabbed)
    {
        ctx->input.mouse.pos.x = ctx->input.mouse.prev.x;
        ctx->input.mouse.pos.y = ctx->input.mouse.prev.y;
    }
}

NK_API int
nk_sdl_handle_event(SDL_Event *evt)
{
    struct nk_context *ctx = &sdl.ctx;
    int ctrl_down = SDL_GetModState() & (SDL_KMOD_LCTRL | SDL_KMOD_RCTRL);

    switch(evt->type)
    {
        case SDL_EVENT_KEY_UP: /* KEYUP & KEYDOWN share same routine */
        case SDL_EVENT_KEY_DOWN:
            {
                int down = evt->type == SDL_EVENT_KEY_DOWN;
                switch(evt->key.key)
                {
                    case SDLK_RSHIFT: /* RSHIFT & LSHIFT share same routine */
                    case SDLK_LSHIFT:    nk_input_key(ctx, NK_KEY_SHIFT, down); break;
                    case SDLK_DELETE:    nk_input_key(ctx, NK_KEY_DEL, down); break;

                    case SDLK_KP_ENTER:
                    case SDLK_RETURN:    nk_input_key(ctx, NK_KEY_ENTER, down); break;

                    case SDLK_TAB:       nk_input_key(ctx, NK_KEY_TAB, down); break;
                    case SDLK_BACKSPACE: nk_input_key(ctx, NK_KEY_BACKSPACE, down); break;
                    case SDLK_HOME:      nk_input_key(ctx, NK_KEY_TEXT_START, down);
                                         nk_input_key(ctx, NK_KEY_SCROLL_START, down); break;
                    case SDLK_END:       nk_input_key(ctx, NK_KEY_TEXT_END, down);
                                         nk_input_key(ctx, NK_KEY_SCROLL_END, down); break;
                    case SDLK_PAGEDOWN:  nk_input_key(ctx, NK_KEY_SCROLL_DOWN, down); break;
                    case SDLK_PAGEUP:    nk_input_key(ctx, NK_KEY_SCROLL_UP, down); break;
                    case SDLK_Z:         nk_input_key(ctx, NK_KEY_TEXT_UNDO, down && ctrl_down); break;
                    case SDLK_R:         nk_input_key(ctx, NK_KEY_TEXT_REDO, down && ctrl_down); break;
                    case SDLK_C:         nk_input_key(ctx, NK_KEY_COPY, down && ctrl_down); break;
                    case SDLK_V:         nk_input_key(ctx, NK_KEY_PASTE, down && ctrl_down); break;
                    case SDLK_X:         nk_input_key(ctx, NK_KEY_CUT, down && ctrl_down); break;
                    case SDLK_B:         nk_input_key(ctx, NK_KEY_TEXT_LINE_START, down && ctrl_down); break;
                    case SDLK_E:         nk_input_key(ctx, NK_KEY_TEXT_LINE_END, down && ctrl_down); break;
                    case SDLK_UP:        nk_input_key(ctx, NK_KEY_UP, down); break;
                    case SDLK_DOWN:      nk_input_key(ctx, NK_KEY_DOWN, down); break;
                    case SDLK_A:
                        if (ctrl_down)
                            nk_input_key(ctx,NK_KEY_TEXT_SELECT_ALL, down);
                        break;
                    case SDLK_LEFT:
                        if (ctrl_down)
                            nk_input_key(ctx, NK_KEY_TEXT_WORD_LEFT, down);
                        else nk_input_key(ctx, NK_KEY_LEFT, down);
                        break;
                    case SDLK_RIGHT:
                        if (ctrl_down)
                            nk_input_key(ctx, NK_KEY_TEXT_WORD_RIGHT, down);
                        else nk_input_key(ctx, NK_KEY_RIGHT, down);
                        break;
                }
            }
            return 1;

        case SDL_EVENT_MOUSE_BUTTON_UP: /* MOUSEBUTTONUP & MOUSEBUTTONDOWN share same routine */
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            {
                int down = evt->type == SDL_EVENT_MOUSE_BUTTON_DOWN;
                const int x = evt->button.x, y = evt->button.y;
                switch(evt->button.button)
                {
                    case SDL_BUTTON_LEFT:
                        if (evt->button.clicks > 1)
                            nk_input_button(ctx, NK_BUTTON_DOUBLE, x, y, down);
                        nk_input_button(ctx, NK_BUTTON_LEFT, x, y, down); break;
                    case SDL_BUTTON_MIDDLE: nk_input_button(ctx, NK_BUTTON_MIDDLE, x, y, down); break;
                    case SDL_BUTTON_RIGHT:  nk_input_button(ctx, NK_BUTTON_RIGHT, x, y, down); break;
                }
            }
            return 1;

        case SDL_EVENT_MOUSE_MOTION:
            if (ctx->input.mouse.grabbed) {
                int x = (int)ctx->input.mouse.prev.x, y = (int)ctx->input.mouse.prev.y;
                nk_input_motion(ctx, x + evt->motion.xrel, y + evt->motion.yrel);
            }
            else nk_input_motion(ctx, evt->motion.x, evt->motion.y);
            return 1;

        case SDL_EVENT_TEXT_INPUT:
            {
                nk_glyph glyph;
                memcpy(glyph, evt->text.text, NK_UTF_SIZE);
                nk_input_glyph(ctx, glyph);
            }
            return 1;

        case SDL_EVENT_MOUSE_WHEEL:
            nk_input_scroll(ctx,nk_vec2(evt->wheel.mouse_x, evt->wheel.mouse_y));
            return 1;
    }
    return 0;
}

NK_API
void nk_sdl_shutdown(void)
{
    struct nk_sdl_device *dev = &sdl.ogl;
    nk_font_atlas_clear(&sdl.atlas);
    nk_free(&sdl.ctx);
    SDL_DestroyTexture(dev->font_tex);
    /* glDeleteTextures(1, &dev->font_tex); */
    nk_buffer_free(&dev->cmds);
    memset(&sdl, 0, sizeof(sdl));
}

#endif /* NK_SDL_RENDERER_IMPLEMENTATION */
