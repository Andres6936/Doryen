#ifndef LIBTCOD_INFO_HPP
#define LIBTCOD_INFO_HPP

/*Information about the PNG image, except pixels, width and height.*/
class LodePNGInfo
{

public:

	/*header (IHDR), palette (PLTE) and transparency (tRNS) chunks*/
	unsigned compression_method;/*compression method of the original file. Always 0.*/
	unsigned filter_method;     /*filter method of the original file*/
	unsigned interlace_method;  /*interlace method of the original file*/
	LodePNGColorMode color;     /*color type and bits, palette and transparency of the PNG file*/

	/*
	suggested background color chunk (bKGD)
	This color uses the same color mode as the PNG (except alpha channel), which can be 1-bit to 16-bit.

	For greyscale PNGs, r, g and b will all 3 be set to the same. When encoding
	the encoder writes the red one. For palette PNGs: When decoding, the RGB value
	will be stored, not a palette index. But when encoding, specify the index of
	the palette in background_r, the other two are then ignored.

	The decoder does not use this background color to edit the color of pixels.
	*/
	unsigned background_defined; /*is a suggested background color given?*/
	unsigned background_r;       /*red component of suggested background color*/
	unsigned background_g;       /*green component of suggested background color*/
	unsigned background_b;       /*blue component of suggested background color*/

	/*
	non-international text chunks (tEXt and zTXt)

	The char** arrays each contain num strings. The actual messages are in
	text_strings, while text_keys are keywords that give a short description what
	the actual text represents, e.g. Title, Author, Description, or anything else.

	A keyword is minimum 1 character and maximum 79 characters long. It's
	discouraged to use a single line length longer than 79 characters for texts.

	Don't allocate these text buffers yourself. Use the init/cleanup functions
	correctly and use lodepng_add_text and lodepng_clear_text.
	*/
	size_t text_num; /*the amount of texts in these char** buffers (there may be more texts in itext)*/
	char** text_keys; /*the keyword of a text chunk (e.g. "Comment")*/
	char** text_strings; /*the actual text*/

	/*
	international text chunks (iTXt)
	Similar to the non-international text chunks, but with additional strings
	"langtags" and "transkeys".
	*/
	size_t itext_num; /*the amount of international texts in this PNG*/
	char** itext_keys; /*the English keyword of the text chunk (e.g. "Comment")*/
	char** itext_langtags; /*language tag for this text's language, ISO/IEC 646 string, e.g. ISO 639 language tag*/
	char** itext_transkeys; /*keyword translated to the international language - UTF-8 string*/
	char** itext_strings; /*the actual international text - UTF-8 string*/

	/*time chunk (tIME)*/
	unsigned time_defined; /*set to 1 to make the encoder generate a tIME chunk*/
	LodePNGTime time;

	/*phys chunk (pHYs)*/
	unsigned phys_defined; /*if 0, there is no pHYs chunk and the values below are undefined, if 1 else there is one*/
	unsigned phys_x; /*pixels per unit in x direction*/
	unsigned phys_y; /*pixels per unit in y direction*/
	unsigned phys_unit; /*may be 0 (unknown unit) or 1 (metre)*/

	/*
	unknown chunks
	There are 3 buffers, one for each position in the PNG where unknown chunks can appear
	each buffer contains all unknown chunks for that position consecutively
	The 3 buffers are the unknown chunks between certain critical chunks:
	0: IHDR-PLTE, 1: PLTE-IDAT, 2: IDAT-IEND
	Do not allocate or traverse this data yourself. Use the chunk traversing functions declared
	later, such as lodepng_chunk_next and lodepng_chunk_append, to read/write this struct.
	*/
	unsigned char* unknown_chunks_data[3];
	size_t unknown_chunks_size[3]; /*size in bytes of the unknown chunks, given for protection*/
};

#endif //LIBTCOD_INFO_HPP
