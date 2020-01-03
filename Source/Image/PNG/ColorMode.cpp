#include "Image/PNG/ColorMode.hpp"

void LodePNGColorMode::cleanup()
{
	delete[] palette;
	palette = nullptr;
	palettesize = 0;
}
