#ifndef LIBTCOD_ENCODERSETTINGS_HPP
#define LIBTCOD_ENCODERSETTINGS_HPP

#include "Doryen/Image/PNG/CompressSettings.hpp"
#include "Doryen/Image/PNG/FilterStrategy.hpp"
#include "Doryen/Image/PNG/AutoConvert.hpp"

/*Settings for the encoder.*/
class LodePNGEncoderSettings
{

public:

	/**
	 * settings for the zlib encoder, such as window size, ...
	 */
	LodePNGCompressSettings zlibsettings;

	/**
	 * how to automatically choose output PNG color type, if at all
	 */
	LodePNGAutoConvert auto_convert = LodePNGAutoConvert::LAC_AUTO;

	LodePNGFilterStrategy filter_strategy = LodePNGFilterStrategy::LFS_HEURISTIC;

	/**
	 * used if filter_strategy is LFS_PREDEFINED. In that case,
	 * this must point to a buffer with the same length as
	 * the amount of scanlines in the image, and each value
	 * must <= 5. You have to cleanup this buffer, LodePNG
	 * will never free it.
	 */
	unsigned char* predefined_filters = nullptr;

	/**
	 * force creating a PLTE chunk if colortype is 2
	 * or 6 (= a suggested palette).
	 * If colortype is 3, PLTE is _always_ created.
	 */
	unsigned force_palette = 0;

	/**
	 * add LodePNG identifier and version as a text chunk,
	 * for debugging
	 */
	unsigned add_id = 0;

	/**
	 * encode text chunks as zTXt chunks instead of
	 * tEXt chunks, and use compression in iTXt chunks
	 */
	unsigned text_compression = 1;
};

#endif //LIBTCOD_ENCODERSETTINGS_HPP
