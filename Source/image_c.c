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
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

#include "Doryen/libtcod.h"
#include "Doryen/libtcod_int.h"

typedef struct
{
	int width, height;
	float fwidth, fheight;
	TCOD_color_t* buf;
	bool dirty;
} mipmap_t;

typedef struct
{
	void* sys_img;
	int nb_mipmaps;
	mipmap_t *mipmaps;
	TCOD_color_t key_color;
	bool has_key_color;
} image_data_t;

static int TCOD_image_get_mipmap_levels(int width, int height) {
	int curw=width;
	int curh=height;
	int nb_mipmap=0;
	while ( curw > 0 && curh > 0 ) {
		nb_mipmap++;
		curw >>= 1;
		curh >>= 1;
	}
	return nb_mipmap;
}

static void TCOD_image_init_mipmaps(image_data_t *img) {
	int w,h,i,x,y;
	float fw,fh;
	if (! img->sys_img ) return;
	TCOD_sys_get_image_size(img->sys_img,&w,&h);
	img->nb_mipmaps=TCOD_image_get_mipmap_levels(w,h);
	img->mipmaps = (mipmap_t *)calloc(sizeof(mipmap_t),img->nb_mipmaps);
	img->mipmaps[0].buf = (TCOD_color_t *)calloc(sizeof(TCOD_color_t),w*h);
	for (x=0; x < w; x++) {
		for (y=0;y < h; y++) {
			img->mipmaps[0].buf[x+y*w]=TCOD_sys_get_image_pixel(img->sys_img,x,y);
		}
	}
	fw=(float)w;
	fh=(float)h;
	for ( i=0; i < img->nb_mipmaps; i++) {
		img->mipmaps[i].width=w;
		img->mipmaps[i].height=h;
		img->mipmaps[i].fwidth=fw;
		img->mipmaps[i].fheight=fh;
		img->mipmaps[i].dirty=true;
		w >>= 1;
		h >>= 1;
		fw *= 0.5f;
		fh *= 0.5f;
	}
	img->mipmaps[0].dirty=false;
}

TCOD_image_t TCOD_image_new(int width, int height) {
	int i;
	float fw,fh;
	image_data_t *ret=(image_data_t *)calloc(sizeof(image_data_t),1);
	ret->nb_mipmaps=TCOD_image_get_mipmap_levels(width,height);
	ret->mipmaps = (mipmap_t *)calloc(sizeof(mipmap_t),ret->nb_mipmaps);
	ret->mipmaps[0].buf = (TCOD_color_t *)calloc(sizeof(TCOD_color_t),width*height);

	for (i=0; i< width*height; i++) {
		ret->mipmaps[0].buf[i] = TCOD_black;
	}
	fw=(float)width;
	fh=(float)height;
	for ( i=0; i < ret->nb_mipmaps; i++) {
		ret->mipmaps[i].width=width;
		ret->mipmaps[i].height=height;
		ret->mipmaps[i].fwidth=fw;
		ret->mipmaps[i].fheight=fh;
		width >>= 1;
		height >>= 1;
		fw *= 0.5f;
		fh *= 0.5f;
	}
	return (TCOD_image_t)ret;
}

TCOD_image_t TCOD_image_load(const char *filename) {
	image_data_t *ret=(image_data_t *)calloc(sizeof(image_data_t),1);
	ret->sys_img=TCOD_sys_load_image(filename);
	return (TCOD_image_t)ret;
}

void TCOD_image_get_size(TCOD_image_t image, int *w,int *h) {
	image_data_t *img=(image_data_t *)image;
	if ( !img->mipmaps && !img->sys_img) return; /* no image data */
	if ( img->mipmaps ) {
		*w = img->mipmaps[0].width;
		*h = img->mipmaps[0].height;
	} else {
		TCOD_sys_get_image_size(img->sys_img,w,h);
	}
}

