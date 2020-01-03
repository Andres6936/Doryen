#ifndef LIBTCOD_DECODERSETTINGS_HPP
#define LIBTCOD_DECODERSETTINGS_HPP

#include "Image/PNG/DecompressSettings.hpp"

/*
Settings for the decoder. This contains settings for the PNG and the Zlib
decoder, but not the Info settings from the Info structs.
*/
class LodePNGDecoderSettings
{

public:

	LodePNGDecompressSettings zlibsettings; /*in here is the setting to ignore Adler32 checksums*/

	unsigned ignore_crc; /*ignore CRC checksums*/
	unsigned color_convert; /*whether to convert the PNG to the color type you want. Default: yes*/
	unsigned read_text_chunks; /*if false but remember_unknown_chunks is true, they're stored in the unknown chunks*/
	/*store all bytes from unknown chunks in the LodePNGInfo (off by default, useful for a png editor)*/
	unsigned remember_unknown_chunks;
};

#endif //LIBTCOD_DECODERSETTINGS_HPP
