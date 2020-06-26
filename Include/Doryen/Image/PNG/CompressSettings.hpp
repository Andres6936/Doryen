#ifndef LIBTCOD_COMPRESSSETTINGS_HPP
#define LIBTCOD_COMPRESSSETTINGS_HPP

/*
Settings for zlib compression. Tweaking these settings tweaks the balance
between speed and compression ratio.
*/
class LodePNGCompressSettings /*deflate = compress*/
{

public:

	/*LZ77 related settings*/

	/**
	 * the block type for LZ (0, 1, 2
	 * or 3, see zlib standard). Should
	 * be 2 for proper compression.
	 */
	unsigned btype = 2;

	/**
	 * whether or not to use LZ77.
	 * Should be 1 for proper compression.
	 */
	unsigned use_lz77 = 1;

	/**
	 * the maximum is 32768, higher gives
	 * more compression but is slower.
	 * Typical value: 2048.
	 */
	unsigned windowsize = 2048;

	/**
	 * use custom encoder if
	 * LODEPNG_CUSTOM_ZLIB_DECODER and
	 * LODEPNG_COMPILE_ZLIB are enabled
	 */
	unsigned custom_encoder = 0;
};

#endif //LIBTCOD_COMPRESSSETTINGS_HPP
