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
#include "Doryen/libtcod.h"

#include "Doryen/Image/Image.hpp"
#include "Doryen/Image/ImageData.hpp"

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
	imageData = ImageData();
	imageData.createBitmapFrom(console);
}

Geometry::Size Image::getSize() const
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

void
Image::blitRect(Console& console, int x, int y, int w, int h, BackgroundFlag flag)
{
	const auto[width, height] = getSize();

	if (w == -1) w = width;
	if (h == -1) h = height;

	// Not satisfied invariants
	if (w <= 0 or h <= 0 || flag == BackgroundFlag::NONE) return;

	const float scaleX = w / width;
	const float scaleY = h / height;

	const int X = static_cast<int>(x + w * 0.5f);
	const int Y = static_cast<int>(y + h * 0.5f);

	blit(console, { X, Y }, flag, scaleX, scaleY, 0.0f);
}

void Image::save(const char* filename) const
{
	TCOD_image_save(data, filename);
}

void Image::setKeyColor(const Color& keyColor)
{
	imageData.setKeyColor(keyColor);
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

void Image::flippingHorizontally()
{
	const auto[width, height] = getSize();

	for (int py = 0; py < height; ++py)
	{
		for (int px = 0; px < width / 2; ++px)
		{
			const Color col1 = getPixel(px, py);
			const Color col2 = getPixel(width - 1 - px, py);

			setPixel(px, py, col2);
			setPixel(width - 1 - px, py, col1);
		}
	}
}

void Image::rotate90(int numRotations)
{
	TCOD_image_rotate90(data, numRotations);
}

void Image::vflip()
{
	TCOD_image_vflip(data);
}

float Image::calculateFractionalEdge(Color& _color,
		const Color& leftTop, const float _weightLeft,
		const Color& rightBottom, const float _weightRight) const
{
	_color.r += leftTop.r * _weightLeft + rightBottom.r * _weightRight;
	_color.g += leftTop.g * _weightLeft + rightBottom.g * _weightRight;
	_color.b += leftTop.b * _weightLeft + rightBottom.b * _weightRight;

	return _weightLeft + _weightRight;
}

float Image::calculateCorners(Color& _color, const Color& pixel,
		const float weightLeft, const float weightRight)
{
	_color.r += pixel.r * (weightLeft * weightRight);
	_color.g += pixel.g * (weightLeft * weightRight);
	_color.b += pixel.b * (weightLeft * weightRight);

	return weightLeft * weightRight;
}

void Image::scale(int neww, int newh)
{
	// TODO: Is necessary resize the image

	// The image will had the same size
	// Not compute necessary
	if (neww == 0 and newh == 0) return;

	const Geometry::Size newSize{ neww, newh };
	const Geometry::Size originalSize = getSize();

	if (newSize < originalSize)
	{
		// Scale down image, using super-sampling
		for (int py = 0; py < newSize.h; ++py)
		{
			float y0 = static_cast<float>(py * originalSize.h / newSize.h);
			float y0floor = std::floor(y0);
			float y0weight = 1.0f - (y0 - y0floor);
			int iy0 = static_cast<int>(y0floor);

			float y1 = static_cast<float>((py + 1) * originalSize.h / newSize.h);
			float y1floor = std::floor(y1 - 0.000'01);
			float y1weight = (y1 - y1floor);
			int iy1 = static_cast<int>(y1floor);

			for (int px = 0; px < newSize.h; ++px)
			{
				float x0 = static_cast<float>(px * originalSize.w / newSize.w);
				float x0floor = std::floor(x0);
				float x0weight = 1.0f - (x0 - x0floor);
				int ix0 = static_cast<int>(x0floor);

				float x1 = static_cast<float>((px + 1) * originalSize.w / newSize.w);
				float x1floor = std::floor(x1 - 0.000'01);
				float x1weight = (x1 - x1floor);
				int ix1 = static_cast<int>(x1floor);

				Color color = Color();
				float sumWeight = 0.0f;

				// Left and Right Fractional Edges
				for (int srcy = y0 + 1; srcy < y1; ++srcy)
				{
					const Color left = imageData.getPixel(ix0, srcy);
					const Color right = imageData.getPixel(ix1, srcy);

					sumWeight += calculateFractionalEdge(color, left, x0weight, right, x1weight);
				}

				// Top and Bottom Fractional Edges
				for (int srcx = x0 + 1; srcx < x1; ++srcx)
				{
					const Color top = imageData.getPixel(srcx, iy0);
					const Color bottom = imageData.getPixel(srcx, iy1);

					sumWeight += calculateFractionalEdge(color, top, y0weight, bottom, y1weight);
				}

				// Center
				for (int srcy = y0 + 1; srcy < y1; ++srcy)
				{
					for (int srcx = x0 + 1; srcx < x1; ++srcx)
					{
						const Color pixel = getPixel(srcx, srcy);

						color.r += pixel.r;
						color.g += pixel.g;
						color.b += pixel.b;

						sumWeight += 1.0f;
					}
				}

				// Corners
				sumWeight += calculateCorners(color, getPixel(ix0, iy0), x0weight, y0weight);
				sumWeight += calculateCorners(color, getPixel(ix0, iy1), x0weight, y1weight);
				sumWeight += calculateCorners(color, getPixel(ix1, iy1), x1weight, y1weight);
				sumWeight += calculateCorners(color, getPixel(ix1, iy0), x1weight, y0weight);

				sumWeight = 1.0f / sumWeight;

				color.r = color.r * sumWeight + 0.5f;
				color.g = color.g * sumWeight + 0.5f;
				color.b = color.b * sumWeight + 0.5f;

				setPixel(px, py, color);
			}
		}
	}
	else
	{
		// Scale up image, using nearest neighbour
		for (int py = 0; py < newSize.h; ++py)
		{
			int srcY = py * originalSize.h / newSize.h;

			for (int px = 0; px < newSize.w; ++px)
			{
				int srcX = px * originalSize.w / newSize.w;

				setPixel(px, py, getPixel(srcX, srcY));
			}
		}
	}


}

void Image::blit2x(Console& dest, int dx, int dy, int sx, int sy, int w, int h) const
{
	//TCOD_image_blit_2x(data, dest->data, dx, dy, sx, sy, w, h);

	Point destination(dx, dy);
	Point source(sx, sy);
	Geometry::Size size(w, h);

	Geometry::Size imageSize = getSize();

	if (size.w == -1) size.w = imageSize.w;
	if (size.h == -1) size.h = imageSize.h;

	int maxX = destination.x + size.w / 2 <= dest.getWidth() ? size.w : (dest.getWidth() - destination.x) * 2;
	int maxY = destination.y + size.h / 2 <= dest.getHeight() ? size.h : (dest.getHeight() - destination.y) * 2;

	maxX += source.x;
	maxY += source.y;

	for (int cx = source.x; cx < maxX; cx += 2)
	{
		for (int cy = source.y; cy < maxY; cy += 2)
		{
			int conX = destination.x + (cx - source.x) / 2;
			int conY = destination.y + (cy - source.y) / 2;

			Color consoleBackground = dest.getCharBackground(conX, conY);

			std::array<Color, 4> grid;

			grid.at(0) = getPixel(cx, cy);

			if (imageData.getKeyColor().equals(grid.at(0)))
			{
				grid.at(0) = consoleBackground;
			}

			if (cx < maxX - 1)
			{
				grid.at(1) = getPixel(cx + 1, cy);

				if (imageData.getKeyColor().equals(grid.at(1)))
				{
					grid.at(1) = consoleBackground;
				}
			}
			else
			{
				grid.at(1) = consoleBackground;
			}

			if (cy < maxY - 1)
			{
				grid.at(2) = getPixel(cx, cy + 1);

				if (imageData.getKeyColor().equals(grid.at(2)))
				{
					grid.at(2) = consoleBackground;
				}
			}
			else
			{
				grid.at(2) = consoleBackground;
			}

			if (cx < maxX - 1 and cy < maxY - 1)
			{
				grid.at(3) = getPixel(cx + 1, cy + 1);

				if (imageData.getKeyColor().equals(grid.at(3)))
				{
					grid.at(3) = consoleBackground;
				}
			}
			else
			{
				grid.at(3) = consoleBackground;
			}

			std::array<Color, 2> cols;

			// Analyse color, pasteurize, get patter
			auto [numberColors, ASCII] = getPattern(grid, cols);

			if (numberColors == 1)
			{
				// Single Color
				dest.setCharBackground(conX, conY, cols.at(0), BackgroundFlag::SET);
				dest.writeChar(conX, conY, ' ');
			}
			else
			{
				if (ASCII >= 0)
				{
					dest.setDefaultBackground(cols.at(0));
					dest.setDefaultForeground(cols.at(1));
					dest.writeChar(conX, conY, ASCII, BackgroundFlag::SET);
				}
				else
				{
					dest.setDefaultBackground(cols.at(1));
					dest.setDefaultForeground(cols.at(0));
					dest.writeChar(conX, conY, -ASCII, BackgroundFlag::SET);
				}
			}
		}
	}
}

void
Image::blit(Console& _console,
		const Point& _center,
		const BackgroundFlag _flag,
		const float scaleX,
		const float scaleY,
		const float angle)
{
	if (scaleX == 0.0f or scaleY == 0.0f or _flag == BackgroundFlag::NONE) return;

	// Size of the image
	Geometry::Size size = imageData.getSize();

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

		Point min;

		min.x = std::max(rx, 0);
		min.y = std::max(ry, 0);

		Point max;

		max.x = std::min(rx + rw, (int)_console.getWidth());
		max.y = std::min(ry + rh, (int)_console.getHeight());

		float invScaleX = 1.0f / scaleX;
		float invScaleY = 1.0f / scaleY;

		for (int cx = min.x; cx < max.x; ++cx)
		{
			for (int cy = min.y; cy < max.y; ++cy)
			{
				float ix = (iw + (float)(cx - _center.x) * newXX + (float)(cy - _center.y) * (-newYX)) * invScaleX;
				float iy = (ih + (float)(cx - _center.x) * newXY - (float)(cy - _center.y) * newYY) * invScaleY;

				Color color = imageData.getPixel((int)ix, (int)iy);

				if (not imageData.isHasKeyColor() or not imageData.getKeyColor().equals(color))
				{
					if (scaleX < 1.0f or scaleY < 1.0f)
					{
						Pointf _point0{ ix, iy };
						Pointf _point1{ ix + 1.0f, iy + 1.0f };

						color = imageData.getMipmapPixel(_point0, _point1);
					}

					_console.setCharBackground(cx, cy, color, _flag);
				}
			}
		}
	}
}

void Image::setPixel(int x, int y, const Color& _color)
{
	imageData.setPixel(x, y, _color);
}

std::pair<int, int> Image::getPattern(std::array<Color, 4>& desired, std::array<Color, 2>& palette) const
{
	// First colour trivial
	palette.at(0) = desired.at(0);

	int numberColors = 0;

	int counterOfColorsEquals = 0;

	// Ignore all duplicates
	for (int i = 1; i < 4; ++i)
	{
		if (not desired.at(i).equals(palette.at(0)))
		{
			break;
		}

		counterOfColorsEquals += 1;
	}

	// All the same
	if (counterOfColorsEquals == 4)
	{
		numberColors = 1;
		return { numberColors, 0 };
	}

	std::array<int, 2> weight = { counterOfColorsEquals, 1 };

	// Found a second color ...
	palette.at(1) = desired.at(counterOfColorsEquals);

	int flag = 0;

	flag |= 1 << (counterOfColorsEquals - 1);

	numberColors = 2;

	// Remaining colors
	counterOfColorsEquals += 1;

	while (counterOfColorsEquals < 4)
	{
		if (desired.at(counterOfColorsEquals).equals(palette.at(0)))
		{
			weight.at(0) += 1;
		}
		else if (desired.at(counterOfColorsEquals).equals(palette.at(1)))
		{
			flag |= 1 << (counterOfColorsEquals - 1);
			weight.at(1) += 1;
		}
		else
		{
			// Bah, too many colors
			// merge the two nearest
			int dist0i = distanceBetweenTwoColor(desired.at(counterOfColorsEquals), palette.at(0));
			int dist1i = distanceBetweenTwoColor(desired.at(counterOfColorsEquals), palette.at(1));
			int dist01 = distanceBetweenTwoColor(palette.at(0), palette.at(1));

			if (dist0i < dist1i)
			{
				if (dist0i <= dist01)
				{
					// Merge 0 and i
					palette.at(0) = Color::lerp(desired.at(counterOfColorsEquals), palette.at(0),
							weight.at(0) / (1.0f + weight.at(0)));
					weight.at(0) += 1;
				}
				else
				{
					// Merge 0 and 1
					palette.at(0) = Color::lerp(palette.at(0), palette.at(1),
							weight.at(1) / weight.at(0) + weight.at(1));
					weight.at(0) += 1;
					palette.at(1) = desired.at(counterOfColorsEquals);
					flag = 1 << (counterOfColorsEquals - 1);
				}
			}
			else
			{
				if (dist1i <= dist01)
				{
					palette.at(1) = Color::lerp(desired.at(counterOfColorsEquals), palette.at(1),
							weight.at(1) / (1.0f + weight.at(1)));
					weight.at(1) += 1;
					flag |= 1 << (counterOfColorsEquals - 1);
				}
				else
				{
					palette.at(0) = Color::lerp(palette.at(0), palette.at(1),
							weight.at(1) / (weight.at(0) + weight.at(1)));
					weight.at(0) += 1;
					palette.at(1) = desired.at(counterOfColorsEquals);
					flag = 1 << (counterOfColorsEquals - 1);
				}
			}
		}

		counterOfColorsEquals += 1;
	}

	// See the version of .c for the names and meanings
	const std::array<int, 8> flagToASCII = { 0, 227, 232, -230,
											 229, 231, -228, -226 };

	return { numberColors, flagToASCII.at(flag) };
}

int Image::distanceBetweenTwoColor(const Color& _lhs, const Color& _rhs) const
{
	int dr = _lhs.r - _rhs.r;
	int dg = _lhs.g - _rhs.g;
	int db = _lhs.b - _rhs.b;

	return dr * dr + dg * dg + db * db;
}
