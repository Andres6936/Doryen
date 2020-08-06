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

#include <string.h>
#include <stdio.h>
#include <ctype.h>

#if defined (__HAIKU__) || defined(__ANDROID__)
#include <SDL.h>
#elif defined (TCOD_SDL2)
#include <SDL2/SDL.h>
#else

#include <SDL/SDL.h>

#endif

#include "Doryen/libtcod.h"
#include "Doryen/libtcod_int.h"

/* to enable bitmap locking. Is there any use ?? makes the OSX port renderer to fail */
/*#define USE_SDL_LOCKS */


/* image support stuff */
bool TCOD_sys_check_bmp(const char* filename);

SDL_Surface* TCOD_sys_read_bmp(const char* filename);

void TCOD_sys_write_bmp(const SDL_Surface* surf, const char* filename);

bool TCOD_sys_check_png(const char* filename)
{
	static uint8 magic_number[] = { 137, 80, 78, 71, 13, 10, 26, 10 };
	return TCOD_sys_check_magic_number(filename, sizeof(magic_number), magic_number);
}

SDL_Surface* TCOD_sys_read_png(const char* filename)
{
	return NULL;
}

void TCOD_sys_write_png(const SDL_Surface* surf, const char* filename)
{

}

typedef struct
{
	char* extension;

	bool (* check_type)(const char* filename);

	SDL_Surface* (* read)(const char* filename);

	void (* write)(const SDL_Surface* surf, const char* filename);
} image_support_t;

static image_support_t image_type[] = {
		{ "BMP", TCOD_sys_check_bmp, TCOD_sys_read_bmp, TCOD_sys_write_bmp },
		{ "PNG", TCOD_sys_check_png, TCOD_sys_read_png, TCOD_sys_write_png },
		{ NULL, NULL, NULL, NULL },
};


#if SDL_VERSION_ATLEAST(2, 0, 0)
																														static SDL_Window* window=NULL;
#   ifdef USE_SDL2_RENDERER
static SDL_Renderer* renderer=NULL;
static SDL_Surface* screen=NULL;
#   endif
#else

static SDL_Surface* screen = NULL;

#endif

static SDL_Surface* charmap = NULL;

static char_t* consoleBuffer = NULL;

static char_t* prevConsoleBuffer = NULL;

static bool has_startup = false;

/* font transparent color */
static TCOD_color_t fontKeyCol = { 0, 0, 0 };

static Uint32 sdl_key = 0, rgb_mask = 0, nrgb_mask = 0;

/* mouse stuff */
static bool mousebl = false;

static bool mousebm = false;

static bool mousebr = false;

static bool mouse_force_bl = false;

#if SDL_VERSION_ATLEAST(2, 0, 0)
static bool mouse_touch=true;
#endif

/* minimum length for a frame (when fps are limited) */
static int min_frame_length = 0;

static int min_frame_length_backup = 0;

/* number of frames in the last second */
static int fps = 0;

/* current number of frames */
static int cur_fps = 0;

/* length of the last rendering loop */
static float last_frame_length = 0.0f;

static TCOD_color_t* charcols = NULL;

static bool* first_draw = NULL;

static bool key_status[TCODK_CHAR + 1];

static int oldFade = -1;

/* convert SDL vk to a char (depends on the keyboard layout) */
#if SDL_VERSION_ATLEAST(2, 0, 0)
																														typedef struct {
	SDL_Keycode	sdl_key;
	int tcod_key;
} vk_to_c_entry;
#define NUM_VK_TO_C_ENTRIES 10
static vk_to_c_entry vk_to_c[NUM_VK_TO_C_ENTRIES];
#else

static char vk_to_c[SDLK_LAST];

#endif

/* convert ASCII code to TCOD layout position */
static int init_ascii_to_tcod[256] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 76, 77, 0, 0, 0, 0, 0, /* ASCII 0 to 15 */
		71, 70, 72, 0, 0, 0, 0, 0, 64, 65, 67, 66, 0, 73, 68, 69, /* ASCII 16 to 31 */
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, /* ASCII 32 to 47 */
		16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, /* ASCII 48 to 63 */
		32, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, /* ASCII 64 to 79 */
		111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 33, 34, 35, 36, 37, /* ASCII 80 to 95 */
		38, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, /* ASCII 96 to 111 */
		143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 39, 40, 41, 42, 0, /* ASCII 112 to 127 */
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* ASCII 128 to 143 */
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* ASCII 144 to 159 */
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* ASCII 160 to 175 */
		43, 44, 45, 46, 49, 0, 0, 0, 0, 81, 78, 87, 88, 0, 0, 55, /* ASCII 176 to 191 */
		53, 50, 52, 51, 47, 48, 0, 0, 85, 86, 82, 84, 83, 79, 80, 0, /* ASCII 192 to 207 */
		0, 0, 0, 0, 0, 0, 0, 0, 0, 56, 54, 0, 0, 0, 0, 0, /* ASCII 208 to 223 */
		74, 75, 57, 58, 59, 60, 61, 62, 63, 0, 0, 0, 0, 0, 0, 0, /* ASCII 224 to 239 */
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* ASCII 240 to 255 */
};

