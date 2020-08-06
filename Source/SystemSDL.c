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

void TCOD_sys_map_ascii_to_font(int asciiCode, int fontCharX, int fontCharY)
{
	if (asciiCode > 0 && asciiCode < TCOD_ctx.max_font_chars)
	{
		TCOD_ctx.ascii_to_tcod[asciiCode] = fontCharX + fontCharY * TCOD_ctx.fontNbCharHoriz;
	}
}

void TCOD_sys_load_font()
{
	int i;
	bool hasTransparent = false;
	int x, y;

	if (charmap)
	{ SDL_FreeSurface(charmap); }
	charmap = (TCOD_sys_load_image(TCOD_ctx.font_file));
	if (charmap == NULL)
	{ TCOD_fatal("SDL : cannot load %s", TCOD_ctx.font_file); }
	if ((float)(charmap->w / TCOD_ctx.fontNbCharHoriz) != charmap->w / TCOD_ctx.fontNbCharHoriz
		|| (float)(charmap->h / TCOD_ctx.fontNbCharVertic) != charmap->h / TCOD_ctx.fontNbCharVertic)
	{
		TCOD_fatal(" %s size is not a multiple of font layout (%dx%d)\n",
				TCOD_ctx.font_file, TCOD_ctx.fontNbCharHoriz, TCOD_ctx.fontNbCharVertic);
	}
	TCOD_ctx.font_width = charmap->w / TCOD_ctx.fontNbCharHoriz;
	TCOD_ctx.font_height = charmap->h / TCOD_ctx.fontNbCharVertic;
	/* allocated bool array for colored flags */
	if (TCOD_ctx.colored)
	{ free(TCOD_ctx.colored); }
	TCOD_ctx.colored = (bool*)calloc(sizeof(bool), TCOD_ctx.fontNbCharHoriz * TCOD_ctx.fontNbCharVertic);
	check_ascii_to_tcod();
	/* figure out what kind of font we have */
	/* check if the alpha layer is actually used */
	if (charmap->format->BytesPerPixel == 4)
	{
		printf("32bits font... checking for alpha layer... ");
		for (x = 0; !hasTransparent && x < charmap->w; x++)
		{
			for (y = 0; !hasTransparent && y < charmap->h; y++)
			{
				Uint8* pixel = (Uint8*)(charmap->pixels) + y * charmap->pitch + x * charmap->format->BytesPerPixel;
				Uint8 alpha = *((pixel) + charmap->format->Ashift / 8);
				if (alpha < 255)
				{
					hasTransparent = true;
				}
			}
		}
		printf(hasTransparent ? "present\n" : "not present\n");
	}
	else if (charmap->format->BytesPerPixel != 3)
	{
		/* convert to 24 bits */
		SDL_Surface* temp;
		printf("font bpp < 24. converting to 24bits\n");
		temp = (SDL_Surface*)TCOD_sys_get_surface(charmap->w, charmap->h, false);
		SDL_BlitSurface(charmap, NULL, temp, NULL);
		SDL_FreeSurface(charmap);
		charmap = temp;
	}
	else
	{
		printf("24 bits font.\n");
	}
	if (!hasTransparent)
	{
		/* alpha layer not used */
		int keyx, keyy;
		Uint8* pixel;
		/* the key color is found on the character corresponding to space ' ' */
		if (TCOD_ctx.font_tcod_layout)
		{
			keyx = TCOD_ctx.font_width / 2;
			keyy = TCOD_ctx.font_height / 2;
		}
		else if (TCOD_ctx.font_in_row)
		{
			keyx = ((int)(' ') % TCOD_ctx.fontNbCharHoriz) * TCOD_ctx.font_width + TCOD_ctx.font_width / 2;
			keyy = ((int)(' ') / TCOD_ctx.fontNbCharHoriz) * TCOD_ctx.font_height + TCOD_ctx.font_height / 2;
		}
		else
		{
			keyx = ((int)(' ') / TCOD_ctx.fontNbCharVertic) * TCOD_ctx.font_width + TCOD_ctx.font_width / 2;
			keyy = ((int)(' ') % TCOD_ctx.fontNbCharVertic) * TCOD_ctx.font_height + TCOD_ctx.font_height / 2;
		}
		pixel = (Uint8*)(charmap->pixels) + keyy * charmap->pitch + keyx * charmap->format->BytesPerPixel;
		fontKeyCol.r = *((pixel) + charmap->format->Rshift / 8);
		fontKeyCol.g = *((pixel) + charmap->format->Gshift / 8);
		fontKeyCol.b = *((pixel) + charmap->format->Bshift / 8);
		printf("key color : %d %d %d\n", fontKeyCol.r, fontKeyCol.g, fontKeyCol.b);
		if (!TCOD_ctx.font_greyscale && charmap->format->BytesPerPixel == 4)
		{
			/* 32 bits font but alpha layer not used. convert to 24 bits (faster) */
			SDL_Surface* temp;
			printf("32bits font with no alpha => converting to faster 24 bits\n");
			temp = (SDL_Surface*)TCOD_sys_get_surface(charmap->w, charmap->h, false);
			SDL_BlitSurface(charmap, NULL, temp, NULL);
			SDL_FreeSurface(charmap);
			charmap = temp;
		}
	}
	/* detect colored tiles */
	for (i = 0; i < TCOD_ctx.fontNbCharHoriz * TCOD_ctx.fontNbCharVertic; i++)
	{
		int px, py, cx, cy;
		bool end = false;
		cx = (i % TCOD_ctx.fontNbCharHoriz);
		cy = (i / TCOD_ctx.fontNbCharHoriz);
		for (px = 0; !end && px < TCOD_ctx.font_width; px++)
		{
			for (py = 0; !end && py < TCOD_ctx.font_height; py++)
			{
				Uint8* pixel = (Uint8*)(charmap->pixels) + (cy * TCOD_ctx.font_height + py) * charmap->pitch
							   + (cx * TCOD_ctx.font_width + px) * charmap->format->BytesPerPixel;
				Uint8 r = *((pixel) + charmap->format->Rshift / 8);
				Uint8 g = *((pixel) + charmap->format->Gshift / 8);
				Uint8 b = *((pixel) + charmap->format->Bshift / 8);
				if (charmap->format->BytesPerPixel == 3)
				{
					/* ignore key color */
					if (r == fontKeyCol.r && g == fontKeyCol.g && b == fontKeyCol.b)
					{ continue; }
				}
				/* colored tile if a pixel is not desaturated */
				if (r != g || g != b || b != r)
				{
					TCOD_ctx.colored[i] = true;
					printf("character for ascii code %d is colored\n", i);
					end = true;
				}
			}
		}
	}
	/* convert 24/32 bits greyscale to 32bits font with alpha layer */
	if (!hasTransparent && TCOD_ctx.font_greyscale)
	{
		bool invert = (fontKeyCol.r > 128); /* black on white font ? */
		/* convert the surface to 32 bits if needed */
		if (charmap->format->BytesPerPixel != 4)
		{
			SDL_Surface* temp;
			printf("24bits greyscale font. converting to 32bits\n");
			temp = (SDL_Surface*)TCOD_sys_get_surface(charmap->w, charmap->h, true);
			SDL_BlitSurface(charmap, NULL, temp, NULL);
			SDL_FreeSurface(charmap);
			charmap = temp;
		}
		for (i = 0; i < TCOD_ctx.fontNbCharHoriz * TCOD_ctx.fontNbCharVertic; i++)
		{
			int cx, cy;
			cx = (i % TCOD_ctx.fontNbCharHoriz);
			cy = (i / TCOD_ctx.fontNbCharHoriz);
			/* fill the surface with white (except colored tiles), use alpha layer for characters */
			for (x = cx * TCOD_ctx.font_width; x < (cx + 1) * TCOD_ctx.font_width; x++)
			{
				for (y = cy * TCOD_ctx.font_height; y < (cy + 1) * TCOD_ctx.font_height; y++)
				{
					if (!TCOD_ctx.colored[i])
					{
						Uint8* pixel =
								(Uint8*)(charmap->pixels) + y * charmap->pitch + x * charmap->format->BytesPerPixel;
						Uint8 r = *((pixel) + charmap->format->Rshift / 8);
						*((pixel) + charmap->format->Ashift / 8) = (invert ? 255 - r : r);
						*((pixel) + charmap->format->Rshift / 8) = 255;
						*((pixel) + charmap->format->Gshift / 8) = 255;
						*((pixel) + charmap->format->Bshift / 8) = 255;
					}
					else
					{
						Uint8* pixel =
								(Uint8*)(charmap->pixels) + y * charmap->pitch + x * charmap->format->BytesPerPixel;
						Uint8 r = *((pixel) + charmap->format->Rshift / 8);
						Uint8 g = *((pixel) + charmap->format->Gshift / 8);
						Uint8 b = *((pixel) + charmap->format->Bshift / 8);
						if (r == fontKeyCol.r && g == fontKeyCol.g && b == fontKeyCol.b)
						{
							*((pixel) + charmap->format->Ashift / 8) = 0;
						}
						else
						{
							*((pixel) + charmap->format->Ashift / 8) = 255;
						}
					}
				}
			}
		}
	}
	sdl_key = SDL_MapRGB(charmap->format, fontKeyCol.r, fontKeyCol.g, fontKeyCol.b);
	rgb_mask = charmap->format->Rmask | charmap->format->Gmask | charmap->format->Bmask;
	nrgb_mask = ~rgb_mask;
	sdl_key &= rgb_mask; /* remove the alpha part */
#if SDL_VERSION_ATLEAST(2, 0, 0)
	if ( charmap->format->BytesPerPixel == 3 ) SDL_SetColorKey(charmap,SDL_TRUE|SDL_RLEACCEL,sdl_key);
#else
	if (charmap->format->BytesPerPixel == 3)
	{ SDL_SetColorKey(charmap, SDL_SRCCOLORKEY | SDL_RLEACCEL, sdl_key); }
#endif
	for (i = 0; i < TCOD_ctx.fontNbCharHoriz * TCOD_ctx.fontNbCharVertic; i++)
	{
		charcols[i] = fontKeyCol;
		first_draw[i] = true;
	}
	check_ascii_to_tcod();
	if (!TCOD_ctx.font_tcod_layout)
	{
		/* apply standard ascii mapping */
		if (TCOD_ctx.font_in_row)
		{
			/* for font in row */
			for (i = 0; i < TCOD_ctx.max_font_chars; i++)
			{ TCOD_ctx.ascii_to_tcod[i] = i; }
		}
		else
		{
			/* for font in column */
			for (i = 0; i < TCOD_ctx.max_font_chars; i++)
			{
				int fy = i % TCOD_ctx.fontNbCharVertic;
				int fx = i / TCOD_ctx.fontNbCharVertic;
				TCOD_ctx.ascii_to_tcod[i] = fx + fy * TCOD_ctx.fontNbCharHoriz;
			}
		}
	}
}

