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



