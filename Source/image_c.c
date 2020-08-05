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

static void TCOD_image_generate_mip(image_data_t *img, int mip) {
	mipmap_t *orig=&img->mipmaps[0];
	mipmap_t *cur =&img->mipmaps[mip];
	int x,y;
	if (! cur->buf) {
		cur->buf=(TCOD_color_t *)calloc(sizeof(TCOD_color_t),cur->width*cur->height);
	}
	cur->dirty=false;
	for (x=0; x < cur->width; x++) {
		for (y=0; y < cur->height; y++) {
			int r=0,g=0,b=0, count=0;
			int sx,sy;
			TCOD_color_t *col;
			for (sx=(x << mip); sx < ((x+1)<<mip); sx ++) {
				for (sy=(y << mip); sy < ((y+1)<<mip); sy ++) {
					int offset=sx+sy*orig->width;
					count++;
					r+=orig->buf[offset].r;
					g+=orig->buf[offset].g;
					b+=orig->buf[offset].b;
				}
			}
			r /= count;
			g /= count;
			b /= count;
			col = &cur->buf[x+y*cur->width];
			col->r=r;
			col->g=g;
			col->b=b;
		}
	}
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

TCOD_color_t TCOD_image_get_mipmap_pixel(TCOD_image_t image,float x0,float y0, float x1, float y1) {
	int texel_xsize, texel_ysize, texel_size, texel_x, texel_y;
	int cur_size = 1;
	int mip = 0;
	image_data_t* img = (image_data_t*)image;
	if (!img->mipmaps && !img->sys_img) return TCOD_black; /* no image data */
	if (!img->mipmaps) TCOD_image_init_mipmaps(img);
	texel_xsize = (int)(x1 - x0);
	texel_ysize = (int)(y1 - y0);

	if (texel_xsize < texel_ysize)
	{
		texel_size = texel_ysize;
	}
	else
	{
		texel_size = texel_xsize;
	}

	while (mip < img->nb_mipmaps - 1 && cur_size < texel_size)
	{
		mip++;
		cur_size <<= 1;
	}
	if (mip > 0) mip--;
	texel_x = (int)(x0 * (img->mipmaps[mip].width) / img->mipmaps[0].fwidth);
	texel_y = (int)(y0 * (img->mipmaps[mip].height) / img->mipmaps[0].fheight);

	if (img->mipmaps[mip].buf == NULL || img->mipmaps[mip].dirty)
	{
		TCOD_image_generate_mip(img,mip);
	}
	if ( texel_x < 0 || texel_y < 0 || texel_x >= img->mipmaps[mip].width || texel_y >= img->mipmaps[mip].height )
		return TCOD_black;
	return img->mipmaps[mip].buf[texel_x+texel_y*img->mipmaps[mip].width];
}

void TCOD_image_put_pixel(TCOD_image_t image,int x, int y,TCOD_color_t col) {
	image_data_t *img=(image_data_t *)image;
	if ( !img->mipmaps && !img->sys_img) return; /* no image data */
	if ( ! img->mipmaps ) {
		TCOD_image_init_mipmaps(img);
	}
	if ( x >= 0 && x < img->mipmaps[0].width
		&& y >= 0 && y < img->mipmaps[0].height ) {
		int mip;
		img->mipmaps[0].buf[x+y*img->mipmaps[0].width] = col;
		for (mip=1; mip < img->nb_mipmaps; mip++) {
			img->mipmaps[mip].dirty=true;
		}
	}
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

void TCOD_image_hflip(TCOD_image_t image) {
	int px,py;
	int width,height;
	TCOD_image_get_size(image,&width,&height);
	for (py = 0; py < height; py++ ) {
		for (px = 0; px < width/2; px++ ) {
			TCOD_color_t col1=TCOD_image_get_pixel(image,px,py);
			TCOD_color_t col2=TCOD_image_get_pixel(image,width-1-px,py);
			TCOD_image_put_pixel(image,px,py,col2);
			TCOD_image_put_pixel(image,width-1-px,py,col1);
		}
	}
}

void TCOD_image_vflip(TCOD_image_t image) {
	int px,py;
	int width,height;
	TCOD_image_get_size(image,&width,&height);
	for (px = 0; px < width; px++ ) {
		for (py = 0; py < height/2; py++ ) {
			TCOD_color_t col1=TCOD_image_get_pixel(image,px,py);
			TCOD_color_t col2=TCOD_image_get_pixel(image,px,height-1-py);
			TCOD_image_put_pixel(image,px,py,col2);
			TCOD_image_put_pixel(image,px,height-1-py,col1);
		}
	}
}

void TCOD_image_rotate90(TCOD_image_t image, int numRotations) {
	int px,py;
	int width,height;
	numRotations = numRotations % 4;
	if (numRotations == 0 ) return;
	if ( numRotations < 0 ) numRotations += 4;
	TCOD_image_get_size(image,&width,&height);
	if (numRotations == 1) {
		/* rotate 90 degrees */
		TCOD_image_t newImg=TCOD_image_new(height,width);
		image_data_t *img=(image_data_t *)image;
		image_data_t *img2=(image_data_t *)newImg;
		for (px = 0; px < width; px++ ) {
			for (py = 0; py < height; py++ ) {
				TCOD_color_t col1=TCOD_image_get_pixel(image,px,py);
				TCOD_image_put_pixel(newImg,height-1-py,px,col1);
			}
		}
		TCOD_image_delete_internal(image);
		/* update img with the new image content */
		img->mipmaps = img2->mipmaps;
		img->sys_img=NULL;
		img->nb_mipmaps=img2->nb_mipmaps;
		free(img2);
	} else if ( numRotations == 2 ) {
		/* rotate 180 degrees */
		int maxy=height/2 + ((height & 1) == 1? 1 : 0 );
		for (px = 0; px < width; px++ ) {
			for (py = 0; py < maxy; py++ ) {
				if ( py != height-1-py || px < width/2 ) {
					TCOD_color_t col1=TCOD_image_get_pixel(image,px,py);
					TCOD_color_t col2=TCOD_image_get_pixel(image,width-1-px,height-1-py);
					TCOD_image_put_pixel(image,px,py,col2);
					TCOD_image_put_pixel(image,width-1-px,height-1-py,col1);
				}
			}
		}
	} else if (numRotations == 3) {
		/* rotate 270 degrees */
		TCOD_image_t newImg=TCOD_image_new(height,width);
		image_data_t *img=(image_data_t *)image;
		image_data_t *img2=(image_data_t *)newImg;
		for (px = 0; px < width; px++ ) {
			for (py = 0; py < height; py++ ) {
				TCOD_color_t col1=TCOD_image_get_pixel(image,px,py);
				TCOD_image_put_pixel(newImg,py,width-1-px,col1);
			}
		}
		TCOD_image_delete_internal(image);
		/* update img with the new image content */
		img->mipmaps = img2->mipmaps;
		img->sys_img=NULL;
		img->nb_mipmaps=img2->nb_mipmaps;
		free(img2);
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

void TCOD_image_blit_2x(TCOD_image_t image, TCOD_console_t con, int dx, int dy, int sx, int sy, int w, int h) {
	TCOD_color_t grid[4];
	TCOD_color_t cols[2];
	int nbCols;
	int width,height,ascii,cx,cy;
	TCOD_console_data_t *dat = con ? (TCOD_console_data_t *)(con) : TCOD_ctx.root;
	image_data_t *img=(image_data_t *)image;
	int maxx,maxy;
	TCOD_IFNOT(image != NULL && dat != NULL) return;

	TCOD_image_get_size(image,&width,&height);
	if ( w == -1 ) w=width;
	if ( h == -1 ) h=height;

	/* check that the sx,sy/w,h rectangle is inside the image */
	TCOD_ASSERT(sx >= 0 && sy >= 0 && sx+w <= width && sy+h <= height);
	TCOD_IFNOT(w > 0 && h > 0) return;

	sx=MAX(0,sx);
	sy=MAX(0,sy);
	w = MIN(w,width-sx);
	h = MIN(h,height-sy);

	maxx=dx+w/2 <= dat->w ? w : (dat->w-dx)*2;
	maxy=dy+h/2 <= dat->h ? h : (dat->h-dy)*2;
	/* check that the image is not blitted outside the console */
	TCOD_IFNOT(dx+maxx/2 >= 0 && dy+maxy/2 >= 0 && dx < dat->w && dy < dat->h) return;
	maxx+=sx;
	maxy+=sy;

	for (cx=sx; cx < maxx; cx += 2) {
		for (cy=sy; cy < maxy; cy += 2) {
			/* get the 2x2 super pixel colors from the image */
			int conx=dx+(cx-sx)/2;
			int cony=dy+(cy-sy)/2;
			TCOD_color_t consoleBack=TCOD_console_get_char_background(con,conx,cony);
			grid[0]=TCOD_image_get_pixel(image,cx,cy);
			if ( img->has_key_color && grid[0].r == img->key_color.r  && grid[0].g == img->key_color.g && grid[0].b == img->key_color.b)
				grid[0]=consoleBack;
			if ( cx < maxx-1 ) {
				grid[1]=TCOD_image_get_pixel(image,cx+1,cy);
				if ( img->has_key_color && grid[1].r == img->key_color.r  && grid[1].g == img->key_color.g && grid[1].b == img->key_color.b)
					grid[1]=consoleBack;
			} else grid[1]=consoleBack;
			if ( cy < maxy-1 ) {
				grid[2]=TCOD_image_get_pixel(image,cx,cy+1);
				if ( img->has_key_color && grid[2].r == img->key_color.r  && grid[2].g == img->key_color.g && grid[2].b == img->key_color.b)
					grid[2]=consoleBack;
			} else grid[2]=consoleBack;
			if ( cx < maxx-1 && cy < maxy-1 ) {
				grid[3]=TCOD_image_get_pixel(image,cx+1,cy+1);
				if ( img->has_key_color && grid[3].r == img->key_color.r  && grid[3].g == img->key_color.g && grid[3].b == img->key_color.b)
					grid[3]=consoleBack;
			} else grid[3]=consoleBack;
			/* analyse color, posterize, get pattern */
			getPattern(grid,cols,&nbCols,&ascii);
			if ( nbCols == 1 ) {
				/* single color */
				TCOD_console_set_char_background(con,conx,cony,cols[0],TCOD_BKGND_SET);
				TCOD_console_set_char(con,conx,cony,' ');
			} else {
				if ( ascii >= 0 ) {
					TCOD_console_set_default_background(con,cols[0]);
					TCOD_console_set_default_foreground(con,cols[1]);
					TCOD_console_put_char(con,conx,cony,ascii,TCOD_BKGND_SET);
				} else {
					/* negative ascii code means we need to invert back/fore colors */
					TCOD_console_set_default_background(con,cols[1]);
					TCOD_console_set_default_foreground(con,cols[0]);
					TCOD_console_put_char(con,conx,cony,-ascii,TCOD_BKGND_SET);
				}
			}
		}
	}
}



