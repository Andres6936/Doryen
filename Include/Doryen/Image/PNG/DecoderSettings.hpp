#ifndef LIBTCOD_DECODERSETTINGS_HPP
#define LIBTCOD_DECODERSETTINGS_HPP

#include "Doryen/Image/PNG/DecompressSettings.hpp"

/*
Settings for the decoder. This contains settings for the PNG and the Zlib
decoder, but not the Info settings from the Info structs.
*/
class LodePNGDecoderSettings
{

public:

	/**
	 * in here is the setting to ignore Adler32 checksums
	 */
	LodePNGDecompressSettings zlibsettings;

	/**
	 * ignore CRC checksums
	 */
	unsigned ignore_crc = 0;

	/**
	 * whether to convert the PNG to the color type you want. Default: yes
	 */
	unsigned color_convert = 1;

	/**
	 * if false but remember_unknown_chunks is true, they're stored in the unknown chunks
	 */
	unsigned read_text_chunks = 1;

	/**
	 * store all bytes from unknown chunks in the LodePNGInfo (off by default, useful for a png editor)
	 */
	unsigned remember_unknown_chunks = 0;
};

#endif //LIBTCOD_DECODERSETTINGS_HPP