static bool* ascii_updated = NULL;

static bool any_ascii_updated = false;

static void alloc_ascii_tables()
{
	if (TCOD_ctx.ascii_to_tcod)
	{ free(TCOD_ctx.ascii_to_tcod); }
	if (ascii_updated)
	{ free(ascii_updated); }
	if (charcols)
	{
		free(charcols);
		free(first_draw);
	}

	TCOD_ctx.ascii_to_tcod = (int*)calloc(sizeof(int), TCOD_ctx.max_font_chars);
	ascii_updated = (bool*)calloc(sizeof(bool), TCOD_ctx.max_font_chars);
	charcols = (TCOD_color_t*)calloc(sizeof(TCOD_color_t), TCOD_ctx.max_font_chars);
	first_draw = (bool*)calloc(sizeof(bool), TCOD_ctx.max_font_chars);
	memcpy(TCOD_ctx.ascii_to_tcod, init_ascii_to_tcod, sizeof(int) * 256);
}

static void check_ascii_to_tcod()
{
	if (TCOD_ctx.fontNbCharHoriz * TCOD_ctx.fontNbCharVertic != TCOD_ctx.max_font_chars)
	{
		TCOD_ctx.max_font_chars = TCOD_ctx.fontNbCharHoriz * TCOD_ctx.fontNbCharVertic;
		alloc_ascii_tables();
	}
}

