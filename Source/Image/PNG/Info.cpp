#include "Image/PNG/Info.hpp"

LodePNGInfo::LodePNGInfo()
{
	for (unsigned i = 0; i < 3; ++i)
	{
		unknown_chunks_data[i] = nullptr;
		unknown_chunks_size[i] = 0;
	}
}

void LodePNGInfo::init()
{
	interlace_method = 0;
	compression_method = 0;
	filter_method = 0;
	background_defined = 0;
	background_r = 0;
	background_g = 0;
	background_b = 0;

	text_num = 0;
	text_keys = nullptr;
	text_strings = nullptr;

	itext_num = 0;
	itext_keys = nullptr;
	itext_langtags = nullptr;
	itext_transkeys = nullptr;
	itext_strings = nullptr;

	for (unsigned i = 0; i < 3; ++i)
	{
		unknown_chunks_data[i] = nullptr;
		unknown_chunks_size[i] = 0;
	}

	time_defined = 0;
	phys_defined = 0;
}

void LodePNGInfo::cleanup()
{
	color.cleanup();

	for (unsigned int i = 0; i < text_num; ++i)
	{
		delete[] text_keys[i];
		delete[] text_strings[i];
	}

	delete[] text_keys;
	delete[] text_strings;

	for (unsigned int i = 0; i < itext_num; ++i)
	{
		delete[] itext_keys[i];
		delete[] itext_langtags[i];
		delete[] itext_transkeys[i];
		delete[] itext_strings[i];
	}

	delete[] itext_keys;
	delete[] itext_langtags;
	delete[] itext_transkeys;
	delete[] itext_strings;

	for (unsigned int i = 0; i < 3; ++i)
	{
		delete[] unknown_chunks_data[i];
	}
}
