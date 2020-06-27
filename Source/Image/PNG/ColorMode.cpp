#include <new>
#include "Doryen/Image/PNG/ColorMode.hpp"

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

bool LodePNGColorMode::isEquals(const LodePNGColorMode& other)
{
	if (this->colortype != other.colortype ||
		this->bitdepth != other.bitdepth ||
		this->key_defined != other.key_defined ||
		this->palettesize != other.palettesize)
	{
		return false;
	}

	if (this->key_defined)
	{
		if (this->key_r != other.key_r ||
			this->key_g != other.key_g ||
			this->key_b != other.key_b)
		{
			return false;
		}
	}

	for (int i = 0; i < this->palettesize * 4; ++i)
	{
		if (this->palette[i] != other.palette[i])
		{
			return false;
		}
	}

	return true;
}

void LodePNGColorMode::copy(const LodePNGColorMode& other)
{
	this->cleanup();

	// Hack?
	*this = other;

	if (other.palette)
	{
		this->palette = new unsigned char[other.palettesize * 4];

		for (int i = 0; i < other.palettesize * 4; ++i)
		{
			this->palette[i] = other.palette[i];
		}
	}
}

unsigned int LodePNGColorMode::getRawSize(unsigned int w, unsigned int h)
{
	return (w * h * getBitsPerPixel() + 7) / 8;
}
