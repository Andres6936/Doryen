#include "Image/PNG/ColorMode.hpp"

void LodePNGColorMode::cleanup()
{
	delete[] palette;
	palette = nullptr;
	palettesize = 0;
}

unsigned LodePNGColorMode::getBitsPerPixel()
{
	// calculate bits per pixel out of colortype and bitdepth
	// bits per pixel is amount of channels * bits per channel
	return getNumberColorChannels(colortype) * bitdepth;
}

unsigned LodePNGColorMode::getNumberColorChannels(LodePNGColorType colortype)
{
	switch (colortype)
	{
	case LodePNGColorType::LCT_GREY:
		return 1; /*grey*/

	case LodePNGColorType::LCT_RGB:
		return 3; /*RGB*/

	case LodePNGColorType::LCT_PALETTE:
		return 1; /*palette*/

	case LodePNGColorType::LCT_GREY_ALPHA:
		return 2; /*grey + alpha*/

	case LodePNGColorType::LCT_RGBA:
		return 4; /*RGBA*/
	}

	return 0; /*unexisting color type*/
}
