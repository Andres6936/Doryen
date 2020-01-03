#ifndef LIBTCOD_AUTOCONVERT_HPP
#define LIBTCOD_AUTOCONVERT_HPP

/*automatically use color type with less bits per pixel if losslessly possible. Default: LAC_AUTO*/
enum LodePNGAutoConvert
{
	LAC_NO, /*use color type user requested*/
	LAC_ALPHA, /*use color type user requested, but if only opaque pixels and RGBA or grey+alpha, use RGB or grey*/
	LAC_AUTO, /*use PNG color type that can losslessly represent the uncompressed image the smallest possible*/
	/*
	like AUTO, but do not choose 1, 2 or 4 bit per pixel types.
	sometimes a PNG image compresses worse if less than 8 bits per pixels.
	*/
			LAC_AUTO_NO_NIBBLES
};

#endif //LIBTCOD_AUTOCONVERT_HPP