TCOD_color_t TCOD_image_get_pixel(TCOD_image_t image,int x, int y) {
	image_data_t *img=(image_data_t *)image;
	if ( !img->mipmaps && !img->sys_img) return TCOD_black; /* no image data */
	if ( img->mipmaps ) {
		if ( x >= 0 && x < img->mipmaps[0].width
			&& y >= 0 && y < img->mipmaps[0].height ) {
			return img->mipmaps[0].buf[x+y*img->mipmaps[0].width];
		} else {
			return TCOD_black;
		}
	} else {
		return TCOD_sys_get_image_pixel(img->sys_img,x,y);
	}
}

int TCOD_image_get_alpha(TCOD_image_t image,int x, int y) {
	image_data_t *img=(image_data_t *)image;
	if ( img->sys_img ) {
		return TCOD_sys_get_image_alpha(img->sys_img,x,y);
	} else return 255;
}

void TCOD_image_delete_internal(TCOD_image_t image) {
	image_data_t *img=(image_data_t *)image;
	if ( img->mipmaps ) {
		int i;
		for ( i=0; i < img->nb_mipmaps; i++) {
			if ( img->mipmaps[i].buf ) free(img->mipmaps[i].buf);
		}
		free(img->mipmaps);
	}
	if ( img->sys_img ) {
		TCOD_sys_delete_bitmap(img->sys_img);
	}
}

void TCOD_image_delete(TCOD_image_t image) {
	TCOD_image_delete_internal(image);
	free(image);
}

bool TCOD_image_is_pixel_transparent(TCOD_image_t image, int x, int y) {
	image_data_t *img=(image_data_t *)image;
	TCOD_color_t col=TCOD_image_get_pixel(image,x,y);
	if ( img->has_key_color && img->key_color.r == col.r
		&& img->key_color.g == col.g && img->key_color.b == col.b ) {
		return true;
	}
	if ( TCOD_image_get_alpha(image,x,y) == 0 ) return true;
	return false;
}

void TCOD_image_refresh_console(TCOD_image_t image, TCOD_console_t console) {
	image_data_t *img=(image_data_t *)image;
	TCOD_sys_console_to_bitmap(img->sys_img, TCOD_console_get_width(console), TCOD_console_get_height(console),
		TCOD_console_get_buf(console),NULL);
}

void TCOD_image_save(TCOD_image_t image, const char *filename) {
	image_data_t *img=(image_data_t *)image;
	void *bitmap=NULL;
	bool must_free=false;
	if ( img->sys_img ) {
		bitmap=img->sys_img;
	} else if ( img->mipmaps ){
		bitmap=TCOD_sys_create_bitmap(img->mipmaps[0].width, img->mipmaps[0].height, img->mipmaps[0].buf);
		must_free=true;
	}
	if (bitmap) {
		TCOD_sys_save_bitmap(bitmap, filename);
		if ( must_free ) {
			TCOD_sys_delete_bitmap(bitmap);
		}
	}
}

void TCOD_image_invert(TCOD_image_t image) {
	int i,mip;
	int width,height;
	image_data_t *img=(image_data_t *)image;
	if ( !img->mipmaps && !img->sys_img) return; /* no image data */
	if ( ! img->mipmaps ) {
		TCOD_image_init_mipmaps(img);
	}
	TCOD_image_get_size(image,&width,&height);
	for (i=0; i< width*height; i++) {
		TCOD_color_t col=img->mipmaps[0].buf[i];
		col.r=255-col.r;
		col.g=255-col.g;
		col.b=255-col.b;
		img->mipmaps[0].buf[i] = col;
	}
	for (mip=1; mip < img->nb_mipmaps; mip++) {
		img->mipmaps[mip].dirty=true;
	}
}


/* distance between two colors */
int rgbdist(const TCOD_color_t *c1,const TCOD_color_t *c2) {
	int dr=(int)(c1->r)-c2->r;
	int dg=(int)(c1->g)-c2->g;
	int db=(int)(c1->b)-c2->b;
	return dr*dr+dg*dg+db*db;
}

