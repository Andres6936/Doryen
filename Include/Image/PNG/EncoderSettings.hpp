#ifndef LIBTCOD_ENCODERSETTINGS_HPP
#define LIBTCOD_ENCODERSETTINGS_HPP

#include "Image/PNG/CompressSettings.hpp"
#include "Image/PNG/FilterStrategy.hpp"
#include "Image/PNG/AutoConvert.hpp"

/*Settings for the encoder.*/
class LodePNGEncoderSettings
{

public:

	LodePNGCompressSettings zlibsettings; /*settings for the zlib encoder, such as window size, ...*/

	LodePNGAutoConvert auto_convert; /*how to automatically choose output PNG color type, if at all*/

	LodePNGFilterStrategy filter_strategy;

	/*used if filter_strategy is LFS_PREDEFINED. In that case, this must point to a buffer with
	  the same length as the amount of scanlines in the image, and each value must <= 5. You
	  have to cleanup this buffer, LodePNG will never free it.*/
	unsigned char* predefined_filters;

	/*force creating a PLTE chunk if colortype is 2 or 6 (= a suggested palette).
	If colortype is 3, PLTE is _always_ created.*/
	unsigned force_palette;
	/*add LodePNG identifier and version as a text chunk, for debugging*/
	unsigned add_id;
	/*encode text chunks as zTXt chunks instead of tEXt chunks, and use compression in iTXt chunks*/
	unsigned text_compression;
};

#endif //LIBTCOD_ENCODERSETTINGS_HPP
