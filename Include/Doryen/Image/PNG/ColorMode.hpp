#ifndef LIBTCOD_COLORMODE_HPP
#define LIBTCOD_COLORMODE_HPP

#include <cstddef>
#include "Doryen/Image/PNG/ColorType.hpp"

/*
Color mode of an image. Contains all information required to decode the pixel
bits to RGBA colors. This information is the same as used in the PNG file
format, and is used both for PNG and raw image data in LodePNG.
*/
class LodePNGColorMode
{

public:

	/*header (IHDR)*/
	LodePNGColorType colortype = LodePNGColorType::LCT_RGBA; /*color type, see PNG standard or documentation further in this header file*/
	unsigned bitdepth = 8;  /*bits per sample, see PNG standard or documentation further in this header file*/

	/*
	palette (PLTE and tRNS)

	Dynamically allocated with the colors of the palette, including alpha.
	When encoding a PNG, to store your colors in the palette of the LodePNGColorMode, first use
	lodepng_palette_clear, then for each color use lodepng_palette_add.
	If you encode an image without alpha with palette, don't forget to put value 255 in each A byte of the palette.

	When decoding, by default you can ignore this palette, since LodePNG already
	fills the palette colors in the pixels of the raw RGBA output.

	The palette is only supported for color type 3.
	*/
	unsigned char* palette = nullptr; /*palette in RGBARGBA... order*/
	size_t palettesize = 0; /*palette size in number of colors (amount of bytes is 4 * palettesize)*/

	/*
	transparent color key (tRNS)

	This color uses the same bit depth as the bitdepth value in this struct, which can be 1-bit to 16-bit.
	For greyscale PNGs, r, g and b will all 3 be set to the same.

	When decoding, by default you can ignore this information, since LodePNG sets
	pixels with this key to transparent already in the raw RGBA output.

	The color key is only supported for color types 0 and 2.
	*/
	unsigned key_defined = 0; /*is a transparent color key given? 0 = false, 1 = true*/
	unsigned key_r = 0;       /*red/greyscale component of color key*/
	unsigned key_g = 0;       /*green component of color key*/
	unsigned key_b = 0;       /*blue component of color key*/

	LodePNGColorMode() = default;

	// Getters

	/**
	 * @return The byte size of a raw image buffer with
	 * given width, height and color mode
	 */
	unsigned int getRawSize(unsigned int w, unsigned int h);

	// Methods

	void cleanup();

	unsigned getBitsPerPixel();

	bool isEquals(const LodePNGColorMode& other);

	void copy(const LodePNGColorMode& other);

private:

	static unsigned getNumberColorChannels(LodePNGColorType colortype);

};

#endif //LIBTCOD_COLORMODE_HPP
