#include "Image/PNG/DecompressSettings.hpp"
#include "Image/PNG/HuffmanTree.hpp"

unsigned
LodePNGDecompressSettings::zlibDecompress(std::vector <unsigned char>& out,
		unsigned int outsize, std::vector <unsigned char>& in)
{
	if (in.size() < 2)
	{
		// error, size of zlib data too small
		return 53;
	}

	// read information from zlib header
	if ((in[0] * 256 + in[1]) % 31 != 0)
	{
		// error: 256 * in[0] + in[1] must be a multiple of
		// 31, the FCHECK value is supposed to be made that way
		return 24;
	}

	unsigned CM = in[0] & 15;
	unsigned CINFO = (in[0] >> 4) & 15;
	// FCHECK = in[1] & 31; //FCHECK is already tested above
	unsigned FDICT = (in[1] >> 5) & 1;
	// FLEVEL = (in[1] >> 6) & 3; //FLEVEL is not used here

	if (CM != 8 || CINFO > 7)
	{
		// error: only compression method 8: inflate with
		// sliding window of 32k is supported by the PNG spec
		return 25;
	}

	if (FDICT != 0)
	{
		// error: the specification of PNG says about the zlib stream:
		// "The additional flags shall not specify a preset dictionary."
		return 26;
	}

	// Delete the two first elements
	in.erase(in.begin());
	in.erase(in.begin());

	unsigned error = inflate(out, in);

	return error;
}

unsigned int LodePNGDecompressSettings::inflate(std::vector <unsigned char>& out, std::vector <unsigned char>& in)
{
	std::vector <unsigned char> v = out;

	// bit pointer in the "in" data, current byte is
	// bp >> 3, current bit is bp & 0x7 (from lsb to msb
	// of the byte)
	size_t bp = 0;

	// byte position in the out buffer
	size_t pos = 0;

	unsigned error = 0;

	bool BFINAL = false;

	while (!BFINAL)
	{
		if (bp + 2 >= in.size() * 8)
		{
			// rror, bit pointer will jump past memory
			return 52;
		}

		BFINAL = readBitFromStream(&bp, in);
		unsigned BTYPE = 1 * readBitFromStream(&bp, in);
		BTYPE += 2 * readBitFromStream(&bp, in);

		if (BTYPE == 3)
		{
			// error: invalid BTYPE
			return 20;
		}
		else if (BTYPE == 0)
		{
			// no compression
			error = inflateNoCompression(out, in, &bp, &pos);
		}
		else
		{
			// TODO: Implemented Huffman Block
		}

		if (error)
		{
			return error;
		}
	}

	return error;
}

unsigned char LodePNGDecompressSettings::readBitFromStream(
		size_t* bitpointer, std::vector <unsigned char>& bitstream)
{
	auto result = (unsigned char)(bitstream[*bitpointer >> 3] >>
															  (*bitpointer & 0x7) & (unsigned char)1);

	(*bitpointer)++;

	return result;
}

unsigned LodePNGDecompressSettings::inflateNoCompression(
		std::vector <unsigned char>& out,
		std::vector <unsigned char>& in,
		size_t* bp, size_t* pos)
{
	// go to first boundary of byte
	while (((*bp) & 0x7) != 0)
	{
		(*bp)++;
	}

	// byte position
	size_t p = (*bp) / 8;

	// read LEN (2 bytes) and NLEN (2 bytes)
	if (p >= in.size() - 4)
	{
		// error, bit pointer will jump past memory
		return 52;
	}

	unsigned LEN = in[p] + 256 * in[p + 1];
	p += 2;

	unsigned NLEN = in[p] + 256 * in[p + 1];
	p += 2;

	// check if 16-bit NLEN is really the one's complement of LEN
	if (LEN + NLEN != 65'535)
	{
		// error: NLEN is not one's complement of LEN
		return 21;
	}

	if ((*pos) + LEN >= out.size())
	{
		out.resize((*pos) + LEN);
	}

	// read the literal data: LEN bytes are now stored
	// in the out buffer
	if (p + LEN > in.size())
	{
		// error: reading outside of in buffer
		return 23;
	}

	for (unsigned int n = 0; n < LEN; n++)
	{
		out[(*pos)++] = in[p++];
	}

	(*bp) = p * 8;

	return 0;
}

unsigned LodePNGDecompressSettings::inflateHuffmanBlock(
		std::vector <unsigned char>& out,
		std::vector <unsigned char>& in,
		size_t* bp, size_t* pos, unsigned btype)
{
	// inflate a block with dynamic of fixed Huffman tree

	unsigned error = 0;

	// the huffman tree for literal and length codes
	HuffmanTree tree_ll;
	// the huffman tree for distance codes
	HuffmanTree tree_d;

	size_t inbitlength = in.size() * 8;

	if (btype == 1)
	{
		HuffmanTree::getTreeInflateFixed(tree_ll, tree_d);
	}
	else if (btype == 2)
	{

	}

	while (!error)
	{

	}

	return error;
}
