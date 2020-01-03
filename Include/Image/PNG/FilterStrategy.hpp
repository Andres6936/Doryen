#ifndef LIBTCOD_FILTERSTRATEGY_HPP
#define LIBTCOD_FILTERSTRATEGY_HPP

/*automatically use color type with less bits per pixel if losslessly possible. Default: AUTO*/
enum LodePNGFilterStrategy
{
	LFS_HEURISTIC, /*official PNG heuristic*/
	LFS_ZERO, /*every filter at zero*/
	LFS_MINSUM, /*like the official PNG heuristic, but use minimal sum always, including palette and low bitdepth images*/
	/*
	Brute-force-search PNG filters by compressing each filter for each scanline.
	This gives better compression, at the cost of being super slow. Experimental!
	If you enable this, also set zlibsettings.windowsize to 32768 and choose an
	optimal color mode for the PNG image for best compression. Default: 0 (false).
	*/
			LFS_BRUTE_FORCE,
	LFS_PREDEFINED /*use predefined_filters buffer: you specify the filter type for each scanline*/
};

#endif //LIBTCOD_FILTERSTRATEGY_HPP
