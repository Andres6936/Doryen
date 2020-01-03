#ifndef LIBTCOD_DECOMPRESSSETTINGS_HPP
#define LIBTCOD_DECOMPRESSSETTINGS_HPP

/*Settings for zlib decompression*/
class LodePNGDecompressSettings
{

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
};

#endif //LIBTCOD_DECOMPRESSSETTINGS_HPP
