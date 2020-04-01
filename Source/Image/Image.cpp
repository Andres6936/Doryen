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
#include "libtcod.h"

#include "Image/Image.hpp"
#include "Image/ImageData.hpp"

#include <cmath>

using namespace Doryen;

Image::Image(int width, int height) : deleteData(true)
{
	data = (void*)TCOD_image_new(width, height);

	imageData = ImageData(width, height);
}

Image::Image(const char* filename) : deleteData(true)
{
	data = (void*)TCOD_image_load(filename);

	imageData = ImageData(filename);
}

Image::Image(const Console& console)
{
//	data = (void*)TCOD_image_from_console(console->data);

	imageData = ImageData();
	imageData.createBitmapFrom(console);
}

void Image::clear(const Color col)
{
	TCOD_color_t ccol;
	ccol.r = col.r;
	ccol.g = col.g;
	ccol.b = col.b;
	TCOD_image_clear(data, ccol);
}

void Image::getSize(int* w, int* h) const
{
	TCOD_image_get_size(data, w, h);
}

Size Image::getSize() const
{
	return imageData.getSize();
}

Image::~Image()
{
	if (deleteData)
	{ TCOD_image_delete(data); }
}

Color Image::getPixel(int x, int y) const
{
	return imageData.getPixel(x, y);
}

int Image::getAlpha(int x, int y) const
{
	return imageData.getAlpha(x, y);
}

Color Image::getMipmapPixel(float x0, float y0, float x1, float y1)
{
	TCOD_color_t c = TCOD_image_get_mipmap_pixel(data, x0, y0, x1, y1);
	return Color(c.r, c.g, c.b);
}

void Image::putPixel(int x, int y, const Color col)
{
	TCOD_color_t ccol = { col.r, col.g, col.b };
	TCOD_image_put_pixel(data, x, y, ccol);
}

void
Image::blit(Console* console, float x, float y, TCOD_bkgnd_flag_t bkgnd_flag, float scalex,
		float scaley,
		float angle) const
{
	TCOD_image_blit(data,console->data,x,y,bkgnd_flag,scalex,scaley,angle);
}

void
Image::blitRect(Console* console, int x, int y, int w, int h, TCOD_bkgnd_flag_t bkgnd_flag) const
{
	TCOD_image_blit_rect(data, console->data, x, y, w, h, bkgnd_flag);
}

void Image::save(const char* filename) const
{
	TCOD_image_save(data, filename);
}

void Image::setKeyColor(const Color keyColor)
{
	TCOD_color_t ccol = { keyColor.r, keyColor.g, keyColor.b };
	TCOD_image_set_key_color(data, ccol);
}

bool Image::isPixelTransparent(int x, int y) const
{
	return TCOD_image_is_pixel_transparent(data, x, y) != 0;
}

void Image::refreshConsole(const Console* console)
{
	TCOD_image_refresh_console(data, console->data);
}

void Image::invert()
{
	TCOD_image_invert(data);
}

void Image::hflip()
{
	TCOD_image_hflip(data);
}

void Image::rotate90(int numRotations)
{
	TCOD_image_rotate90(data, numRotations);
}

void Image::vflip()
{
	TCOD_image_vflip(data);
}


void Image::scale(int neww, int newh)
{
	TCOD_image_scale(data, neww, newh);
}

void Image::blit2x(Console* dest, int dx, int dy, int sx, int sy, int w, int h) const
{
	TCOD_image_blit_2x(data, dest->data, dx, dy, sx, sy, w, h);
}

void
Image::blit(Console& _console,
		const Point& _center,
		BackgroundFlag _flag,
		const float scaleX,
		const float scaleY,
		const float angle) const
{
	if (scaleX == 0.0f or scaleY == 0.0f or _flag == BackgroundFlag::NONE) return;

	// Size of the image
	Size size = imageData.getSize();

	if (scaleX == 1.0f and scaleY == 1.0f and angle == 0.0f)
	{
		// Clip the image
		int ix = (int)(_center.x - size.w * 0.5f);
		int iy = (int)(_center.y - size.h * 0.5f);

		int minX = std::max(ix, 0);
		int minY = std::max(iy, 0);

		int maxX = std::min(ix + size.w, (int)_console.getWidth());
		int maxY = std::min(iy + size.h, (int)_console.getHeight());

		int offX = 0;
		int offY = 0;

		if (ix < 0) offX = -ix;
		if (iy < 0) offY = -iy;

		for (int cx = minX; cx < maxX; cx++)
		{
			for (int cy = minY; cy < maxY; cy++)
			{
				Color color = imageData.getPixel(cx - minX + offX, cy - minY + offY);

				if (not imageData.isHasKeyColor() or not imageData.getKeyColor().equals(color))
				{
					_console.setCharBackground(cx, cy, color, _flag);
				}
			}
		}
	}
	else
	{
		float iw = (float)size.w / 2 * scaleX;
		float ih = (float)size.h / 2 * scaleY;

		// Get the coordinates of the image corners in the console
		float newXX = std::cos(angle);
		float newXY = -std::sin(angle);

		float newYX = newXY;
		float newYY = -newXX;

		// Point of floats
		using Pointf = Geometry::Point2D<float>;

		Pointf corner0;

		corner0.x = _center.x - iw * newXX + ih * newYX;
		corner0.y = _center.y - iw * newXY + ih * newYY;

		Pointf corner1;

		corner1.x = _center.x + iw * newXX + ih * newYX;
		corner1.y = _center.y + iw * newXY + ih * newYY;

		Pointf corner2;

		corner2.x = _center.x + iw * newXX - ih * newYX;
		corner2.y = _center.y + iw * newXY - ih * newYY;

		Pointf corner3;

		corner3.x = _center.x - iw * newXX - ih * newYX;
		corner3.y = _center.y - iw * newXY - ih * newYY;

		int rx = std::min(std::min(corner0.x, corner1.x), std::min(corner2.x, corner3.x));
		int ry = std::min(std::min(corner0.y, corner1.y), std::min(corner2.y, corner3.y));
		int rw = (int)std::max(std::max(corner0.x, corner1.x), std::max(corner2.x, corner3.x)) - rx;
		int rh = (int)std::max(std::max(corner0.y, corner1.y), std::max(corner2.y, corner3.y)) - ry;

		int cx;
		int cy;

		Point min;

		min.x = std::max(rx, 0);
		min.y = std::max(ry, 0);

		Point max;

		max.x = std::min(rx + rw, (int)_console.getWidth());
		max.y = std::min(ry + rh, (int)_console.getHeight());

		float invScaleX = 1.0f / scaleX;
		float invScaleY = 1.0f / scaleY;

		// TODO: Implement
	}
}