void getPattern(TCOD_color_t desired[4], TCOD_color_t palette[2], int *nbCols, int *ascii) {
	/* adapted from Jeff Lait's code posted on r.g.r.d */
	int flag=0;
	/*
		pixels have following flag values :
			X 1
			2 4
		flag indicates which pixels uses foreground color (palette[1])
	*/
	static int flagToAscii[8] = {
			0,
			TCOD_CHAR_SUBP_NE, TCOD_CHAR_SUBP_SW, -TCOD_CHAR_SUBP_DIAG, TCOD_CHAR_SUBP_SE,
			TCOD_CHAR_SUBP_E, -TCOD_CHAR_SUBP_N, -TCOD_CHAR_SUBP_NW
	};
	int weight[2] = { 0, 0 };
	int counterColorEquals;

	/* First colour trivial. */
	palette[0] = desired[0];

	/* Ignore all duplicates... */
	for (counterColorEquals = 1; counterColorEquals < 4; counterColorEquals++)
	{
		if (desired[counterColorEquals].r != palette[0].r || desired[counterColorEquals].g != palette[0].g ||
			desired[counterColorEquals].b != palette[0].b)
			break;
	}

	/* All the same. */
	if (counterColorEquals == 4)
	{
		*nbCols = 1;
		return;
	}
	weight[0] = counterColorEquals;

	/* Found a second colour... */
	palette[1] = desired[counterColorEquals];
	weight[1] = 1;
	flag |= 1 << (counterColorEquals - 1);
	*nbCols = 2;
	/* remaining colours */
	counterColorEquals++;
	while (counterColorEquals < 4)
	{
		if (desired[counterColorEquals].r == palette[0].r && desired[counterColorEquals].g == palette[0].g &&
			desired[counterColorEquals].b == palette[0].b)
		{
			weight[0]++;
		}
		else if (desired[counterColorEquals].r == palette[1].r && desired[counterColorEquals].g == palette[1].g &&
				 desired[counterColorEquals].b == palette[1].b)
		{
			flag |= 1 << (counterColorEquals - 1);
			weight[1]++;
		}
		else
		{
			/* Bah, too many colours, */
			/* merge the two nearest */
			int dist0i = rgbdist(&desired[counterColorEquals], &palette[0]);
			int dist1i = rgbdist(&desired[counterColorEquals], &palette[1]);
			int dist01 = rgbdist(&palette[0], &palette[1]);

			if (dist0i < dist1i)
			{
				if (dist0i <= dist01)
				{
					/* merge 0 and i */
					palette[0] = TCOD_color_lerp(desired[counterColorEquals], palette[0],
							weight[0] / (1.0f + weight[0]));
					weight[0]++;
				}
				else
				{
					/* merge 0 and 1 */
					palette[0] = TCOD_color_lerp(palette[0], palette[1], (float)(weight[1]) / (weight[0] + weight[1]));
					weight[0]++;
					palette[1] = desired[counterColorEquals];
					flag = 1 << (counterColorEquals - 1);
				}
			} else {
				if ( dist1i <= dist01 )
				{
					/* merge 1 and i */
					palette[1] = TCOD_color_lerp(desired[counterColorEquals], palette[1],
							weight[1] / (1.0f + weight[1]));
					weight[1]++;
					flag |= 1 << (counterColorEquals - 1);
				}
				else
				{
					/* merge 0 and 1 */
					palette[0] = TCOD_color_lerp(palette[0], palette[1], (float)(weight[1]) / (weight[0] + weight[1]));
					weight[0]++;
					palette[1] = desired[counterColorEquals];
					flag = 1 << (counterColorEquals - 1);
				}
			}
		}
		counterColorEquals++;
	}
	*ascii=flagToAscii[flag];
}



