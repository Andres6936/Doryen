#ifndef LIBTCOD_DECOMPRESSSETTINGS_HPP
#define LIBTCOD_DECOMPRESSSETTINGS_HPP

#include <cstddef>
#include <vector>

/*Settings for zlib decompression*/
class LodePNGDecompressSettings
{

private:

	static constexpr short FIRST_LENGTH_CODE_INDEX = 257;

	static constexpr short LAST_LENGTH_CODE_INDEX = 285;

	/**
	 * the base lengths represented by codes 257-285
	 */
	static const unsigned LENGTHBASE[29];

	/**
	 * the extra bits used by codes 257-285 (added
	 * to base length)
	 */
	static const unsigned LENGTHEXTRA[29];

	/**
	 * the base backwards distances (the bits of
	 * distance codes appear after length codes
	 * and use their own huffman tree)
	 */
	static const unsigned DISTANCEBASE[30];

	/**
	 * the order in which "code length alphabet code
	 * lengths" are stored, out of this the huffman
	 * tree of the dynamic huffman tree lengths
	 * is generated
	 */
	static const unsigned DISTANCEEXTRA[30];

public:

	/**
	 * if 1, continue and don't give an error
	 * message if the Adler32 checksum is corrupted
	 */
	unsigned ignore_adler32 = 0;

	/**
	 * use custom decoder if
	 * LODEPNG_CUSTOM_ZLIB_DECODER and
	 * LODEPNG_COMPILE_ZLIB are enabled
	 */
	unsigned custom_decoder = 0;

	// Methods

	/**
	 * Decompresses Zlib data. Reallocates the out buffer and
	 * appends the data. The data must be according to the
	 * zlib specification. Either, *out must be NULL and *outsize
	 * must be 0, or, *out must be a valid buffer and *outsize its
	 * size in bytes. out must be freed by user after usage.
	 */
	unsigned zlibDecompress(std::vector <unsigned char>& out,
			unsigned int outsize, std::vector <unsigned char>& in);

private:

	/**
	 * Inflate a buffer. Inflate is the decompression step of
	 * deflate. Out buffer must be freed after use.
	 */
	static unsigned int inflate(std::vector <unsigned char>& out,
			std::vector <unsigned char>& in);

	static unsigned char readBitFromStream(size_t* bitpointer,
			std::vector <unsigned char>& bitstream);

	static unsigned inflateNoCompression(std::vector <unsigned char>& out,
			std::vector <unsigned char>& in, size_t* bp, size_t* pos);

	static unsigned inflateHuffmanBlock(std::vector <unsigned char>& out,
			std::vector <unsigned char>& in, size_t* bp, size_t* pos,
			unsigned btype);

	static unsigned readBitsFromStream(
			size_t* bitpointer,
			const std::vector <unsigned char>& bitstream,
			size_t nbits);

	static unsigned read32BitInt(const std::vector <unsigned char>& buffer);
};

#endif //LIBTCOD_DECOMPRESSSETTINGS_HPP
