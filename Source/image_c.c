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



