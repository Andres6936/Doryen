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

#include <Doryen/console.h>

typedef void* TCOD_image_t;

TCOD_image_t TCOD_image_new(int width, int height);

TCOD_image_t TCOD_image_from_console(TCOD_console_t console);

void TCOD_image_refresh_console(TCOD_image_t image, TCOD_console_t console);

TCOD_image_t TCOD_image_load(const char* filename);

void TCOD_image_clear(TCOD_image_t image, TCOD_color_t color);

void TCOD_image_invert(TCOD_image_t image);

void TCOD_image_hflip(TCOD_image_t image);

void TCOD_image_rotate90(TCOD_image_t image, int numRotations);

void TCOD_image_vflip(TCOD_image_t image);

void TCOD_image_save(TCOD_image_t image, const char *filename);

void TCOD_image_get_size(TCOD_image_t image, int *w,int *h);

TCOD_color_t TCOD_image_get_pixel(TCOD_image_t image,int x, int y);

int TCOD_image_get_alpha(TCOD_image_t image,int x, int y);

TCOD_color_t TCOD_image_get_mipmap_pixel(TCOD_image_t image,float x0,float y0, float x1, float y1);

void TCOD_image_put_pixel(TCOD_image_t image,int x, int y,TCOD_color_t col);

void TCOD_image_blit(TCOD_image_t image, TCOD_console_t console, float x, float y,
	TCOD_bkgnd_flag_t bkgnd_flag, float scalex, float scaley, float angle);

void TCOD_image_blit_2x(TCOD_image_t image, TCOD_console_t dest, int dx, int dy, int sx, int sy, int w, int h);

void TCOD_image_delete(TCOD_image_t image);

bool TCOD_image_is_pixel_transparent(TCOD_image_t image, int x, int y);

