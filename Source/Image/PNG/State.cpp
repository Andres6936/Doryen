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

unsigned int LodePNGState::decode(unsigned char** out, unsigned* w,
		unsigned* h, const unsigned char* in, size_t insize)
{
	*out = nullptr;

	decodeGeneric(out, w, h, in, insize);

	if (error)
	{
		return error;
	}

	if (!decoder.color_convert || info_raw.isEquals(info_png.color))
	{
		// Same color type, no copying or converting of data needed
		// Store the info_png color settings on the info_raw so that
		// the info_raw still reflects what colortype the raw image
		// has to the end user

		if (!decoder.color_convert)
		{
			info_raw.copy(info_png.color);
		}
	}
	else
	{
		// color conversion needed; sort of copy of the data
		unsigned char* data = *out;

		// TODO: check if this works according to the statement
		//  in the documentation: "The converter can convert
		//  from greyscale input color type, to 8-bit greyscale
		//  or greyscale with alpha
		if (!(info_raw.colortype == LodePNGColorType::LCT_RGB ||
			  info_raw.colortype == LodePNGColorType::LCT_RGBA) &&
			!(info_raw.bitdepth == 8))
		{
			// unsupported color mode conversion
			return 56;
		}

		unsigned int outsize = info_raw.getRawSize(*w, *h);

		try
		{
			*out = new unsigned char[outsize];
		}
		catch (std::bad_alloc& exception)
		{
			// error: Alloc fail
			error = 83;
		}

		convert(*out, data, &info_raw, &info_png.color, *w, *h);

		delete[] data;
	}

	return error;
}

