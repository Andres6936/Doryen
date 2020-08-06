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

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "Doryen/console.h"
#include "Doryen/libtcod.h"
#include "Doryen/libtcod_int.h"

#ifndef NO_UNICODE

#include <wchar.h>

#endif

#if defined( TCOD_VISUAL_STUDIO )
static const char *version_string ="libtcod "TCOD_STRVERSION;
#else
static const char* version_string __attribute__((unused)) = "libtcod "TCOD_STRVERSION;
#endif

TCOD_internal_context_t TCOD_ctx={
	/* number of characters in the bitmap font */
	16, 16,
	/* font type and layout */
	false, false, false,
	/* character size in font */
	8, 8,
    "Terminal.png", "",
    NULL, NULL, NULL, 0, false, 0, 0, 0, 0, 0, 0,
	/* default renderer to use */
	TCOD_RENDERER_GLSL,
    NULL,
	/* fading data */
	{0,0,0}, 255,
};

void TCOD_fatal_nopar(const char *msg) {
	TCOD_sys_term();
	printf("%s\n%s\n",version_string,msg);
	exit (1);
}


TCOD_color_t TCOD_console_get_char_background(TCOD_console_t con,int x, int y) {
	TCOD_console_data_t *dat=con ? (TCOD_console_data_t *)con : TCOD_ctx.root;
	TCOD_IFNOT ( dat != NULL
		&& (unsigned)(x) < (unsigned)dat->w && (unsigned)(y) < (unsigned)dat->h )
		return TCOD_black;
	return dat->buf[ y * dat->w + x ].back;
}

void TCOD_console_set_char(TCOD_console_t con,int x, int y, int c) {
	TCOD_console_data_t *dat=con ? (TCOD_console_data_t *)con : TCOD_ctx.root;
	if ( (unsigned)(x) >= (unsigned)dat->w || (unsigned)(y) >= (unsigned)dat->h ) return;
	dat->buf[ y * dat->w + x ].c=c;
	dat->buf[ y * dat->w + x ].cf = TCOD_ctx.ascii_to_tcod[c];
}


void TCOD_console_set_default_foreground(TCOD_console_t con,TCOD_color_t col) {
	TCOD_console_data_t *dat=con ? (TCOD_console_data_t *)con : TCOD_ctx.root;
	TCOD_IFNOT(dat != NULL) return;
	dat->fore=col;
}

void TCOD_console_set_default_background(TCOD_console_t con,TCOD_color_t col) {
	TCOD_console_data_t *dat=con ? (TCOD_console_data_t *)con : TCOD_ctx.root;
	TCOD_IFNOT(dat != NULL) return;
	dat->back=col;
}

int TCOD_console_get_width(TCOD_console_t con) {
	TCOD_console_data_t *dat=con ? (TCOD_console_data_t *)con : TCOD_ctx.root;
	TCOD_IFNOT(dat != NULL) return 0;
	return dat->w;
}

int TCOD_console_get_height(TCOD_console_t con) {
	TCOD_console_data_t *dat=con ? (TCOD_console_data_t *)con : TCOD_ctx.root;
	TCOD_IFNOT(dat != NULL) return 0;
	return dat->h;
}

char_t *TCOD_console_get_buf(TCOD_console_t con) {
	TCOD_console_data_t *dat=con ? (TCOD_console_data_t *)con : TCOD_ctx.root;
	TCOD_IFNOT(dat != NULL) return NULL;
	return dat->buf;
}