void* TCOD_sys_get_surface(int width, int height, bool alpha)
{
	Uint32 rmask, gmask, bmask, amask;
	SDL_Surface* bitmap;
	int flags = SDL_SWSURFACE;

	if (alpha)
	{
		if (SDL_BYTEORDER == SDL_LIL_ENDIAN)
		{
			rmask = 0x000000FF;
			gmask = 0x0000FF00;
			bmask = 0x00FF0000;
			amask = 0xFF000000;
		}
		else
		{
			rmask = 0xFF000000;
			gmask = 0x00FF0000;
			bmask = 0x0000FF00;
			amask = 0x000000FF;
		}
#if !SDL_VERSION_ATLEAST(2, 0, 0)
		flags |= SDL_SRCALPHA;
#endif
	}
	else
	{
		if (SDL_BYTEORDER == SDL_LIL_ENDIAN)
		{
			rmask = 0x0000FF;
			gmask = 0x00FF00;
			bmask = 0xFF0000;
		}
		else
		{
			rmask = 0xFF0000;
			gmask = 0x00FF00;
			bmask = 0x0000FF;
		}
		amask = 0;
	}
#if SDL_VERSION_ATLEAST(2, 0, 0)
	bitmap=SDL_CreateRGBSurface(flags,width,height,
#else
	bitmap = SDL_AllocSurface(flags, width, height,
#endif
			alpha ? 32 : 24,
			rmask, gmask, bmask, amask);
	if (alpha)
	{
#if SDL_VERSION_ATLEAST(2, 0, 0)
		SDL_SetSurfaceAlphaMod(bitmap, 255);
#else
		SDL_SetAlpha(bitmap, SDL_SRCALPHA, 255);
#endif
	}
	return (void*)bitmap;
}

#ifdef TCOD_MACOSX
void CustomSDLMain();
#endif

void TCOD_sys_save_bitmap(void* bitmap, const char* filename)
{
	image_support_t* img = image_type;
	while (img->extension != NULL && strcasestr(filename, img->extension) == NULL)
	{ img++; }
	if (img->extension == NULL || img->write == NULL)
	{ img = image_type; } /* default to bmp */
	img->write((SDL_Surface*)bitmap, filename);
}

void TCOD_sys_save_screenshot(const char* filename)
{
	char buf[128];
	if (filename == NULL)
	{
		/* generate filename */
		int idx = 0;
		do
		{
			FILE* f = NULL;
			sprintf(buf, "./screenshot%03d.png", idx);
			f = fopen(buf, "rb");
			if (!f)
			{ filename = buf; }
			else
			{
				idx++;
				fclose(f);
			}
		} while (!filename);
	}
	if (TCOD_ctx.renderer == TCOD_RENDERER_SDL)
	{
#if SDL_VERSION_ATLEAST(2, 0, 0)
																																#   ifdef USE_SDL2_RENDERER
		/* somethign with SDL_SetRenderTarget? */
#	else
		TCOD_sys_save_bitmap((void *)SDL_GetWindowSurface(window),filename);
#	endif
#else
		TCOD_sys_save_bitmap((void*)screen, filename);
#endif
#ifndef NO_OPENGL
	}
	else
	{
		SDL_Surface* screenshot = (SDL_Surface*)TCOD_opengl_get_screen();
		TCOD_sys_save_bitmap((void*)screenshot, filename);
		SDL_FreeSurface(screenshot);
#endif
	}
}


void TCOD_sys_sleep_milli(uint32 milliseconds)
{
	SDL_Delay(milliseconds);
}

void TCOD_sys_term()
{
	SDL_Quit();
#if SDL_VERSION_ATLEAST(2, 0, 0)
																															#   ifdef USE_SDL2_RENDERER
	renderer=NULL;
#   endif
	window=NULL;
#else
	screen = NULL;
#endif
}

void* TCOD_sys_load_image(const char* filename)
{
	image_support_t* img = image_type;
	while (img->extension != NULL && !img->check_type(filename))
	{ img++; }
	if (img->extension == NULL || img->read == NULL)
	{ return NULL; } /* unknown format */
	return img->read(filename);
}

void TCOD_sys_get_image_size(const void* image, int* w, int* h)
{
	SDL_Surface* surf = (SDL_Surface*)image;
	*w = surf->w;
	*h = surf->h;
}

TCOD_color_t TCOD_sys_get_image_pixel(const void* image, int x, int y)
{
	TCOD_color_t ret;
	SDL_Surface* surf = (SDL_Surface*)image;
	Uint8 bpp;
	Uint8* bits;
	if (x < 0 || y < 0 || x >= surf->w || y >= surf->h)
	{ return TCOD_black; }
	bpp = surf->format->BytesPerPixel;
	bits = ((Uint8*)surf->pixels) + y * surf->pitch + x * bpp;
	switch (bpp)
	{
	case 1 :
	{
		if (surf->format->palette)
		{
			SDL_Color col = surf->format->palette->colors[(*bits)];
			ret.r = col.r;
			ret.g = col.g;
			ret.b = col.b;
		}
		else
		{ return TCOD_black; }
	}
		break;
	default :
		ret.r = *((bits) + surf->format->Rshift / 8);
		ret.g = *((bits) + surf->format->Gshift / 8);
		ret.b = *((bits) + surf->format->Bshift / 8);
		break;
	}

	return ret;
}

uint32 TCOD_sys_elapsed_milli()
{
	return (uint32)SDL_GetTicks();
}

float TCOD_sys_elapsed_seconds()
{
	static float div = 1.0f / 1000.0f;
	return SDL_GetTicks() * div;
}

void TCOD_sys_get_char_size(int* w, int* h)
{
	*w = TCOD_ctx.font_width;
	*h = TCOD_ctx.font_height;
}

void TCOD_sys_get_current_resolution(int* w, int* h)
{
#if SDL_VERSION_ATLEAST(2, 0, 0)
																															int displayidx;
	SDL_Rect rect = { 0, 0, 0, 0 };
	if (window) {
		TCOD_IFNOT(window) return;
		displayidx = SDL_GetWindowDisplay(window);
		TCOD_IFNOT(displayidx >= 0) return;
	} else {
		/* No window if no console, but user can want to know res before opening one. */
		TCOD_IFNOT(SDL_GetNumVideoDisplays() > 0) return;
		displayidx = 0;
	}
	TCOD_IFNOT(SDL_GetDisplayBounds(displayidx, &rect) == 0) return;
	*w=rect.w;
	*h=rect.h;
#else
	const SDL_VideoInfo* info = SDL_GetVideoInfo();
	*w = info->current_w;
	*h = info->current_h;
#endif
}

/* image stuff */
bool TCOD_sys_check_magic_number(const char* filename, int size, uint8* data)
{
	uint8 tmp[128];
	int i;
#if SDL_VERSION_ATLEAST(2, 0, 0)
																															SDL_RWops *rwops =  SDL_RWFromFile(filename, "rb");
	if (! rwops) return false;
	if ( (i = rwops->read(rwops,tmp,size,1)) != 1 ) {
		rwops->close(rwops);
		return false;
	}
	rwops->close(rwops);
#else
	FILE* f = fopen(filename, "rb");
	if (!f)
	{ return false; }
	if (fread(tmp, 1, 128, f) < (unsigned)size)
	{
		fclose(f);
		return false;
	}
	fclose(f);
#endif
	for (i = 0; i < size; i++)
	{
		if (tmp[i] != data[i])
		{ return false; }
	}
	return true;
}

#if SDL_VERSION_ATLEAST(2, 0, 0)
																														void *TCOD_sys_get_sdl_window() {
	return (void *)window;
}
#endif

#if SDL_VERSION_ATLEAST(2, 0, 0)
																														void TCOD_mouse_includes_touch(bool enable) {
	mouse_touch = enable;
}
#endif