static void find_resolution()
{
#if SDL_VERSION_ATLEAST(2, 0, 0)
	SDL_DisplayMode wantedmode, closestmode;
#else
	SDL_Rect** modes;
	int i;
#endif
	int bestw, besth, wantedw, wantedh;

	if (TCOD_ctx.fullscreen_width > TCOD_ctx.root->w * TCOD_ctx.font_width)
	{
		wantedw = TCOD_ctx.fullscreen_width;
	}
	else
	{
		wantedw = TCOD_ctx.root->w * TCOD_ctx.font_width;
	}

	if (TCOD_ctx.fullscreen_height > TCOD_ctx.root->h * TCOD_ctx.font_height)
	{
		wantedh = TCOD_ctx.fullscreen_height;
	}
	else
	{
		wantedh = TCOD_ctx.root->h * TCOD_ctx.font_height;
	}

	TCOD_ctx.actual_fullscreen_width = wantedw;
	TCOD_ctx.actual_fullscreen_height = wantedh;

#if SDL_VERSION_ATLEAST(2, 0, 0)
																															wantedmode.w = wantedw;
	wantedmode.h = wantedh;
	wantedmode.format = 0;  /* don't care for rest. */
	wantedmode.refresh_rate = 0;
	wantedmode.driverdata = 0;
	if (SDL_GetClosestDisplayMode(window?SDL_GetWindowDisplay(window):0, &wantedmode, &closestmode) == &closestmode) {
		bestw=closestmode.w;
		besth=closestmode.h;
	} else {
		bestw=99999;
		besth=99999;
	}
#else
	modes = SDL_ListModes(NULL, SDL_FULLSCREEN);

	bestw = 99999;
	besth = 99999;
	if (modes != (SDL_Rect**)0 && modes != (SDL_Rect**)-1)
	{
		for (i = 0; modes[i]; ++i)
		{
			if (modes[i]->w >= wantedw && modes[i]->w <= bestw
				&& modes[i]->h >= wantedh && modes[i]->h <= besth
				&& SDL_VideoModeOK(modes[i]->w, modes[i]->h, 32, SDL_FULLSCREEN))
			{
				bestw = modes[i]->w;
				besth = modes[i]->h;
			}
		}
	}
#endif
	if (bestw != 99999)
	{
		TCOD_ctx.actual_fullscreen_width = bestw;
		TCOD_ctx.actual_fullscreen_height = besth;
	}
}

