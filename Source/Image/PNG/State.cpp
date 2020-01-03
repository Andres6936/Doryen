#include "Image/PNG/State.hpp"
#include "Image/PNG/CRC32Table.hpp"

unsigned int LodePNGState::inspect(unsigned* w, unsigned* h, const unsigned char* in, size_t insize)
{
	if (insize == 0 || in == nullptr)
	{
		// error: the given data is empty
		error = 48;
		return error;
	}
	else if (insize < 29)
	{
		// error: the data length is smaller
		// than the length of a PNG header
		error = 27;
		return error;
	}

	// when decoding a new PNG image, make sure all
	// parameters created after previous decoding
	// are reset

	info_png.cleanup();
	info_png.init();

	if (in[0] != 137 || in[1] != 80 || in[2] != 78 || in[3] != 71 ||
		in[4] != 13 || in[5] != 10 || in[6] != 26 || in[7] != 10)
	{
		// error: the first 8 bytes are not the correct PNG signature
		error = 28;
		return error;
	}
	else if (in[12] != 'I' || in[13] != 'H' || in[14] != 'D' || in[15] != 'R')
	{
		// error: it doesn't start with a IHDR chunk!
		error = 29;
		return error;
	}

	// read the values given in the header
	*w = read32BitInt(&in[16]);
	*h = read32BitInt(&in[20]);

	info_png.color.bitdepth = in[24];
	info_png.color.colortype = (LodePNGColorType)in[25];
	info_png.compression_method = in[26];
	info_png.filter_method = in[27];
	info_png.interlace_method = in[28];

	if (!decoder.ignore_crc)
	{
		unsigned CRC = read32BitInt(&in[29]);
		unsigned checksum = crc32(&in[12], 17);

		if (CRC != checksum)
		{
			// invalid CRC
			error = 57;
			return error;
		}
	}

	if (info_png.compression_method != 0)
	{
		// error: only compression method 0 is
		// allowed in the specification
		error = 32;
		return error;
	}
	else if (info_png.filter_method != 0)
	{
		// error: only filter method 0 is allowed
		// in the specification
		error = 33;
		return error;
	}
	else if (info_png.interlace_method > 1)
	{
		// error: only interlace methods 0 and
		// 1 exist in the specification
		error = 34;
		return error;
	}

	error = checkColorValidity(
			info_png.color.colortype,
			info_png.color.bitdepth);

	return error;
}

unsigned LodePNGState::read32BitInt(const unsigned char* buffer)
{
	return buffer[0] << 24 | buffer[1] << 16 | buffer[2] << 8 | buffer[3];
}

unsigned LodePNGState::crc32(const unsigned char* buffer, size_t length)
{
	// Update a running CRC with the bytes buffer[0..length-1]
	// -- the CRC should be initialized to all 1's, and the
	// transmitted value is the 1's complement of the final
	// running CRC (see the crc() routine below).

	unsigned c = 0xFFFFFFFFL;

	// Compute table {See construct}.
	CRC32Table crc32Table;

	for (int n = 0; n < length; ++n)
	{
		c = crc32Table.getIndex((c ^ buffer[n]) & 0xff) ^ (c >> 8);
	}

	return c ^ 0xffffffffL;
}

unsigned int LodePNGState::checkColorValidity(LodePNGColorType colortype,
		unsigned bd)
{
	switch (colortype)
	{
	case LodePNGColorType::LCT_GREY:

		if (!(bd == 1 || bd == 2 || bd == 4 || bd == 8 || bd == 16))
		{ return 37; }
		break; /*grey*/

	case LodePNGColorType::LCT_RGB:
		if (!(bd == 8 || bd == 16))
		{ return 37; }
		break; /*RGB*/

	case LodePNGColorType::LCT_PALETTE:

		if (!(bd == 1 || bd == 2 || bd == 4 || bd == 8))
		{ return 37; }
		break; /*palette*/

	case LodePNGColorType::LCT_GREY_ALPHA:

		if (!(bd == 8 || bd == 16))
		{ return 37; }
		break; /*grey + alpha*/

	case LodePNGColorType::LCT_RGBA:

		if (!(bd == 8 || bd == 16))
		{ return 37; }
		break; /*RGBA*/

	default:

		return 31;
	}

	// allowed color type / bits combination
	return 0;
}

unsigned LodePNGState::getBitsPerPixel()
{
	// calculate bits per pixel out of colortype
	// and bitdepth
	return info_png.color.getBitsPerPixel();
}
