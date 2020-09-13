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

#pragma once

#include <array>
#include <utility>

#include "Doryen/Renderer/Sdl/ImageSdl.hpp"

namespace Doryen
{

	/**
	 * This toolkit contains some image manipulation utilities.
	 */
	class Image
	{

	private:

		using Point = Geometry::Point2D<int>;

	public:

		ImageSdl imageData;

		/**
		 * Creating an empty image.
		 *
		 * You can create an image of any size, filled with
		 * black with this function.
		 *
		 * @param width Width of imagen.
		 * @param height Height of imagen.
		 */
		Image(int width, int height);

		/**
		 * Loading a .bmp or .png image.
		 *
		 * You can read data from a .bmp or .png file (for example
		 * to draw an image using the background color of the
		 * console cells).
		 *
		 * @note Note that only 24bits and 32bits PNG files
		 * are currently supported.
		 *
		 * @param filename
		 */
		explicit Image(const char* filename);

		/**
		 * @brief Getting the size of an image.
		 *
		 * @return The size of image in pixels.
		 */
		Geometry::Size getSize() const;

		/**
		@PageName image_read
		@FuncTitle Getting the color of a pixel
		@FuncDesc You can read the colors from an image with this function.
		@Cpp Doryen::TCODColor TCODImage::getPixel(int x, int y) const
		@C TCOD_color_t TCOD_image_get_pixel(TCOD_image_t image,int x, int y)
		@Py image_get_pixel(image, x, y)
		@C# Doryen::TCODColor TCODImage::getPixel(int x, int y)
		@Param image In the C and python version, the image handler, obtained with the load function.
		@Param x,y The pixel coordinates inside the image.
			0 <= x < width
			0 <= y < height
		@CppEx
			TCODImage *pix = new TCODImage(80,50);
			Doryen::TCODColor col=pix->getPixel(40,25);
		@CEx
			TCOD_image_t pix = TCOD_image_new(80,50);
			TCOD_color_t col=TCOD_image_get_pixel(pix,40,25);
		@PyEx
			pix = litbcod.image_new(80,50)
			col=litbcod.image_get_pixel(pix,40,25)
		*/
		Color getPixel(int x, int y) const;

		/**
		@PageName image_read
		@FuncTitle Getting the alpha value of a pixel
		@FuncDesc If you have set a key color for this image with setKeyColor, or if this image was created from a 32 bits PNG file (with alpha layer), you can get the pixel transparency with this function. This function returns a value between 0 (transparent pixel) and 255 (opaque pixel).
		@Cpp int TCODImage::getAlpha(int x, int y) const
		@C int TCOD_image_get_alpha(TCOD_image_t image, int x, int y)
		@Py image_get_alpha(image, x, y)
		@C# int TCODImage::getAlpha(int x, int y)
		@Param image In the C and python version, the image handler, obtained with the load function.
		@Param x,y The pixel coordinates inside the image.
			0 <= x < width
			0 <= y < height
		*/
		int getAlpha(int x, int y) const;

		/**
		 * @brief Checking if a pixel is transparent.
		 *
		 * You can use this simpler version (for images with alpha layer,
		 * returns true only if alpha == 0)
		 *
		 * @param x Coordinates in x of pixel inside the image.
		 * @param y Coordinates in y of pixel inside the image.
		 * @return True if the channel alpha is 0 or the pixel is equal
		 *  to key color, false otherwise.
		 */
		bool isPixelTransparent(int x, int y) const;

		/**
		 * @brief Changing the color of a pixel
		 *
		 * @param x Coordinates in x of pixel inside the image.
		 * @param y Coordinates in y of pixel inside the image.
		 * @param _color The new color of the pixel.
		 */
		void setPixel(int x, int y, const Color& _color);

		/**
		@PageName image_update
		@FuncTitle Scaling an image
		@FuncDesc You can resize an image and scale its content. If neww < oldw or newh < oldh, supersampling is used to scale down the image. Else the image is scaled up using nearest neightbor.
		@Cpp void TCODImage::scale(int neww, int newh)
		@C void TCOD_image_scale(TCOD_image_t image,int neww, int newh)
		@Py image_scale(image, neww,newh)
		@C# void TCODImage::scale(int neww, int newh)
		@Param image	In the C and python version, the image handler, obtained with the load function.
		@Param neww,newh	The new size of the image.
		*/
		void scale(int neww, int newh);

		/**
		 * Flipping the image horizontally
		 */
		void flippingHorizontally();

		/**
		 * Flipping the image vertically
		 */
		void flippingVertically();