void* TCOD_sys_create_bitmap_for_console(TCOD_console_t console)
{
	int w, h;
	w = TCOD_console_get_width(console) * TCOD_ctx.font_width;
	h = TCOD_console_get_height(console) * TCOD_ctx.font_height;
	return TCOD_sys_get_surface(w, h, false);
}

static void TCOD_sys_render(void* vbitmap, int console_width, int console_height, char_t* console_buffer,
		char_t* prev_console_buffer)
{
#if SDL_VERSION_ATLEAST(2, 0, 0) && defined(USE_SDL2_RENDERER)
	SDL_Texture *screentexture;
#endif
	if (TCOD_ctx.renderer == TCOD_RENDERER_SDL)
	{
		TCOD_sys_console_to_bitmap(vbitmap, console_width, console_height, console_buffer, prev_console_buffer);
		if (TCOD_ctx.sdl_cbk)
		{
#if SDL_VERSION_ATLEAST(2, 0, 0)
																																	#	if defined (USE_SDL2_RENDERER)
			printf("TCOD_sys_render call to renderer unsupported yet, in SDL2\n");
#	else
			TCOD_ctx.sdl_cbk((void *)SDL_GetWindowSurface(window));
#	endif
#else
			TCOD_ctx.sdl_cbk((void*)screen);
#endif
		}
#if SDL_VERSION_ATLEAST(2, 0, 0)
																																#   ifdef USE_SDL2_RENDERER
		screentexture = SDL_CreateTextureFromSurface(renderer, screen);
		SDL_RenderCopy(renderer, screentexture, NULL, NULL);
		SDL_RenderPresent(renderer);
		SDL_DestroyTexture(screentexture);
#   else
		SDL_UpdateWindowSurface(window);
#   endif
#else
		SDL_Flip(screen);
#endif
	}
#ifndef NO_OPENGL
	else
	{
		TCOD_opengl_render(oldFade, ascii_updated, console_buffer, prev_console_buffer);
		TCOD_opengl_swap();
	}
#endif
	oldFade = (int)TCOD_console_get_fade();
	if (any_ascii_updated)
	{
		memset(ascii_updated, 0, sizeof(bool) * TCOD_ctx.max_font_chars);
		any_ascii_updated = false;
	}
}

