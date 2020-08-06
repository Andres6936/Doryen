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

void TCOD_console_set_window_closed() {
}


uint8 TCOD_console_get_fade() {
	return TCOD_ctx.fade;
}

TCOD_color_t TCOD_console_get_fading_color() {
	return TCOD_ctx.fading_color;
}


void TCOD_console_put_char(TCOD_console_t con,int x, int y, int c, TCOD_bkgnd_flag_t flag) {
	int offset;
	TCOD_console_data_t *dat=con ? (TCOD_console_data_t *)con : TCOD_ctx.root;
	TCOD_IFNOT ( dat != NULL && (unsigned)(x) < (unsigned)dat->w && (unsigned)(y) < (unsigned)dat->h ) return;
	TCOD_IFNOT (c >= 0 && c < TCOD_ctx.max_font_chars ) return;
	offset = y * dat->w + x;
	dat->buf[ offset ].c = c;
	dat->buf[ offset ].cf = TCOD_ctx.ascii_to_tcod[c];
	dat->buf[ offset ].fore = dat->fore;
	TCOD_console_set_char_background(con,x,y,dat->back,(TCOD_bkgnd_flag_t)flag);
}

TCOD_color_t TCOD_console_get_char_background(TCOD_console_t con,int x, int y) {
	TCOD_console_data_t *dat=con ? (TCOD_console_data_t *)con : TCOD_ctx.root;
	TCOD_IFNOT ( dat != NULL
		&& (unsigned)(x) < (unsigned)dat->w && (unsigned)(y) < (unsigned)dat->h )
		return TCOD_black;
	return dat->buf[ y * dat->w + x ].back;
}

void TCOD_console_set_char_background(TCOD_console_t con,int x, int y, TCOD_color_t col, TCOD_bkgnd_flag_t flag) {
	TCOD_color_t *back;
	int newr,newg,newb;
	int alpha;
	TCOD_console_data_t *dat=con ? (TCOD_console_data_t *)con : TCOD_ctx.root;
	TCOD_IFNOT ( dat != NULL
		&& (unsigned)(x) < (unsigned)dat->w && (unsigned)(y) < (unsigned)dat->h )
		return;
	back=&dat->buf[y*dat->w+x].back;
	if ( flag == TCOD_BKGND_DEFAULT ) flag = dat->bkgnd_flag;
	switch ( flag & 0xff ) {
		case TCOD_BKGND_SET : *back = col; break;
		case TCOD_BKGND_MULTIPLY  : *back= TCOD_color_multiply(*back, col); break;
		case TCOD_BKGND_LIGHTEN :
			back->r = MAX(back->r,col.r);
			back->g = MAX(back->g,col.g);
			back->b = MAX(back->b,col.b);
		break;
		case TCOD_BKGND_DARKEN :
			back->r = MIN(back->r,col.r);
			back->g = MIN(back->g,col.g);
			back->b = MIN(back->b,col.b);
		break;
		case TCOD_BKGND_SCREEN :
			/* newbk = white - (white - oldbk) * (white - curbk) */
			back->r = (uint8)(255 - (int)(255 - back->r)*(255 - col.r)/255);
			back->g = (uint8)(255 - (int)(255 - back->g)*(255 - col.g)/255);
			back->b = (uint8)(255 - (int)(255 - back->b)*(255 - col.b)/255);
		break;
		case TCOD_BKGND_COLOR_DODGE :
			/* newbk = curbk / (white - oldbk) */
			if ( back->r != 255 ) newr = (int)(255 * col.r) / (255 - back->r);
			else newr=255;
			if ( back->g != 255 ) newg = (int)(255 * col.g) / (255 - back->g);
			else newg=255;
			if ( back->b != 255 ) newb = (int)(255 * col.b) / (255 - back->b);
			else newb=255;
			back->r=(uint8)CLAMP(0,255,newr);
			back->g=(uint8)CLAMP(0,255,newg);
			back->b=(uint8)CLAMP(0,255,newb);
		break;
		case TCOD_BKGND_COLOR_BURN :
			/* newbk = white - (white - oldbk) / curbk */
			if ( col.r > 0 ) newr = 255 - (int)(255 * (255 - back->r) )/col.r;
			else newr = 0;
			if ( col.g > 0 ) newg = 255 - (int)(255 * (255 - back->g)) /col.g;
			else newg = 0;
			if ( col.b > 0 ) newb = 255 - (int)(255 * (255 - back->b)) /col.b;
			else newb = 0;
			back->r=(uint8)CLAMP(0,255,newr);
			back->g=(uint8)CLAMP(0,255,newg);
			back->b=(uint8)CLAMP(0,255,newb);
		break;
		case TCOD_BKGND_ADD :
			/* newbk = oldbk + curbk */
			newr=(int)(back->r)+col.r;
			newg=(int)(back->g)+col.g;
			newb=(int)(back->b)+col.b;
			back->r=(uint8)CLAMP(0,255,newr);
			back->g=(uint8)CLAMP(0,255,newg);
			back->b=(uint8)CLAMP(0,255,newb);
		break;
		case TCOD_BKGND_ADDA :
			alpha=(flag >> 8);
			/* newbk = oldbk + alpha * curbk */
			newr=(int)(back->r)+alpha * col.r / 255;
			newg=(int)(back->g)+alpha * col.g / 255;
			newb=(int)(back->b)+alpha * col.b / 255;
			back->r=(uint8)CLAMP(0,255,newr);
			back->g=(uint8)CLAMP(0,255,newg);
			back->b=(uint8)CLAMP(0,255,newb);
		break;
		case TCOD_BKGND_BURN :
			/* newbk = oldbk + curbk - white */
			newr=(int)(back->r)+col.r-255;
			newg=(int)(back->g)+col.g-255;
			newb=(int)(back->b)+col.b-255;
			back->r=(uint8)CLAMP(0,255,newr);
			back->g=(uint8)CLAMP(0,255,newg);
			back->b=(uint8)CLAMP(0,255,newb);
		break;
		case TCOD_BKGND_OVERLAY :
			/* newbk = curbk.x <= 0.5 ? 2*curbk*oldbk : white - 2*(white-curbk)*(white-oldbk) */
			newr = col.r <= 128 ? 2 * (int)(col.r) * back->r / 255 : 255 - 2*(int)(255 - col.r)*(255-back->r)/255;
			newg = col.g <= 128 ? 2 * (int)(col.g) * back->g / 255 : 255 - 2*(int)(255 - col.g)*(255-back->g)/255;
			newb = col.b <= 128 ? 2 * (int)(col.b) * back->b / 255 : 255 - 2*(int)(255 - col.b)*(255-back->b)/255;
			back->r=(uint8)CLAMP(0,255,newr);
			back->g=(uint8)CLAMP(0,255,newg);
			back->b=(uint8)CLAMP(0,255,newb);
		break;
		case TCOD_BKGND_ALPH :
			/* newbk = (1.0f-alpha)*oldbk + alpha*(curbk-oldbk) */
			alpha=(flag >> 8);
			*back = TCOD_color_lerp(*back,col,(float)(alpha/255.0f));
		break;
		default : break;
	}
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

