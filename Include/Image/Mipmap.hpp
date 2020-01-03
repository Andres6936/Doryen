#ifndef LIBTCOD_PIXEL_HPP
#define LIBTCOD_PIXEL_HPP

#include <vector>
#include "Color.hpp"

namespace Doryen
{
	class Mipmap
	{

	public:

		int width;
		int height;

		float fwidth;
		float fheight;

		std::vector <Color> buf;

		bool dirty;
	};
}


#endif //LIBTCOD_PIXEL_HPP