void TCOD_sys_console_to_bitmap(void* vbitmap, int console_width, int console_height, char_t* console_buffer,
		char_t* prev_console_buffer)
{
	static SDL_Surface* charmap_backup = NULL;
	int x, y;
	SDL_Surface* bitmap = (SDL_Surface*)vbitmap;
	Uint32 sdl_back = 0, sdl_fore = 0;
	TCOD_color_t fading_color = TCOD_console_get_fading_color();
	int fade = (int)TCOD_console_get_fade();
	bool track_changes = (oldFade == fade && prev_console_buffer);
	Uint8 bpp = charmap->format->BytesPerPixel;

	char_t* character = &console_buffer[0];
	char_t* previuosCharacter = &prev_console_buffer[0];

	int hdelta;
	if (bpp == 4)
	{
		hdelta = (charmap->pitch - TCOD_ctx.font_width * bpp) / 4;
	}
	else
	{
		hdelta = (charmap->pitch - TCOD_ctx.font_width * bpp);
	}
	if (charmap_backup == NULL)
	{
		charmap_backup = (SDL_Surface*)TCOD_sys_get_surface(charmap->w, charmap->h, true);
		SDL_BlitSurface(charmap, NULL, charmap_backup, NULL);
	}
#ifdef USE_SDL_LOCKS
	if ( SDL_MUSTLOCK( bitmap ) && SDL_LockSurface( bitmap ) < 0 ) return;
#endif
#if SDL_VERSION_ATLEAST(2, 0, 0)
																															#   ifdef USE_SDL2_RENDERER
	if (bitmap == NULL)
		bitmap = screen;
#   else
	if (bitmap == NULL)
		bitmap = SDL_GetWindowSurface(window);
#   endif
#endif
	for (y = 0; y < console_height; y++)
	{
		for (x = 0; x < console_width; x++)
		{
			SDL_Rect srcRect, dstRect;
			bool changed = true;
			if (character->cf == -1)
			{ character->cf = TCOD_ctx.ascii_to_tcod[character->c]; }
			if (track_changes)
			{
				changed = false;
				if (character->dirt || ascii_updated[character->c] || character->back.r != previuosCharacter->back.r ||
					character->back.g != previuosCharacter->back.g
					|| character->back.b != previuosCharacter->back.b || character->fore.r != previuosCharacter->fore.r
					|| character->fore.g != previuosCharacter->fore.g || character->fore.b != previuosCharacter->fore.b
					|| character->c != previuosCharacter->c || character->cf != previuosCharacter->cf)
				{
					changed = true;
				}
			}
			character->dirt = 0;
			if (changed)
			{
				TCOD_color_t b = character->back;
				dstRect.x = x * TCOD_ctx.font_width;
				dstRect.y = y * TCOD_ctx.font_height;
				dstRect.w = TCOD_ctx.font_width;
				dstRect.h = TCOD_ctx.font_height;
				/* draw background */
				if (fade != 255)
				{
					b.r = ((int)b.r) * fade / 255 + ((int)fading_color.r) * (255 - fade) / 255;
					b.g = ((int)b.g) * fade / 255 + ((int)fading_color.g) * (255 - fade) / 255;
					b.b = ((int)b.b) * fade / 255 + ((int)fading_color.b) * (255 - fade) / 255;
				}
				sdl_back = SDL_MapRGB(bitmap->format, b.r, b.g, b.b);
#if SDL_VERSION_ATLEAST(2, 0, 0)
				if ( vbitmap == NULL && TCOD_ctx.fullscreen ) {
#else
				if (bitmap == screen && TCOD_ctx.fullscreen)
				{
#endif
					dstRect.x += TCOD_ctx.fullscreen_offsetx;
					dstRect.y += TCOD_ctx.fullscreen_offsety;
				}
				SDL_FillRect(bitmap, &dstRect, sdl_back);
				if (character->c != ' ')
				{
					/* draw foreground */
					int ascii = character->cf;
					TCOD_color_t* curtext = &charcols[ascii];
					bool first = first_draw[ascii];
					TCOD_color_t f = character->fore;

					if (fade != 255)
					{
						f.r = ((int)f.r) * fade / 255 + ((int)fading_color.r) * (255 - fade) / 255;
						f.g = ((int)f.g) * fade / 255 + ((int)fading_color.g) * (255 - fade) / 255;
						f.b = ((int)f.b) * fade / 255 + ((int)fading_color.b) * (255 - fade) / 255;
					}
					/* only draw character if foreground color != background color */
					if (ascii_updated[character->c] || f.r != b.r || f.g != b.g || f.b != b.b)
					{
						if (charmap->format->Amask == 0
							&& f.r == fontKeyCol.r && f.g == fontKeyCol.g && f.b == fontKeyCol.b)
						{
							/* cannot draw with the key color... */
							if (f.r < 255)
							{ f.r++; }
							else
							{ f.r--; }
						}
						srcRect.x = (ascii % TCOD_ctx.fontNbCharHoriz) * TCOD_ctx.font_width;
						srcRect.y = (ascii / TCOD_ctx.fontNbCharHoriz) * TCOD_ctx.font_height;
						srcRect.w = TCOD_ctx.font_width;
						srcRect.h = TCOD_ctx.font_height;

						if (charmap && (first || curtext->r != f.r || curtext->g != f.g || curtext->b != f.b))
						{
							/* change the character color in the font */
							first_draw[ascii] = false;
							sdl_fore = SDL_MapRGB(charmap->format, f.r, f.g, f.b) & rgb_mask;
							*curtext = f;
#ifdef USE_SDL_LOCKS
																																					if ( SDL_MUSTLOCK(charmap) ) {
								if ( SDL_LockSurface(charmap) < 0 ) return;
							}
#endif
							if (bpp == 4)
							{
								/* 32 bits font : fill the whole character with foreground color */
								Uint32* pix = (Uint32*)(((Uint8*)charmap->pixels) + srcRect.x * bpp +
														srcRect.y * charmap->pitch);
								int h = TCOD_ctx.font_height;
								if (!TCOD_ctx.colored[ascii])
								{
									while (h--)
									{
										int w = TCOD_ctx.font_width;
										while (w--)
										{
											(*pix) &= nrgb_mask;
											(*pix) |= sdl_fore;
											pix++;
										}
										pix += hdelta;
									}
								}
								else
								{
									/* colored character : multiply color with foreground color */
									Uint32* pixorig = (Uint32*)(((Uint8*)charmap_backup->pixels) + srcRect.x * bpp +
																srcRect.y * charmap_backup->pitch);
									int hdelta_backup = (charmap_backup->pitch - TCOD_ctx.font_width * 4) / 4;
									while (h > 0)
									{
										int w = TCOD_ctx.font_width;
										while (w > 0)
										{
											int r = (int)(*((Uint8*)(pixorig) + charmap_backup->format->Rshift / 8));
											int g = (int)(*((Uint8*)(pixorig) + charmap_backup->format->Gshift / 8));
											int b = (int)(*((Uint8*)(pixorig) + charmap_backup->format->Bshift / 8));
											(*pix) &= nrgb_mask; /* erase the color */
											r = r * f.r / 255;
											g = g * f.g / 255;
											b = b * f.b / 255;
											/* set the new color */
											(*pix) |= (r << charmap->format->Rshift) | (g << charmap->format->Gshift) |
													  (b << charmap->format->Bshift);
											w--;
											pix++;
											pixorig++;
										}
										h--;
										pix += hdelta;
										pixorig += hdelta_backup;
									}
								}
							}
							else
							{
								/* 24 bits font : fill only non key color pixels */
								Uint32* pix = (Uint32*)(((Uint8*)charmap->pixels) + srcRect.x * bpp +
														srcRect.y * charmap->pitch);
								int h = TCOD_ctx.font_height;
								if (!TCOD_ctx.colored[ascii])
								{
									while (h--)
									{
										int w = TCOD_ctx.font_width;
										while (w--)
										{
											if (((*pix) & rgb_mask) != sdl_key)
											{
												(*pix) &= nrgb_mask;
												(*pix) |= sdl_fore;
											}
											pix = (Uint32*)(((Uint8*)pix) + 3);
										}
										pix = (Uint32*)(((Uint8*)pix) + hdelta);
									}
								}
								else
								{
									/* colored character : multiply color with foreground color */
									Uint32* pixorig = (Uint32*)(((Uint8*)charmap_backup->pixels) + srcRect.x * 4 +
																srcRect.y * charmap_backup->pitch);
									/* charmap_backup is always 32 bits */
									int hdelta_backup = (charmap_backup->pitch - TCOD_ctx.font_width * 4) / 4;
									while (h > 0)
									{
										int w = TCOD_ctx.font_width;
										while (w > 0)
										{
											if (((*pixorig) & rgb_mask) != sdl_key)
											{
												int r = (int)(*((Uint8*)(pixorig) +
																charmap_backup->format->Rshift / 8));
												int g = (int)(*((Uint8*)(pixorig) +
																charmap_backup->format->Gshift / 8));
												int b = (int)(*((Uint8*)(pixorig) +
																charmap_backup->format->Bshift / 8));
												(*pix) &= nrgb_mask; /* erase the color */
												r = r * f.r / 255;
												g = g * f.g / 255;
												b = b * f.b / 255;
												/* set the new color */
												(*pix) |= (r << charmap->format->Rshift) |
														  (g << charmap->format->Gshift) |
														  (b << charmap->format->Bshift);
											}
											w--;
											pix = (Uint32*)(((Uint8*)pix) + 3);
											pixorig++;
										}
										h--;
										pix = (Uint32*)(((Uint8*)pix) + hdelta);
										pixorig += hdelta_backup;
									}
								}
							}
#ifdef USE_SDL_LOCKS
																																					if ( SDL_MUSTLOCK(charmap) ) {
								SDL_UnlockSurface(charmap);
							}
#endif
						}
						SDL_BlitSurface(charmap, &srcRect, bitmap, &dstRect);
					}
				}
			}
			character++;
			previuosCharacter++;
		}
	}
#ifdef USE_SDL_LOCKS
	if ( SDL_MUSTLOCK( bitmap ) ) SDL_UnlockSurface( bitmap );
#endif
}

void TCOD_sys_set_keyboard_repeat(int initial_delay, int interval)
{
#if !SDL_VERSION_ATLEAST(2, 0, 0)
	SDL_EnableKeyRepeat(initial_delay, interval);
#endif
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

void TCOD_sys_update_char(int asciiCode, int fontx, int fonty, TCOD_image_t img, int x, int y)
{
	int px, py;
	int iw, ih;
	static TCOD_color_t pink = { 255, 0, 255 };
	TCOD_sys_map_ascii_to_font(asciiCode, fontx, fonty);
	TCOD_image_get_size(img, &iw, &ih);
	for (px = 0; px < TCOD_ctx.font_width; px++)
	{
		for (py = 0; py < TCOD_ctx.font_height; py++)
		{
			TCOD_color_t col = TCOD_white;
			Uint8* pixel;
			Uint8 bpp;
			if ((unsigned)(x + px) < (unsigned)iw && (unsigned)(y + py) < (unsigned)ih)
			{
				col = TCOD_image_get_pixel(img, x + px, y + py);
			}
			pixel = (Uint8*)(charmap->pixels) + (fonty * TCOD_ctx.font_height + py) * charmap->pitch +
					(fontx * TCOD_ctx.font_width + px) * charmap->format->BytesPerPixel;
			bpp = charmap->format->BytesPerPixel;
			if (bpp == 4)
			{
				*((pixel) + charmap->format->Ashift / 8) = col.r;
				*((pixel) + charmap->format->Rshift / 8) = 255;
				*((pixel) + charmap->format->Gshift / 8) = 255;
				*((pixel) + charmap->format->Bshift / 8) = 255;
			}
			else
			{
				*((pixel) + charmap->format->Rshift / 8) = col.r;
				*((pixel) + charmap->format->Gshift / 8) = col.g;
				*((pixel) + charmap->format->Bshift / 8) = col.b;
			}
		}
	}
	/* TODO : improve this. */
	charcols[asciiCode] = pink;
	ascii_updated[asciiCode] = true;
	any_ascii_updated = true;
}

#ifdef TCOD_MACOSX
void CustomSDLMain();
#endif

void TCOD_sys_startup()
{
	if (has_startup)
	{ return; }
#ifdef TCOD_MACOSX
		CustomSDLMain();
#endif
#if SDL_VERSION_ATLEAST(2, 0, 0)
																																#ifndef NDEBUG
	SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);
#endif
#endif
	TCOD_IFNOT(SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO) >= 0)
	{ return; }
#ifndef    TCOD_WINDOWS
	/* not needed and might crash on windows */
	atexit(SDL_Quit);
#endif
#if !SDL_VERSION_ATLEAST(2, 0, 0)
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
#endif
	TCOD_ctx.max_font_chars = 256;
	alloc_ascii_tables();
#ifndef NO_OPENGL
	TCOD_opengl_init_attributes();
#endif

	has_startup = true;
}

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