void LodePNGState::decodeGeneric(unsigned char** out, unsigned* w,
		unsigned* h, const unsigned char* in, size_t insize)
{
	bool IEDN = false;

	// the data from idat chunks
	std::vector <unsigned char> idat;

	// for unknown chunk order
	bool unknown = false;
	// 1 = after IHDR, 2 = after PLTE, 3 = after IDAT
	unsigned critial_pos = 1;

	// provide some proper output values if error will happen
	*out = nullptr;

	error = inspect(w, h, in, insize);

	if (error)
	{
		return;
	}

	// first byte of the first chunk after the header
	const unsigned char* chunk = &in[33];

	// loop through the chunks, ignoring unknown chunks
	// and stopping at IEND chunk. IDAT data is put at
	// the start of the in buffer
	while (!IEDN && !error)
	{
		if ((size_t)((chunk - in) + 12) > insize || chunk < in)
		{
			// error: size of the in buffer too small
			// to contain next chunk
			error = 30;
			return;
		}

		// length of the data of the chunk, excluding the length
		// bytes, chunk type and CRC bytes
		unsigned chunkLength = read32BitInt(&chunk[0]);

		if (chunkLength > 2'147'483'647)
		{
			// error: chunk length larger than the max PNG chunk size
			error = 63;
			return;
		}

		if ((size_t)((chunk - in) + chunkLength + 12) > insize ||
			(chunk + chunkLength + 12) < in)
		{
			// error: size of the in buffer too small to
			// contain next chunk
			error = 64;
			return;
		}

		// the data in the chunk
		const unsigned char* data = &chunk[8];

		// IDAT chunk, containing compressed image data
		if (isChunkTypeEqualsTo(chunk, "IDAT"))
		{
			idat.reserve(chunkLength);

			for (int i = 0; i < chunkLength; ++i)
			{
				idat.push_back(data[i]);
			}

			critial_pos = 3;
		}
		else if (isChunkTypeEqualsTo(chunk, "IEND"))
		{
			// IEND chunk
			IEDN = true;
		}
		else if (isChunkTypeEqualsTo(chunk, "PLTE"))
		{
			// palette chunk (PLTE)
			if (info_png.color.palette)
			{
				delete[] info_png.color.palette;
			}

			info_png.color.palettesize = chunkLength / 3;

			try
			{
				info_png.color.palette = new unsigned char
				[4 * info_png.color.palettesize];
			}
			catch (std::bad_alloc& exception)
			{
				info_png.color.palettesize = 0;

				// Alloc fail
				error = 83;
				return;
			}

			if (info_png.color.palettesize > 256)
			{
				// error: palette too big
				error = 38;
				return;
			}

			unsigned pos = 0;

			for (int i = 0; i < info_png.color.palettesize; ++i)
			{
				info_png.color.palette[4 * i + 0] = data[pos++]; // R
				info_png.color.palette[4 * i + 1] = data[pos++]; // G
				info_png.color.palette[4 * i + 2] = data[pos++]; // B
				info_png.color.palette[4 * i + 3] = 255; // Alpha
			}

			critial_pos = 2;
		}
		else if (isChunkTypeEqualsTo(chunk, "tRNS"))
		{
			// palette transparency chunk (tRNS)
			if (info_png.color.colortype == LodePNGColorType::LCT_PALETTE)
			{
				if (chunkLength > info_png.color.palettesize)
				{
					// error: more alpha values given than
					// there are palette entries
					error = 38;
					return;
				}

				for (int i = 0; i < chunkLength; ++i)
				{
					info_png.color.palette[4 * i + 3] = data[i];
				}
			}
			else if (info_png.color.colortype == LodePNGColorType::LCT_GREY)
			{
				if (chunkLength != 2)
				{
					// error: this chunk must be 2 bytes
					// for greyscale image
					error = 30;
					return;
				}

				info_png.color.key_defined = 1;

				info_png.color.key_r = 256 * data[0] + data[1];
				info_png.color.key_g = 256 * data[0] + data[1];
				info_png.color.key_b = 256 * data[0] + data[1];
			}
			else if (info_png.color.colortype == LodePNGColorType::LCT_RGB)
			{
				if (chunkLength != 6)
				{
					// error: this chunk must be 6 bytes for RGB image
					error = 41;
					return;
				}

				info_png.color.key_defined = 1;

				info_png.color.key_r = 256 * data[0] + data[1];
				info_png.color.key_g = 256 * data[2] + data[3];
				info_png.color.key_b = 256 * data[4] + data[5];
			}
			else
			{
				// error: tRNS chunk not allowed for other color models
				error = 42;
				return;
			}
		}
		else if (isChunkTypeEqualsTo(chunk, "bKGD"))
		{
			if (info_png.color.colortype == LodePNGColorType::LCT_PALETTE)
			{
				if (chunkLength != 1)
				{
					// error: this chunk must be 1 byte for
					// indexed color image
					error = 43;
					return;
				}

				info_png.background_defined = 1;

				info_png.background_r = data[0];
				info_png.background_g = data[0];
				info_png.background_b = data[0];
			}
			else if (info_png.color.colortype == LodePNGColorType::LCT_GREY ||
					 info_png.color.colortype == LodePNGColorType::LCT_GREY_ALPHA)
			{
				if (chunkLength != 2)
				{
					// error: this chunk must be 2 bytes for
					// greyscale image
					error = 44;
					return;
				}

				info_png.background_defined = 1;

				info_png.background_r = 256 * data[0] + data[1];
				info_png.background_g = 256 * data[0] + data[1];
				info_png.background_b = 256 * data[0] + data[1];
			}
			else if (info_png.color.colortype == LodePNGColorType::LCT_RGB ||
					 info_png.color.colortype == LodePNGColorType::LCT_RGBA)
			{
				if (chunkLength != 6)
				{
					// error: this chunk must be 6 bytes for
					// greyscale image
					error = 45;
					return;
				}

				info_png.background_defined = 1;

				info_png.background_r = 256 * data[0] + data[1];
				info_png.background_g = 256 * data[2] + data[3];
				info_png.background_b = 256 * data[4] + data[5];
			}
		}
		else if (isChunkTypeEqualsTo(chunk, "tEXt"))
		{
			// text chunk (tEXt)
			if (decoder.read_text_chunks)
			{
				std::vector <char> key;
				std::vector <char> str;

				// not really a while loop, only used to break on error
				while (!error)
				{
					unsigned length = 0;

					while (length < chunkLength && data[length] != 0)
					{
						length++;
					}

					if (length + 1 >= chunkLength)
					{
						// error, end reached, no null terminator?
						error = 75;
						return;
					}

					key.reserve(length + 1);

					for (int i = 0; i < length; ++i)
					{
						key.push_back(data[i]);
					}

					unsigned stringBegin = length + 1;

					if (stringBegin > chunkLength)
					{
						// error, end reached, no null terminator?
						error = 75;
						return;
					}

					length = chunkLength - stringBegin;

					str.reserve(length);

					for (int i = 0; i < length; ++i)
					{
						str.push_back(data[stringBegin + i]);
					}

					addText(key, str);

					break;
				}
			}
		}
		else if (isChunkTypeEqualsTo(chunk, "zTXt"))
		{
			// compressed text chunk (zTXt)
			if (decoder.read_text_chunks)
			{
				std::vector <unsigned char> decoded;
				std::vector <char> key;

				// not really a while loop, only used to break on error
				while (!error)
				{
					unsigned length = 0;

					for (length = 0; length < chunkLength && data[length] != 0; length++)
					{

					}

					if (length + 2 >= chunkLength)
					{
						// error, end reached, no null terminator?
						error = 75;
						return;
					}

					key.reserve(length + 1);

					for (int i = 0; i < length; ++i)
					{
						key.push_back(data[i]);
					}

					// Insert a null to end
					key.insert(key.end() + 1, 0);

					if (data[length + 1] != 0)
					{
						// the 0 byte indicating compression
						// must be 0
						error = 72;
						return;
					}

					unsigned stringBegin = length + 2;

					if (stringBegin > chunkLength)
					{
						// error, end reached, no null terminator?
						error = 75;
						return;
					}

					length = chunkLength - stringBegin;

					// TODO: ZLib Decompress

					break;
				}
			}
		}
		else if (isChunkTypeEqualsTo(chunk, "iTXt"))
		{
			if (decoder.read_text_chunks)
			{
				// TODO: Implemented
			}
		}
		else if (isChunkTypeEqualsTo(chunk, "tIME"))
		{
			if (chunkLength != 7)
			{
				// error: invalid tIME chunk size
				error = 73;
				return;
			}

			info_png.time_defined = 1;
			info_png.time.year = 256 * data[0] + data[1];
			info_png.time.month = data[2];
			info_png.time.day = data[3];
			info_png.time.hour = data[4];
			info_png.time.minute = data[5];
			info_png.time.second = data[6];
		}
		else if (isChunkTypeEqualsTo(chunk, "pHYs"))
		{
			if (chunkLength != 9)
			{
				// error: invalid pHYs chunk size
				error = 74;
				return;
			}

			info_png.phys_defined = 1;
			info_png.phys_x = 16'777'216 * data[0] + 65'536 * data[1] + 256 * data[2] + data[3];
			info_png.phys_y = 16'777'216 * data[4] + 65'536 * data[5] + 256 * data[6] + data[7];
			info_png.phys_unit = data[8];
		}
		else
		{
			// it's not an implemented chunk type, so
			// ignore it: skip over the data
			if (!isChunkAncillary(chunk))
			{
				// error: unknown critical chunk (5th bit of first byte of chunk type is 0)
				error = 69;
				return;
			}

			unknown = true;

			if (decoder.remember_unknown_chunks)
			{
				// TODO: Implemented
			}
		}

		// check CRC if wanted, only on known chunk types
		if (!decoder.ignore_crc && !unknown)
		{
			// TODO: Implemented
		}

		if (!IEDN)
		{
			// get the length of the data of the chunk. Total
			// chunk length has 12 bytes more.
			unsigned totalChunkLength = read32BitInt(&chunk[0]) + 12;

			// iterate to next chunks. don't use on IEND chunk, as there
			// is no next chunk then
			chunk = &chunk[totalChunkLength];
		}
	}

	if (!error)
	{
		std::vector <unsigned char> scanlines;

		//maximum final image length is already reserved
		// in the vector's length - this is not really necessary
		unsigned int length = info_png.color.getRawSize(*w, *h) + *h;

		scanlines.reserve(length);

		// decompress with the Zlib decompressor
		error = decoder.zlibsettings.zlibDecompress(scanlines, length, idat);

		if (!error)
		{
			std::vector <unsigned char> outv;

			outv.resize(info_png.color.getRawSize(*w, *h), 0);

			error = info_png.postProcessScanlines(outv, scanlines, *w, *h);

			for (int i = 0; i < outv.size(); ++i)
			{
				*out[i] = outv[i];
			}

			*out[outv.size() + 1] = '\0';
		}
	}
}

bool LodePNGState::isChunkTypeEqualsTo(const unsigned char* chunk,
		const std::string& type)
{
	if (type.size() != 4)
	{
		return false;
	}

	return (chunk[4] == type[0] && chunk[5] == type[1] &&
			chunk[6] == type[2] && chunk[7] == type[3]);
}

unsigned LodePNGState::addText(const std::vector <char>& key,
		const std::vector <char>& str)
{
	std::string tempKey;
	std::string tempStr;

	tempKey.reserve(key.size());

	for (char c : key)
	{
		tempKey.push_back(c);
	}

	tempStr.reserve(str.size());

	for (char c : str)
	{
		tempStr.push_back(c);
	}

	info_png.text[tempKey] = tempStr;

	return 0;
}

bool LodePNGState::isChunkAncillary(const unsigned char* chunk)
{
	return ((chunk[4] & 32) != 0);
}

unsigned LodePNGState::convert(unsigned char* out, const unsigned char* in, LodePNGColorMode* mode_out,
		LodePNGColorMode* mode_in, unsigned w, unsigned h)
{
	return 0;
}
