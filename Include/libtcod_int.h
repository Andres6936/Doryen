/*
* libtcod 1.5.1
* Copyright (c) 2008,2009,2010,2012 Jice & Mingos
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * The name of Jice or Mingos may not be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY JICE AND MINGOS ``AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL JICE OR MINGOS BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef _TCODLIB_INT_H
#define _TCODLIB_INT_H
#include <stdarg.h>
#include <assert.h>
/* tcodlib internal stuff */
#ifdef __cplusplus
extern "C" {
#endif

/* a cell in the console */
typedef struct {
	int c;		/* character ascii code */
	int cf;		/* character number in font */
	TCOD_color_t fore;	/* foreground color */
	TCOD_color_t back;	/* background color */
	uint8 dirt;	/* cell modified since last flush ? */
} char_t;

/* Console non public data */
typedef struct {
	char_t *buf; /* current console */
	char_t *oldbuf; /* console for last frame */
	/* console width and height (in characters,not pixels) */
	int w,h;
	/* default background operator for print & print_rect functions */
	TCOD_bkgnd_flag_t bkgnd_flag;
	/* default alignment for print & print_rect functions */
	TCOD_alignment_t alignment;
	/* foreground (text), background and key colors */
	TCOD_color_t fore,back,key;
	uint8 fade;
	bool haskey; /* a key color has been defined */
} TCOD_console_data_t;

/* fov internal stuff */
typedef struct {
	bool transparent:1;
	bool walkable:1;
	bool fov:1;
} cell_t;
typedef struct {
	int width;
	int height;
	int nbcells;
	cell_t *cells;
} map_t;

/* pseudorandom number generator toolkit */
typedef struct {
	/* algorithm identifier */
	TCOD_random_algo_t algo;
	/* distribution */
	TCOD_distribution_t distribution;
	/* Mersenne Twister stuff */
	uint32 mt[624];
	int cur_mt;
	/* Complementary-Multiply-With-Carry stuff */
	/* shared with Generalised Feedback Shift Register */
	uint32 Q[4096], c;
    int cur;
} mersenne_data_t;

typedef struct {
	/* number of characters in the bitmap font */
	int fontNbCharHoriz;
	int fontNbCharVertic;
	/* font type and layout */
	bool font_tcod_layout;
	bool font_in_row;
	bool font_greyscale;
	/* character size in font */
	int font_width;
	int font_height;
	char font_file[512];
	char window_title[512];
	/* ascii code to tcod layout converter */
	int *ascii_to_tcod;
	/* whether each character in the font is a colored tile */
	bool *colored;
	/* the root console */
	TCOD_console_data_t *root;
	/* nb chars in the font */
	int max_font_chars;
	/* fullscreen data */
	bool fullscreen;
	int fullscreen_offsetx;
	int fullscreen_offsety;
	/* asked by the user */
	int fullscreen_width;
	int fullscreen_height;
	/* actual resolution */
	int actual_fullscreen_width;
	int actual_fullscreen_height;
	/* renderer to use */
	TCOD_renderer_t renderer;
	/* user post-processing callback */
	SDL_renderer_t sdl_cbk;
	/* fading data */
	TCOD_color_t fading_color;
	uint8 fade;
} TCOD_internal_context_t;

extern TCOD_internal_context_t TCOD_ctx;

#if defined(__ANDROID__) && !defined(NDEBUG)
#include <android/log.h>
#ifdef printf
#undef printf
#endif
#ifdef vprintf
#undef vprintf
#endif
#define printf(args...) __android_log_print(ANDROID_LOG_INFO, "libtcod", ## args)
#define vprintf(args...) __android_log_vprint(ANDROID_LOG_INFO, "libtcod", ## args)

#ifdef assert
#undef assert
#endif
#define assert(cond) if(!(cond)) __android_log_assert(#cond, "libtcod", "assertion failed: %s", #cond)
#endif

#ifdef NDEBUG
#define TCOD_IF(x) if (x)
#define TCOD_IFNOT(x) if (!(x))
#define TCOD_ASSERT(x)
#define TCOD_LOG(x)
#else
#define TCOD_IF(x) assert(x);
#define TCOD_IFNOT(x) assert(x); if (0)
#define TCOD_ASSERT(x) assert(x)
#define TCOD_LOG(x) printf x
#endif

#ifndef NO_OPENGL
/* opengl utilities */
void TCOD_opengl_init_attributes();
bool TCOD_opengl_init_state(int conw, int conh, void *font_tex);
bool TCOD_opengl_init_shaders();
bool TCOD_opengl_render(int oldFade, bool *ascii_updated, char_t *console_buffer, char_t *prev_console_buffer);
void TCOD_opengl_swap();
void * TCOD_opengl_get_screen();
#endif

void TCOD_map_compute_fov_diamond_raycasting(TCOD_map_t map, int player_x, int player_y, int max_radius, bool light_walls);
void TCOD_map_compute_fov_permissive2(TCOD_map_t map, int player_x, int player_y, int max_radius, bool light_walls, int fovType);
void TCOD_map_compute_fov_restrictive_shadowcasting(TCOD_map_t map, int player_x, int player_y, int max_radius, bool light_walls);

/* Console non public methods*/
bool TCOD_console_init(TCOD_console_t con,const char *title, bool fullscreen);
int TCOD_console_print_internal(TCOD_console_t con,int x,int y, int w, int h, TCOD_bkgnd_flag_t flag, TCOD_alignment_t align, char *msg, bool can_split, bool count_only);
int TCOD_console_stringLength(const unsigned char *s);
unsigned char * TCOD_console_forward(unsigned char *s,int l);
void TCOD_console_set_window_closed();
char *TCOD_console_vsprint(const char *fmt, va_list ap);
char_t *TCOD_console_get_buf(TCOD_console_t con);
/* fatal errors */
void TCOD_fatal(const char *fmt, ...);
void TCOD_fatal_nopar(const char *msg);

/* Platform non public methods */
void TCOD_sys_startup();
bool TCOD_sys_init(int w,int h, char_t *buf, char_t *oldbuf, bool fullscreen);
void TCOD_sys_set_custom_font(const char *font_name,int nb_ch, int nb_cv,int flags);
void TCOD_sys_map_ascii_to_font(int asciiCode, int fontCharX, int fontCharY);
void *TCOD_sys_create_bitmap_for_console(TCOD_console_t console);
void TCOD_sys_save_bitmap(void *bitmap, const char *filename);
void *TCOD_sys_create_bitmap(int width, int height, TCOD_color_t *buf);
void TCOD_sys_delete_bitmap(void *bitmap);
void TCOD_sys_console_to_bitmap(void *bitmap, int console_width, int console_height, char_t *console_buffer, char_t *prev_console_buffer);
void TCOD_sys_set_keyboard_repeat(int initial_delay, int interval);
void *TCOD_sys_get_surface(int width, int height, bool alpha);
void TCOD_sys_save_fps();
void TCOD_sys_restore_fps();

/* switch fullscreen mode */
void TCOD_sys_set_fullscreen(bool fullscreen);
void TCOD_sys_flush(bool render);
TCOD_key_t TCOD_sys_check_for_keypress(int flags);
TCOD_key_t TCOD_sys_wait_for_keypress(bool flush);
bool TCOD_sys_is_key_pressed(TCOD_keycode_t key);
void TCOD_sys_set_window_title(const char *title);
/* close the window */
void TCOD_sys_term();

/* image manipulation */
void *TCOD_sys_load_image(const char *filename);
void TCOD_sys_get_image_size(const void *image, int *w,int *h);
TCOD_color_t TCOD_sys_get_image_pixel(const void *image,int x, int y);
int TCOD_sys_get_image_alpha(const void *image,int x, int y);
bool TCOD_sys_check_magic_number(const char *filename, int size, uint8 *data);

/* TCOD_list nonpublic methods */
void TCOD_list_set_size(TCOD_list_t l, int size);

/* color values */
#define TCOD_BLACK 0,0,0
#define TCOD_GREY 127,127,127
#define TCOD_WHITE 255,255,255
#define TCOD_SEPIA 127,101,63


/* lightest */
#define TCOD_LIGHTEST_YELLOW 255,255,191
#define TCOD_LIGHTEST_BLUE 191,191,255

/* lighter */
#define TCOD_LIGHTER_BLUE 127,127,255

/* light */
#define TCOD_LIGHT_YELLOW 255,255,63
#define TCOD_LIGHT_BLUE 63,63,255

/* normal */
#define TCOD_RED 255,0,0
#define TCOD_FLAME 255,63,0
#define TCOD_ORANGE 255,127,0
#define TCOD_AMBER 255,191,0
#define TCOD_YELLOW 255,255,0
#define TCOD_LIME 191,255,0
#define TCOD_CHARTREUSE 127,255,0
#define TCOD_GREEN 0,255,0
#define TCOD_SEA 0,255,127
#define TCOD_TURQUOISE 0,255,191
#define TCOD_CYAN 0,255,255
#define TCOD_SKY 0,191,255
#define TCOD_AZURE 0,127,255
#define TCOD_BLUE 0,0,255
#define TCOD_HAN 63,0,255
#define TCOD_VIOLET 127,0,255
#define TCOD_PURPLE 191,0,255
#define TCOD_FUCHSIA 255,0,255
#define TCOD_MAGENTA 255,0,191
#define TCOD_PINK 255,0,127
#define TCOD_CRIMSON 255,0,63

/* dark */
#define TCOD_DARK_YELLOW 191,191,0
#define TCOD_DARK_BLUE 0,0,191

/* darker */
#define TCOD_DARKER_BLUE 0,0,127

/* darkest */
#define TCOD_DARKEST_FLAME 63,15,0

/* metallic */
#define TCOD_BRASS 191,151,96
#define TCOD_COPPER 197,136,124
#define TCOD_GOLD 229,191,0
#define TCOD_SILVER 203,203,203

/* miscellaneous */
#define TCOD_CELADON 172,255,175
#define TCOD_PEACH 255,159,127

#ifdef __cplusplus
}
#endif
#endif

