#ifndef LIBTCOD_PIXEL_HPP
#define LIBTCOD_PIXEL_HPP

#include <vector>
#include "Color.hpp"

namespace Doryen
{
	class Mipmap
	{

	public:

		unsigned int width = 0;
		unsigned int height = 0;

		float fwidth = 0.0f;
		float fheight = 0.0f;

		std::vector <Color> buf;

		bool dirty = false;

		static unsigned int getLevels(
				unsigned int width,
				unsigned int height);
	};
}


#endif //LIBTCOD_PIXEL_HPP
