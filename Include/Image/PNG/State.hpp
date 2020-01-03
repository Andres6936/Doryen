#ifndef LIBTCOD_STATE_HPP
#define LIBTCOD_STATE_HPP

#include "Image/PNG/Info.hpp"
#include "Image/PNG/ColorMode.hpp"
#include "Image/PNG/DecoderSettings.hpp"
#include "Image/PNG/EncoderSettings.hpp"

/*The settings, state and information for extended encoding and decoding.*/
class LodePNGState
{

public:

	LodePNGDecoderSettings decoder; /*the decoding settings*/
	LodePNGEncoderSettings encoder; /*the encoding settings*/
	LodePNGColorMode info_raw; /*specifies the format in which you would like to get the raw pixel buffer*/
	LodePNGInfo info_png; /*info of the PNG image obtained after decoding*/
	unsigned error;

	LodePNGState() = default;

	//For the lodepng::State subclass.
	virtual ~LodePNGState() = default;

};

#endif //LIBTCOD_STATE_HPP
