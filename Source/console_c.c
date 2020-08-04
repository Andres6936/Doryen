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

void TCOD_fatal(const char *fmt, ...) {
	va_list ap;
	TCOD_sys_term();
	printf("%s\n",version_string);
	va_start(ap,fmt);
	vprintf(fmt,ap);
	va_end(ap);
	printf ("\n");
	exit (1);
}

void TCOD_fatal_nopar(const char *msg) {
	TCOD_sys_term();
	printf("%s\n%s\n",version_string,msg);
	exit (1);
}

void TCOD_console_set_window_closed() {
}

void TCOD_console_set_window_title(const char *title) {
	TCOD_sys_set_window_title(title);
}

void TCOD_console_set_fullscreen(bool fullscreen) {
	TCOD_IFNOT(TCOD_ctx.root != NULL) return;
	TCOD_sys_set_fullscreen(fullscreen);
	TCOD_ctx.fullscreen=fullscreen;
}


void TCOD_console_flush() {
	TCOD_console_data_t *dat=TCOD_ctx.root;
	TCOD_IFNOT(TCOD_ctx.root != NULL) return;
	TCOD_sys_flush(true);
	memcpy(dat->oldbuf,dat->buf,sizeof(char_t)*
		dat->w*dat->h);
}

void TCOD_console_set_fade(uint8 val, TCOD_color_t fadecol) {
	TCOD_ctx.fade=val;
	TCOD_ctx.fading_color=fadecol;
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

void TCOD_console_set_char_foreground(TCOD_console_t con,int x,int y, TCOD_color_t col) {
	TCOD_console_data_t *dat=con ? (TCOD_console_data_t *)con : TCOD_ctx.root;
	if ( (unsigned)(x) >= (unsigned)dat->w || (unsigned)(y) >= (unsigned)dat->h ) return;
	TCOD_IFNOT ( dat != NULL
		&& (unsigned)(x) < (unsigned)dat->w && (unsigned)(y) < (unsigned)dat->h )
		return;
	dat->buf[ y * dat->w + x ].fore=col;
}

TCOD_color_t TCOD_console_get_char_foreground(TCOD_console_t con,int x, int y) {
	TCOD_console_data_t *dat=con ? (TCOD_console_data_t *)con : TCOD_ctx.root;
	TCOD_IFNOT ( dat != NULL
		&& (unsigned)(x) < (unsigned)dat->w && (unsigned)(y) < (unsigned)dat->h )
		return TCOD_white;
	return dat->buf[ y * dat->w + x ].fore;
}

int TCOD_console_get_char(TCOD_console_t con,int x, int y) {
	TCOD_console_data_t *dat=con ? (TCOD_console_data_t *)con : TCOD_ctx.root;
	TCOD_IFNOT ( dat != NULL
		&& (unsigned)(x) < (unsigned)dat->w && (unsigned)(y) < (unsigned)dat->h )
		return 0;
	return dat->buf[ y * dat->w + x ].c;
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

TCOD_color_t TCOD_console_get_default_foreground(TCOD_console_t con) {
	TCOD_console_data_t *dat=con ? (TCOD_console_data_t *)con : TCOD_ctx.root;
	TCOD_IFNOT(dat != NULL) return TCOD_white;
	return dat->fore;
}

TCOD_color_t TCOD_console_get_default_background(TCOD_console_t con) {
	TCOD_console_data_t *dat=con ? (TCOD_console_data_t *)con : TCOD_ctx.root;
	TCOD_IFNOT(dat != NULL) return TCOD_black;
	return dat->back;
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

void TCOD_console_set_keyboard_repeat(int initial_delay, int interval) {
	TCOD_sys_set_keyboard_repeat(initial_delay,interval);
}

void TCOD_console_disable_keyboard_repeat() {
	TCOD_sys_set_keyboard_repeat(0,0);
}

bool TCOD_console_is_key_pressed(TCOD_keycode_t key)
{
	return TCOD_sys_is_key_pressed(key);
}

static bool init2 = false;

bool TCOD_console_credits_render(int x, int y, bool alpha)
{
	static char poweredby[128];
	static float char_heat[128];
	static int char_x[128];
	static int char_y[128];
	static bool init1 = false;
	static int len, len1, cw = -1, ch = -1;
	static float xstr;
	static TCOD_color_t colmap[64];
	static TCOD_color_t colmap_light[64];
//	static TCOD_noise_t noise;
	static TCOD_color_t colkeys[4] = {
			{ 255, 255, 204 },
			{ 255, 204, 0 },
			{ 255, 102, 0 },
			{ 102, 153, 255 },
	};
	static TCOD_color_t colkeys_light[4] = {
			{ 255, 255, 204 },
			{ 128, 128, 77 },
			{ 51,  51,  31 },
			{ 0,   0,   0 },
	};
	static int colpos[4] = {
			0, 21, 42, 63
	};
	static TCOD_image_t img = NULL;
	int i, xc, yc, xi, yi, j;
	static int left, right, top, bottom;
	float sparklex, sparkley, sparklerad, sparklerad2, noisex;
	/* mini particule system */
#define MAX_PARTICULES 50
	static float pheat[MAX_PARTICULES];
	static float px[MAX_PARTICULES], py[MAX_PARTICULES], pvx[MAX_PARTICULES], pvy[MAX_PARTICULES];
	static int nbpart = 0, firstpart = 0;
	static float partDelay = 0.1f;
	float elapsed = TCOD_sys_get_last_frame_length();
	TCOD_color_t fbackup; /* backup fg color */

	if (!init1)
	{
		/* initialize all static data, colormaps, ... */
		TCOD_color_t col;
		TCOD_color_gen_map(colmap, 4, colkeys, colpos);
		TCOD_color_gen_map(colmap_light, 4, colkeys_light, colpos);
		sprintf(poweredby, "Powered by\n%s", version_string);
//		noise = TCOD_noise_new(1, TCOD_NOISE_DEFAULT_HURST, TCOD_NOISE_DEFAULT_LACUNARITY, NULL);
		len = strlen(poweredby);
		len1 = 11; /* sizeof "Powered by\n" */
		left = MAX(x - 4, 0);
		top = MAX(y - 4, 0);
		col = TCOD_console_get_default_background(NULL);
		TCOD_console_set_default_background(NULL, TCOD_black);
		TCOD_console_set_default_background(NULL, col);
		init1 = true;
	}
	if (!init2)
	{
		/* reset the credits vars ... */
		int curx, cury;
		xstr = -4.0f;
		curx = x;
		cury = y;
		for (i = 0; i < len; i++)
		{
			char_heat[i] = -1;
			char_x[i] = curx;
			char_y[i] = cury;
			curx++;
			if (poweredby[i] == '\n')
			{
				curx = x;
				cury++;
			}
		}
		nbpart = firstpart = 0;
		init2 = true;
	}
	if (TCOD_console_get_width(NULL) != cw || TCOD_console_get_height(NULL) != ch)
	{
		/* console size has changed */
		int width, height;
		cw = TCOD_console_get_width(NULL);
		ch = TCOD_console_get_height(NULL);
		right = MIN(x + len, cw - 1);
		bottom = MIN(y + 6, ch - 1);
		width = right - left + 1;
		height = bottom - top + 1;
		if (img) TCOD_image_delete(img);
		img = TCOD_image_new(width * 2, height * 2);
	}
	fbackup = TCOD_console_get_default_foreground(NULL);
	if (xstr < (float)len1)
	{
		sparklex = x + xstr;
		sparkley = (float)y;
	}
	else
	{
		sparklex = x - len1 + xstr;
		sparkley = (float)y + 1;
	}
	noisex = xstr * 6;
//	sparklerad = 3.0f + 2 * TCOD_noise_get(noise, &noisex);
	if (xstr >= len - 1) sparklerad -= (xstr - len + 1) * 4.0f;
	else if (xstr < 0.0f) sparklerad += xstr * 4.0f;
	else if (poweredby[(int)(xstr + 0.5f)] == ' ' || poweredby[(int)(xstr + 0.5f)] == '\n') sparklerad /= 2;
	sparklerad2 = sparklerad * sparklerad * 4;

	/* draw the light */
	for (xc = left * 2, xi = 0; xc < (right + 1) * 2; xc++, xi++)
	{
		for (yc = top * 2, yi = 0; yc < (bottom + 1) * 2; yc++, yi++)
		{
			float dist = ((xc - 2 * sparklex) * (xc - 2 * sparklex) + (yc - 2 * sparkley) * (yc - 2 * sparkley));
			TCOD_color_t pixcol;
			if (sparklerad >= 0.0f && dist < sparklerad2)
			{
				int colidx = 63 - (int)(63 * (sparklerad2 - dist) / sparklerad2) + TCOD_random_get_int(NULL, -10, 10);
				colidx = CLAMP(0, 63, colidx);
				pixcol = colmap_light[colidx];
			}
			else
			{
				pixcol = TCOD_black;
			}
			if (alpha)
			{
				/*	console cells have following flag values :
				  		1 2
				  		4 8
				  	flag indicates which subcell uses foreground color */
				static int asciiToFlag[] = {
						1, /* TCOD_CHAR_SUBP_NW */
						2, /* TCOD_CHAR_SUBP_NE */
						3, /* TCOD_CHAR_SUBP_N */
						8, /* TCOD_CHAR_SUBP_SE */
						9, /* TCOD_CHAR_SUBP_DIAG */
						10, /* TCOD_CHAR_SUBP_E */
						4, /* TCOD_CHAR_SUBP_SW */
				};
				int conc = TCOD_console_get_char(NULL, xc / 2, yc / 2);
				TCOD_color_t bk = TCOD_console_get_char_background(NULL, xc / 2, yc / 2);
				if (conc >= TCOD_CHAR_SUBP_NW && conc <= TCOD_CHAR_SUBP_SW)
				{
					/* merge two subcell chars...
					   get the flag for the existing cell on root console */
					int bkflag = asciiToFlag[conc - TCOD_CHAR_SUBP_NW];
					int xflag = (xc & 1);
					int yflag = (yc & 1);
					/* get the flag for the current subcell */
					int credflag = (1 + 3 * yflag) * (xflag + 1);
					if ((credflag & bkflag) != 0)
					{
						/* the color for this subcell on root console
						   is foreground, not background */
						bk = TCOD_console_get_char_foreground(NULL, xc / 2, yc / 2);
					}
				}
				pixcol.r = MIN(255, (int)(bk.r) + pixcol.r);
				pixcol.g = MIN(255, (int)(bk.g) + pixcol.g);
				pixcol.b = MIN(255, (int)(bk.b) + pixcol.b);
			}
			TCOD_image_put_pixel(img, xi, yi, pixcol);
		}
	}

	/* draw and update the particules */
	j = nbpart;
	i = firstpart;
	while (j > 0)
	{
		int colidx = (int)(64 * (1.0f - pheat[i]));
		TCOD_color_t col;
		colidx = MIN(63, colidx);
		col = colmap[colidx];
		if ((int)py[i] < (bottom - top + 1) * 2)
		{
			int ipx = (int)px[i];
			int ipy = (int)py[i];
			float fpx = px[i] - ipx;
			float fpy = py[i] - ipy;
			TCOD_color_t col2 = TCOD_image_get_pixel(img, ipx, ipy);
			col2 = TCOD_color_lerp(col, col2, 0.5f * (fpx + fpy));
			TCOD_image_put_pixel(img, ipx, ipy, col2);
			col2 = TCOD_image_get_pixel(img, ipx + 1, ipy);
			col2 = TCOD_color_lerp(col2, col, fpx);
			TCOD_image_put_pixel(img, ipx + 1, ipy, col2);
			col2 = TCOD_image_get_pixel(img, ipx, ipy + 1);
			col2 = TCOD_color_lerp(col2, col, fpy);
			TCOD_image_put_pixel(img, ipx, ipy + 1, col2);
		}
		else pvy[i] = -pvy[i] * 0.5f;
		pvx[i] *= (1.0f - elapsed);
		pvy[i] += (1.0f - pheat[i]) * elapsed * 300.0f;
		px[i] += pvx[i] * elapsed;
		py[i] += pvy[i] * elapsed;
		pheat[i] -= elapsed * 0.3f;
		if (pheat[i] < 0.0f)
		{
			firstpart = (firstpart + 1) % MAX_PARTICULES;
			nbpart--;
		}
		i = (i + 1) % MAX_PARTICULES;
		j--;
	}
	partDelay -= elapsed;
	if (partDelay < 0.0f && nbpart < MAX_PARTICULES && sparklerad > 2.0f)
	{
		/* fire a new particule */
		int lastpart = firstpart;
		int nb = nbpart;
		while (nb > 0)
		{
			lastpart = (lastpart + 1) % MAX_PARTICULES;
			nb--;
		}
		nbpart++;
		px[lastpart] = 2 * (sparklex - left);
		py[lastpart] = 2 * (sparkley - top) + 2;
		pvx[lastpart] = TCOD_random_get_float(NULL, -5.0f, 5.0f);
		pvy[lastpart] = TCOD_random_get_float(NULL, -0.5f, -15.0f);
		pheat[lastpart] = 1.0f;
		partDelay += 0.1f;
	}
	TCOD_image_blit_2x(img, NULL, left, top, 0, 0, -1, -1);
	/* draw the text */
	for (i = 0; i < len; i++)
	{
		if (char_heat[i] >= 0.0f && poweredby[i] != '\n')
		{
			int colidx = (int)(64 * char_heat[i]);
			TCOD_color_t col;
			colidx = MIN(63, colidx);
			col = colmap[colidx];
			if (xstr >= len)
			{
				float coef = (xstr - len) / len;
				if (alpha)
				{
					TCOD_color_t fore = TCOD_console_get_char_background(NULL, char_x[i], char_y[i]);
					int r = (int)(coef * fore.r + (1.0f - coef) * col.r);
					int g = (int)(coef * fore.g + (1.0f - coef) * col.g);
					int b = (int)(coef * fore.b + (1.0f - coef) * col.b);
					col.r = CLAMP(0, 255, r);
					col.g = CLAMP(0, 255, g);
					col.b = CLAMP(0, 255, b);
					TCOD_console_set_char_foreground(NULL, char_x[i], char_y[i], col);
				}
				else
				{
					col = TCOD_color_lerp(col, TCOD_black, coef);
				}
			}
			TCOD_console_set_char(NULL, char_x[i], char_y[i], poweredby[i]);
			TCOD_console_set_char_foreground(NULL, char_x[i], char_y[i], col);
		}
	}
	/* update letters heat */
	xstr += elapsed * 4;
	for (i = 0; i < (int)(xstr + 0.5f); i++)
	{
		char_heat[i] = (xstr - i) / (len / 2);
	}
	/* restore fg color */
	TCOD_console_set_default_foreground(NULL, fbackup);
	if (xstr <= 2 * len) return false;
	init2 = false;
	return true;
}

void TCOD_console_credits()
{
	bool end = false;
	int x = TCOD_console_get_width(NULL) / 2 - 6;
	int y = TCOD_console_get_height(NULL) / 2;
	int fade = 260;
	TCOD_sys_save_fps();
	TCOD_sys_set_fps(25);
	while (!end)
	{
		TCOD_key_t k;
		end=TCOD_console_credits_render(x,y,false);
		TCOD_sys_check_for_event(TCOD_EVENT_KEY_PRESS,&k,NULL);
		if ( fade == 260 && k.vk != TCODK_NONE ) {
			fade -= 10;
		}
		TCOD_console_flush();
		if ( fade < 260 ) {
			fade -= 10;
			TCOD_console_set_fade(fade,TCOD_black);
			if ( fade == 0 ) end=true;
		}
	}
	TCOD_console_set_fade(255,TCOD_black);
	TCOD_sys_restore_fps();
}

void TCOD_console_credits_reset() {
	init2=false;
}
