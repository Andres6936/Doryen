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

#include "libtcod.hpp"
#include "libtcod.h"

Doryen::TCODImage::TCODImage(const char* filename) : deleteData(true)
{
	data = (void*)TCOD_image_load(filename);
}

Doryen::TCODImage::TCODImage(int width, int height) : deleteData(true)
{
	data = (void*)TCOD_image_new(width, height);
}

Doryen::TCODImage::TCODImage(const Doryen::Console* con)
{
	data=(void *)TCOD_image_from_console(con->data);
}

void Doryen::TCODImage::clear(const Doryen::Color col)
{
	TCOD_color_t ccol;
	ccol.r = col.r;
	ccol.g = col.g;
	ccol.b = col.b;
	TCOD_image_clear(data, ccol);
}

void Doryen::TCODImage::getSize(int* w, int* h) const
{
	TCOD_image_get_size(data, w, h);
}

Doryen::TCODImage::~TCODImage()
{
	if (deleteData)
	{ TCOD_image_delete(data); }
}

Doryen::Color Doryen::TCODImage::getPixel(int x, int y) const
{
	TCOD_color_t c = TCOD_image_get_pixel(data, x, y);
	return Doryen::Color(c.r, c.g, c.b);
}

int Doryen::TCODImage::getAlpha(int x, int y) const
{
	return TCOD_image_get_alpha(data, x, y);
}

Doryen::Color Doryen::TCODImage::getMipmapPixel(float x0, float y0, float x1, float y1)
{
	TCOD_color_t c = TCOD_image_get_mipmap_pixel(data, x0, y0, x1, y1);
	return Doryen::Color(c.r, c.g, c.b);
}

void Doryen::TCODImage::putPixel(int x, int y, const Doryen::Color col)
{
	TCOD_color_t ccol = { col.r, col.g, col.b };
	TCOD_image_put_pixel(data, x, y, ccol);
}

void
Doryen::TCODImage::blit(Doryen::Console* console, float x, float y, TCOD_bkgnd_flag_t bkgnd_flag, float scalex,
		float scaley,
		float angle) const
{
	TCOD_image_blit(data,console->data,x,y,bkgnd_flag,scalex,scaley,angle);
}

void
Doryen::TCODImage::blitRect(Doryen::Console* console, int x, int y, int w, int h, TCOD_bkgnd_flag_t bkgnd_flag) const
{
	TCOD_image_blit_rect(data, console->data, x, y, w, h, bkgnd_flag);
}

void Doryen::TCODImage::save(const char* filename) const
{
	TCOD_image_save(data, filename);
}

void Doryen::TCODImage::setKeyColor(const Doryen::Color keyColor)
{
	TCOD_color_t ccol = { keyColor.r, keyColor.g, keyColor.b };
	TCOD_image_set_key_color(data, ccol);
}

bool Doryen::TCODImage::isPixelTransparent(int x, int y) const
{
	return TCOD_image_is_pixel_transparent(data, x, y) != 0;
}

void Doryen::TCODImage::refreshConsole(const Doryen::Console* console)
{
	TCOD_image_refresh_console(data, console->data);
}

void Doryen::TCODImage::invert()
{
	TCOD_image_invert(data);
}

void Doryen::TCODImage::hflip()
{
	TCOD_image_hflip(data);
}

void Doryen::TCODImage::rotate90(int numRotations)
{
	TCOD_image_rotate90(data, numRotations);
}

void Doryen::TCODImage::vflip()
{
	TCOD_image_vflip(data);
}

void Doryen::TCODImage::scale(int neww, int newh)
{
	TCOD_image_scale(data, neww, newh);
}


void Doryen::TCODImage::blit2x(Doryen::Console* dest, int dx, int dy, int sx, int sy, int w, int h) const
{
	TCOD_image_blit_2x(data, dest->data, dx, dy, sx, sy, w, h);
}
