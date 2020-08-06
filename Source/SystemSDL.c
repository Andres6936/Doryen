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
#include <SDL/SDL.h>
#include "Doryen/libtcod_int.h"

static SDL_Surface* screen = NULL;
static char vk_to_c[SDLK_LAST];

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

void TCOD_sys_get_image_size(const void* image, int* w, int* h)
{
	SDL_Surface* surf = (SDL_Surface*)image;
	*w = surf->w;
	*h = surf->h;
}

float TCOD_sys_elapsed_seconds()
{
	static float div = 1.0f / 1000.0f;
	return SDL_GetTicks() * div;
}
