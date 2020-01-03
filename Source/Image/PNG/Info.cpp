#include "Image/PNG/Info.hpp"

LodePNGInfo::LodePNGInfo()
{
	for (unsigned i = 0; i < 3; ++i)
	{
		unknown_chunks_data[i] = nullptr;
		unknown_chunks_size[i] = 0;
	}
}
