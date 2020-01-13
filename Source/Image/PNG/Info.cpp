#include <vector>
#include "Image/PNG/Info.hpp"

LodePNGInfo::LodePNGInfo()
{
	for (unsigned i = 0; i < 3; ++i)
	{
		unknown_chunks_data[i] = nullptr;
		unknown_chunks_size[i] = 0;
	}
}

void LodePNGInfo::init()
{
	interlace_method = 0;
	compression_method = 0;
	filter_method = 0;
	background_defined = 0;
	background_r = 0;
	background_g = 0;
	background_b = 0;

	text_num = 0;
	text_keys = nullptr;
	text_strings = nullptr;

	itext_num = 0;
	itext_keys = nullptr;
	itext_langtags = nullptr;
	itext_transkeys = nullptr;
	itext_strings = nullptr;

	for (unsigned i = 0; i < 3; ++i)
	{
		unknown_chunks_data[i] = nullptr;
		unknown_chunks_size[i] = 0;
	}

	time_defined = 0;
	phys_defined = 0;
}

void LodePNGInfo::cleanup()
{
	color.cleanup();

	for (unsigned int i = 0; i < text_num; ++i)
	{
		delete[] text_keys[i];
		delete[] text_strings[i];
	}

	delete[] text_keys;
	delete[] text_strings;

	for (unsigned int i = 0; i < itext_num; ++i)
	{
		delete[] itext_keys[i];
		delete[] itext_langtags[i];
		delete[] itext_transkeys[i];
		delete[] itext_strings[i];
	}

	delete[] itext_keys;
	delete[] itext_langtags;
	delete[] itext_transkeys;
	delete[] itext_strings;

	for (unsigned int i = 0; i < 3; ++i)
	{
		delete[] unknown_chunks_data[i];
	}
}

unsigned LodePNGInfo::postProcessScanlines(
		std::vector <unsigned char>& out,
		std::vector <unsigned char>& in,
		unsigned w, unsigned h)
{
	// This function converts the filtered-padded-interlaced
	// data into pure 2D image buffer with the PNG's colortype.

	//  Steps:

	//  *) if no Adam7: 1) unfilter 2) remove padding bits
	//  (= posible extra bits per scanline if bpp < 8)

	//  *) if adam7: 1) 7x unfilter 2) 7x remove padding
	//  bits 3) Adam7_deinterlace

	// NOTE: the in buffer will be overwritten with intermediate data!

	unsigned bpp = this->color.getBitsPerPixel();

	if (bpp == 0)
	{
		// error: invalid colortype
		return 31;
	}

	if (interlace_method == 0)
	{
		if (bpp < 8 && w * bpp != ((w * bpp + 7) / 8) * 8)
		{

		}
		else
		{
			unsigned error = unfilter(out, in, w, h, bpp);

			if (error)
			{
				return error;
			}
		}
	}

	return 0;
}

unsigned LodePNGInfo::unfilter(
		std::vector <unsigned char>& out,
		const std::vector <unsigned char>& in,
		const unsigned int w,
		const unsigned int h,
		const unsigned int bpp)
{
	// For PNG filter method 0 this function unfilters a
	// single image (e.g. without interlacing this is called
	// once, with Adam7 seven times) out must have enough
	// bytes allocated already, in must have the
	// scanlines + 1 filtertype byte per scanline
	// w and h are image dimensions or dimensions of reduced
	// image, bpp is bits per pixel in and out are allowed
	// to be the same memory address (but aren't the same
	// size since in has the extra filter bytes)

	// bytewidth is used for filtering, is 1 when
	// bpp < 8, number of bytes per pixel otherwise
	size_t bytewidth = (bpp + 7) / 8;
	size_t linebytes = (w * bpp + 7) / 8;

	std::vector <unsigned char> prevline;

	for (int y = 0; y < h; ++y)
	{
		unsigned outindex = linebytes * y;

		// the extra filterbyte added to each row
		unsigned inindex = (1 + linebytes) * y;

		unsigned char filterType = in[inindex];

		unsigned error = unfilterScanline(
				out, outindex, in, inindex + 1, prevline,
				bytewidth, filterType,
				linebytes);

		if (error)
		{
			return error;
		}

		prevline.clear();
		prevline.resize(out.size() - outindex);

		unsigned index = 0;

		for (unsigned i = outindex; i < out.size(); ++i)
		{
			prevline[i - outindex] = out[outindex + index];
			index++;
		}
	}

	return 0;
}