		/**
		@PageName image_blit
		@PageFather image
		@PageTitle Blitting an image on a console
		@FuncTitle Standard blitting
		@FuncDesc This function blits a rectangular part of the image on a console without scaling it or rotating it. Each pixel of the image fills a console cell.
		@Cpp void TCODImage::blitRect(Doryen::TCODConsole *console, int x, int y, int w=-1, int h=-1, TCOD_bkgnd_flag_t bkgnd_flag = TCOD_BKGND_SET ) const
		@C void TCOD_image_blit_rect(TCOD_image_t image, TCOD_console_t console, int x, int y, int w, int h, TCOD_bkgnd_flag_t bkgnd_flag)
		@Py image_blit_rect(image, console, x, y, w, h, bkgnd_flag)
		@C#
			void TCODImage::blitRect(Doryen::TCODConsole console, int x, int y)
			void TCODImage::blitRect(Doryen::TCODConsole console, int x, int y, int w)
			void TCODImage::blitRect(Doryen::TCODConsole console, int x, int y, int w, int h)
			void TCODImage::blitRect(Doryen::TCODConsole console, int x, int y, int w, int h, TCODBackgroundFlag bkgnd_flag)
		@Param image	In the C version, the image handler, obtained with the load function.
		@Param console	The console on which the image will be drawn. In the C version, use NULL for the root console.
		@Param x,y	Coordinates in the console of the upper-left corner of the image.
		@Param w,h	Dimension of the image on the console. Use -1,-1 to use the image size.
		@Param flag	This flag defines how the cell's background color is modified. See TCOD_bkgnd_flag_t.
		*/
		void blitRect(Console& console, int x, int y, int w = -1, int h = -1,
				BlendModes flag = BlendModes::SET);

		/**
		 * @brief Blitting with scaling and/or rotation
		 *
		 * This function allows you to specify the floating point coordinates
		 * of the center of the image, its scale and its rotation angle.
		 *
		 * @param _console The console on which the image will be drawn.
		 * @param _center Coordinates in the console of the center of the image.
		 * @param _flag This flag defines how the cell's background color is modified.
		 * @param scaleX Scale coefficient. Must be > 0.0.
		 * @param scaleY Scale coefficient. Must be > 0.0.
		 * @param angle Rotation angle in radians.
		 */
		void blit(Console& _console, const Point& _center, BlendModes _flag,
				float scaleX, float scaleY, float angle);

		/**
		 * @brief Blitting with a mask
		 *
		 * When blitting an image, you can define a key color that will be
		 * ignored by the blitting function. This makes it possible to blit
		 * non rectangular images or images with transparent pixels.
		 *
		 * @example
		 * @code
		 * Image pix {"myPix.bmp"};
		 * pix.setKeyColor(Palette::Red);
		 *
		 * // Blitting the image, omitting red pixels
		 * pix.blitRect(console, 40, 25);
		 * @endcode
		 *
		 * @param keyColor Pixels with this color will be skipped by
		 *  blitting functions.
		 */
		void setKeyColor(const Color& keyColor);

		/**
		@PageName image_blit
		@FuncTitle Blitting with subcell resolution
		@FuncDesc Eventually, you can use some special characters in the libtcod fonts :
			<img src="subcell.png">
			to double the console resolution using this blitting function.
			<table><tr><td>
			Comparison before/after subcell resolution in TCOD :<br />
			<img src="subcell_comp.png"></td><td>
			Pyromancer ! screenshot, making full usage of subcell resolution :<br />
			<img src="subcell_pyro.png"></td></tr></table>
		@Cpp void TCODImage::blit2x(Doryen::TCODConsole *dest, int dx, int dy, int sx=0, int sy=0, int w=-1, int h=-1 ) const;
		@C void TCOD_image_blit_2x(TCOD_image_t image, TCOD_console_t dest, int dx, int dy, int sx, int sy, int w, int h);
		@Py image_blit_2x(image, dest, dx, dy, sx=0, sy=0, w=-1, h=-1)
		@C#
			void TCODImage::blit2x(Doryen::TCODConsole dest, int dx, int dy);
			void TCODImage::blit2x(Doryen::TCODConsole dest, int dx, int dy, int sx);
			void TCODImage::blit2x(Doryen::TCODConsole dest, int dx, int dy, int sx, int sy);
			void TCODImage::blit2x(Doryen::TCODConsole dest, int dx, int dy, int sx, int sy, int w);
			void TCODImage::blit2x(Doryen::TCODConsole dest, int dx, int dy, int sx, int sy, int w, int h);
		@Param image	In the C and python version, the image handler, obtained with the load function.
		@Param dest	The console of which the image will be blited. Foreground, background and character data will be overwritten.
		@Param dx,dy	Coordinate of the console cell where the upper left corner of the blitted image will be.
		@Param sx,sy,w,h	Part of the image to blit. Use -1 in w and h to blit the whole image.
		*/
		void blit2x(Console& dest, int dx, int dy, int sx = 0, int sy = 0, int w = -1, int h = -1) const;

	private:

		float calculateFractionalEdge(Color& _color,
				const Color& leftTop, float _weightLeft,
				const Color& rightBottom, float _weightRight) const;


		float calculateCorners(Color& _color, const Color& pixel, float weightLeft, float weightRight);

		std::pair<int, int> getPattern(std::array<Color, 4>& desired, std::array<Color, 2>& palette) const;

		int distanceBetweenTwoColor(const Color& _lhs, const Color& _rhs) const;
	};
}

