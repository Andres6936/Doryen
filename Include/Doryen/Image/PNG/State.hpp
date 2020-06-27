#ifndef LIBTCOD_STATE_HPP
#define LIBTCOD_STATE_HPP

#include <string>
#include <vector>

#include "Doryen/Image/PNG/Info.hpp"
#include "Doryen/Image/PNG/ColorMode.hpp"
#include "Doryen/Image/PNG/DecoderSettings.hpp"
#include "Doryen/Image/PNG/EncoderSettings.hpp"

/*The settings, state and information for extended encoding and decoding.*/
class LodePNGState
{

public:

	LodePNGDecoderSettings decoder; /*the decoding settings*/
	LodePNGEncoderSettings encoder; /*the encoding settings*/
	LodePNGColorMode info_raw; /*specifies the format in which you would like to get the raw pixel buffer*/
	LodePNGInfo info_png; /*info of the PNG image obtained after decoding*/
	unsigned error = 1;

	LodePNGState() = default;

	//For the lodepng::State subclass.
	virtual ~LodePNGState() = default;

	// Methods

	/**
	 * Read the information from the header and store it
	 * in the LodePNGInfo. return value is error
	 *
	 * Read the PNG header, but not the actual data. This
	 * returns only the information that is in the header
	 * chunk of the PNG, such as width, height and color type.
	 * The information is placed in the info_png field
	 * of the LodePNGState.
	 *
	 * @param in Header chunk of the PNG.
	 * @return Value is error
	 */
	unsigned int inspect(unsigned* w, unsigned* h, const unsigned char* in, size_t insize);

	/**
	 * Same as lodepng_decode_memory, but uses a LodePNGState
	 * to allow custom settings and getting much more information
	 * about the PNG image and color mode.
	 */
	unsigned int decode(std::vector <unsigned char>& out, unsigned* w, unsigned* h,
			const unsigned char* in, size_t insize);

	/**
	 * Get the total amount of bits per pixel, based on
	 * colortype and bitdepth in the struct
	 *
	 * @return total amount of bits per pixel
	 */
	unsigned getBitsPerPixel();

private:

	static unsigned read32BitInt(const unsigned char* buffer);

	/**
	 * Calculate CRC32 of buffer
	 *
	 * @return the CRC of the bytes bufffer[0..length-1].
	 */
	static unsigned crc32(const unsigned char* buffer, size_t length);

	static unsigned int checkColorValidity(LodePNGColorType colortype, unsigned bd);

	/**
	 * read a PNG, the result will be in the same color type as
	 * the PNG (hence "generic")
	 */
	void decodeGeneric(std::vector <unsigned char>& out, unsigned* w, unsigned* h,
			const unsigned char* in, size_t insize);

	/**
	 * check if the type is the given type
	 */
	static bool isChunkTypeEqualsTo(const unsigned char* chunk,
			const std::string& type);

	/**
	 * 0: it's one of the critical chunk types, 1:
	 * it's an ancillary chunk (see PNG standard)
	 */
	static bool isChunkAncillary(const unsigned char* chunk);

	unsigned addText(const std::vector <char>& key, const std::vector <char>& str);

	/**
	 * Converts raw buffer from one color type to another color
	 * type, based on LodePNGColorMode structs to describe the
	 * input and output color type. See the reference manual at
	 * the end of this header file to see which color conversions
	 * are supported.
	 * return value = LodePNG error code (0 if all went ok, an
	 * error if the conversion isn't supported) The out buffer
	 * must have size (w * h * bpp + 7) / 8, where bpp is the
	 * bits per pixel of the output color type (lodepng_get_bpp)
	 */
	unsigned convert(std::vector <unsigned char>& out, const std::vector <unsigned char>& in,
			LodePNGColorMode* mode_out, LodePNGColorMode* mode_in,
			unsigned w, unsigned h);

};

#endif //LIBTCOD_STATE_HPP
