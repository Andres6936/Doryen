#include <Platform/Platform.hpp>
#include "Renderer/Renderer.hpp"

// Static Members

const std::array <int, 256> Doryen::Renderer::init_ascii_to_tcod =

		{
				// ASCII 0 to 15
				0, 0, 0, 0, 0, 0, 0, 0, 0, 76, 77, 0, 0, 0, 0, 0,
				// ASCII 16 to 31
				71, 70, 72, 0, 0, 0, 0, 0, 64, 65, 67, 66, 0, 73, 68, 69,
				//  ASCII 32 to 47
				0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
				// ASCII 48 to 63
				16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
				// ASCII 64 to 79
				32, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110,
				// ASCII 80 to 95
				111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 33, 34, 35, 36, 37,
				// ASCII 96 to 111
				38, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142,
				// ASCII 112 to 127
				143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 39, 40, 41, 42, 0,
				// ASCII 128 to 143
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				// ASCII 144 to 159
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				// ASCII 160 to 175
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				// ASCII 176 to 191
				43, 44, 45, 46, 49, 0, 0, 0, 0, 81, 78, 87, 88, 0, 0, 55,
				// ASCII 192 to 207
				53, 50, 52, 51, 47, 48, 0, 0, 85, 86, 82, 84, 83, 79, 80, 0,
				// ASCII 208 to 223
				0, 0, 0, 0, 0, 0, 0, 0, 0, 56, 54, 0, 0, 0, 0, 0,
				// ASCII 224 to 239
				74, 75, 57, 58, 59, 60, 61, 62, 63, 0, 0, 0, 0, 0, 0, 0,
				// ASCII 240 to 255
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		};

// Methods

void Doryen::Renderer::clearAndResizeASCII(unsigned size)
{
	ascii_updated.clear();
	ascii_updated.resize(size, false);
}

void Doryen::Renderer::clearAndResizeFirstDraw(unsigned size)
{
	first_draw.clear();
	first_draw.resize(size, true);
}

void Doryen::Renderer::clearAndResizeCharcols(unsigned size)
{
	charcols.clear();
	charcols.resize(size);
}
