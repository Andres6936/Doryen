#ifndef LIBTCOD_PIXEL_HPP
#define LIBTCOD_PIXEL_HPP

#include <vector>
#include <cstdint>

#include "Doryen/Graphics/Color/Color.hpp"

namespace Doryen
{

	class Mipmap : public std::vector<Color>
	{

	public:

		std::uint32_t width = 0;
		std::uint32_t height = 0;

		float fwidth = 0.0f;
		float fheight = 0.0f;

		bool dirty = false;

		// Getter

		Color getColorAt(int _x, int _y) const;

		// Setter

		void setDirty(bool _dirty);

		void setColorAt(int _x, int _y, const Color& _color);

		// Static Methods

		static unsigned int getLevels(
				unsigned int width,
				unsigned int height);
	};
}


#endif //LIBTCOD_PIXEL_HPP
