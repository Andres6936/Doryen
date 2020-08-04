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

#include "Doryen/Image/ImageData.hpp"

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

		ImageData imageData;

		void* data;

		bool deleteData;

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
		 * Creating an image from a console.
		 *
		 * You can create an image from any console (either
		 * the root console or an offscreen console).
		 *
		 * The image size will depend on the console size
		 * and the font characters size.
		 *
		 * You can then save the image to a file with the
		 * save function.
		 *
		 * @param console The console to convert.
		 */
		explicit Image(const Console& console);

		/**
		 * Refreshing an image created from a console.
		 *
		 * If you need to refresh the image with the console's
		 * new content, you don't have to delete it and create
		 * another one. Instead, use this function. Note that
		 * you must use the same console that was used in
		 * the TCOD_image_from_console call (or at least a
		 * console with the same size).
		 *
		 * @example
		 *
		 * @code
		 *
		 * Image pix = Image(Console::root); // Create an image from the root console.
		 *
		 * // ... Modify the console.
		 *
		 * pix.refreshConsole(Console::root); // Update the image with the console's new content
		 *
		 * @endcode
		 *
		 * @param console The console to capture.
		 */
		void refreshConsole(const Console* console);

		/**
		@PageName image_read
		@PageTitle Reading data from a TCODImage
		@PageFather image
		@FuncTitle Getting the size of an image
		@FuncDesc You can read the size of an image in pixels with this function.
		@Cpp void TCODImage::getSize(int *w,int *h) const
		@C void TCOD_image_get_size(TCOD_image_t image, int *w,int *h)
		@Py image_get_size(image) # returns w,h
		@C# void TCODImage::getSize(out int w, out int h)
		@Param image In the C version, the image handler, obtained with the load function.
		@Param w,h When the function returns, those variables contain the size of the image.
		@CppEx
			TCODImage *pix = new TCODImage(80,50);
			int w,h;
			pix->getSize(&w,&h); // w = 80, h = 50
		@CEx
			TCOD_image_t pix = TCOD_image_new(80,50);
			int w,h;
			TCOD_image_get_size(pix,&w,&h); // w = 80, h = 50
		@PyEx
			pix = libtcod.image_new(80,50)
			w,h=libtcod.image_get_size(pix)
			# w = 80, h = 50
		*/
		void getSize(int* w, int* h) const;

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
		@PageName image_read
		@FuncTitle Checking if a pixel is transparent
		@FuncDesc You can use this simpler version (for images with alpha layer, returns true only if alpha == 0) :
		@Cpp bool TCODImage::isPixelTransparent(int x,int y) const
		@C bool TCOD_image_is_pixel_transparent(TCOD_image_t image,int x, int y)
		@Py image_is_pixel_transparent(image, x, y)
		@C# bool TCODImage::isPixelTransparent(int x,int y)
		@Param image In the C and python version, the image handler, obtained with the load function.
		@Param x,y The pixel coordinates inside the image.
			0 <= x < width
			0 <= y < height
		*/
		bool isPixelTransparent(int x, int y) const;

		/**
		@PageName image_read
		@FuncTitle Getting the average color of a part of the image
		@FuncDesc This method uses mipmaps to get the average color of an arbitrary rectangular region of the image.
			It can be used to draw a scaled-down version of the image. It's used by libtcod's blitting functions.
		@Cpp Doryen::TCODColor TCODImage::getMipmapPixel(float x0,float y0, float x1, float y1)
		@C TCOD_color_t TCOD_image_get_mipmap_pixel(TCOD_image_t image,float x0,float y0, float x1, float y1)
		@Py image_get_mipmap_pixel(image,x0,y0, x1, y1)
		@C# Doryen::TCODColor TCODImage::getMipmapPixel(float x0,float y0, float x1, float y1)
		@Param image	In the C version, the image handler, obtained with the load function.
		@Param x0,y0	Coordinates in pixels of the upper-left corner of the region.
			0.0 <= x0 < x1
			0.0 <= y0 < y1
		@Param x1,y1	Coordinates in pixels of the lower-right corner of the region.
			x0 < x1 < width
			y0 < y1 < height
		@CppEx
			// Get the average color of a 5x5 "superpixel" in the center of the image.
			TCODImage *pix = new TCODImage(80,50);
			Doryen::TCODColor col=pix->getMipMapPixel(37.5f, 22.5f, 42.5f, 28.5f);
		@CEx
			TCOD_image_t pix = TCOD_image_new(80,50);
			TCOD_color_t col=TCOD_image_get_mipmap_pixel(pix,37.5f, 22.5f, 42.5f, 28.5f);
		@PyEx
			pix = libtcod.image_new(80,50)
			col=libtcod.image_get_mipmap_pixel(pix,37.5, 22.5, 42.5, 28.5)
		*/
		Color getMipmapPixel(float x0, float y0, float x1, float y1);

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
		@PageName image_update
		@FuncTitle Flipping the image horizontally
		@Cpp void TCODImage::hflip()
		@C void TCOD_image_hflip(TCOD_image_t image)
		@Py image_hflip(image)
		@C# void TCODImage::hflip()
		@Param image	In the C and python version, the image handler, obtained with the load function.
		*/
		void hflip();

		/**
		@PageName image_update
		@FuncTitle Flipping the image vertically
		@Cpp void TCODImage::vflip()
		@C void TCOD_image_vflip(TCOD_image_t image)
		@Py image_vflip(image)
		@C# void TCODImage::vflip()
		@Param image	In the C and python version, the image handler, obtained with the load function.
		*/
		void vflip();

		/**
		@PageName image_update
		@FuncTitle Rotating the image clockwise
		@FuncDesc Rotate the image clockwise by increment of 90 degrees.
		@Cpp void TCODImage::rotate90(int numRotations=1)
		@C void TCOD_image_rotate90(TCOD_image_t image, int numRotations)
		@Py image_rotate90(image, num=1)
		@C# void TCODImage::rotate90(int numRotations)
		@Param image	In the C and python version, the image handler, obtained with the load function.
		@Param numRotations	Number of 90 degrees rotations. Should be between 1 and 3.
		*/
		void rotate90(int numRotations = 1);

		/**
		@PageName image_update
		@FuncTitle Inverting the colors of the image
		@Cpp void TCODImage::invert()
		@C void TCOD_image_invert(TCOD_image_t image)
		@Py image_invert(image)
		@C# void TCODImage::invert()
		@Param image	In the C and python version, the image handler, obtained with the load function.
		*/
		void invert();

		/**
		@PageName image_save
		@PageFather image
		@PageTitle Saving an image to a bmp or png file.
		@PageDesc You can save an image to a 24 bits .bmp or .png file.
		@Cpp void TCODImage::save(const char *filename)
		@C void TCOD_image_save(TCOD_image_t image, const char *filename)
		@Py image_save(image, filename)
		@C# void TCODImage::save(string filename)
		@Param image	In the C version, the image handler, obtained with any image creation function.
		@Param filename	Name of the .bmp or .png file.
		@CppEx
			TCODImage *pix = new TCODImage(10,10);
			pix->save("mypic.bmp");
		@CEx
			TCOD_image_t pix = TCOD_image_from_console(my_offscreen_console);
			TCOD_image_save(pix,"mypic.bmp");
		@PyEx
			pix = libtcod.image_from_console(my_offscreen_console)
			libtcod.image_save(pix,"mypic.bmp")
		  */
		void save(const char* filename) const;

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
		void blitRect(Console* console, int x, int y, int w = -1, int h = -1,
				TCOD_bkgnd_flag_t bkgnd_flag = TCOD_BKGND_SET) const;

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
		void blit(Console& _console, const Point& _center, BackgroundFlag _flag,
				float scaleX, float scaleY, float angle);

		/**
		@PageName image_blit
		@FuncTitle Blitting with a mask
		@FuncDesc When blitting an image, you can define a key color that will be ignored by the blitting function. This makes it possible to blit non rectangular images or images with transparent pixels.
		@Cpp void TCODImage::setKeyColor(const Doryen::TCODColor keyColor)
		@C void TCOD_image_set_key_color(TCOD_image_t image, TCOD_color_t keyColor)
		@Py image_set_key_color(image, keyColor)
		@C# void TCODImage::setKeyColor(Doryen::TCODColor keyColor)
		@Param image	In the C and python version, the image handler, obtained with the load function.
		@Param color	Pixels with this color will be skipped by blitting functions.
		@CppEx
			TCODImage *pix = TCODImage("mypix.bmp");
			pix->setKeyColor(Doryen::TCODColor::red);
			// blitting the image, omitting red pixels
			pix->blitRect(Doryen::TCODConsole::root,40,25);
		@CEx
			TCOD_image_t pix = TCOD_image_new(10,10);
			TCOD_image_set_key_color(pix,TCOD_red);
			TCOD_image_blit_rect(pix,NULL,40,25,5,5,TCOD_BKGND_SET);
		@PyEx
			pix = libtcod.image_new(10,10)
			libtcod.image_set_key_color(pix,libtcod.red)
			libtcod.image_blit_rect(pix,0,40,25,5,5,libtcod.BKGND_SET)
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

		Image(TCOD_image_t img, ImageData imageData)
		{
			this->data = img;
			this->deleteData = false;
			this->imageData = imageData;
		}

		explicit Image(TCOD_image_t img)
		{
			this->data = img;
			this->deleteData = false;
		}

		virtual ~Image();

	private:

		float calculateFractionalEdge(Color& _color,
				const Color& leftTop, float _weightLeft,
				const Color& rightBottom, float _weightRight) const;


		float calculateCorners(Color& _color, const Color& pixel, float weightLeft, float weightRight);

		std::pair<int, int> getPattern(std::array<Color, 4>& desired, std::array<Color, 2>& palette) const;

		int distanceBetweenTwoColor(const Color& _lhs, const Color& _rhs) const;
	};
}