unsigned LodePNGInfo::unfilterScanline(
		std::vector <unsigned char>& recon,
		const unsigned int outindex,
		const std::vector <unsigned char>& scanline,
		const unsigned int inindex,
		const std::vector <unsigned char>& precon,
		size_t bytewidth, unsigned char filterType, size_t length)
{
	// For PNG filter method 0
	//  unfilter a PNG image scanline by scanline.
	//  when the pixels are smaller than 1 byte,
	//  the filter works byte per byte (bytewidth = 1)
	//  precon is the previous unfiltered scanline, recon
	//  the result, scanline the current one
	//  the incoming scanlines do NOT include the
	//  filtertype byte, that one is given in the
	//  parameter filterType instead
	//  recon and scanline MAY be the same memory
	//  address! precon must be disjoint.

	unsigned index = 0;

	switch (filterType)
	{
	case 0:

		for (unsigned i = outindex; i < outindex + length; i++)
		{
			recon[i] = scanline[index + inindex];
			index++;
		}

		break;
	case 1:

		for (unsigned i = outindex; i < outindex + bytewidth; i++)
		{
			recon[i] = scanline[index + inindex];
			index++;
		}

		for (unsigned i = bytewidth + outindex; i < outindex + length; i++)
		{
			recon[i] = scanline[index + inindex] + recon[i - bytewidth];
			index++;
		}

		break;

	case 2:

		if (!precon.empty())
		{
			for (unsigned i = outindex; i < outindex + length; i++)
			{
				recon[i] = scanline[index + inindex] + precon[i - outindex];
				index++;
			}
		}
		else
		{
			for (unsigned i = outindex; i < outindex + length; i++)
			{
				recon[i] = scanline[index + inindex];
				index++;
			}
		}

		break;

	case 3:

		if (!precon.empty())
		{
			for (unsigned i = outindex; i < outindex + bytewidth; i++)
			{
				recon[i] = scanline[index + inindex] + precon[i - outindex] / 2;
				index++;
			}

			for (unsigned i = outindex + bytewidth; i < outindex + length; i++)
			{
				recon[i] = scanline[index + inindex] + ((recon[i - bytewidth] + precon[i - outindex]) / 2);
				index++;
			}
		}
		else
		{
			for (unsigned i = outindex; i < outindex + bytewidth; i++)
			{
				recon[i] = scanline[index + inindex];
				index++;
			}

			for (unsigned i = outindex + bytewidth; i < outindex + length; i++)
			{
				recon[i] = scanline[index + inindex] + recon[i - bytewidth] / 2;
				index++;
			}
		}

		break;

	case 4:

		if (!precon.empty())
		{
			for (unsigned i = outindex; i < outindex + bytewidth; i++)
			{
				//paethPredictor(0, precon[i], 0) is always precon[i]
				recon[i] = (scanline[index + inindex] + precon[i - outindex]);
				index++;
			}

			index = 0;

			for (unsigned i = bytewidth; i < length; i++)
			{
				recon[i + outindex] =
						(scanline[index + inindex + bytewidth] +
						 paethPredictor(
								 recon[i - bytewidth + outindex],
								 precon[i],
								 precon[i - bytewidth]));

				index++;
			}
		}
		else
		{
			for (unsigned i = outindex; i < outindex + bytewidth; i++)
			{
				recon[i] = scanline[index + inindex];
				index++;
			}

			index = 0;

			for (unsigned i = outindex + bytewidth; i < outindex + length; i++)
			{
				// paethPredictor(recon[i - bytewidth], 0, 0) is
				// always recon[i - bytewidth]
				recon[i] = (scanline[index + inindex + bytewidth] + recon[i - bytewidth]);
				index++;
			}
		}

		break;

	default:

		return 36; /*error: unexisting filter type given*/
	}

	return 0;
}

unsigned char LodePNGInfo::paethPredictor(short a, short b, short c)
{
	auto pa = (short)std::abs(b - c);
	auto pb = (short)std::abs(a - c);
	auto pc = (short)std::abs(a + b - c - c);

	if (pc < pa && pc < pb)
	{
		return (unsigned char)c;
	}
	else if (pb < pa)
	{
		return (unsigned char)b;
	}
	else
	{
		return (unsigned char)a;
	}
}
