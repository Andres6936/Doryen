#ifndef LIBTCOD_PIXEL_HPP
#define LIBTCOD_PIXEL_HPP

#include <vector>
#include <cstdint>

#include "Doryen/Graphics/Color/Color.hpp"

namespace Doryen
{

	/**
	 * In computer graphics, mipmaps (also MIP maps) or pyramids are pre-calculated,
	 * optimized sequences of images, each of which is a progressively lower resolution
	 * representation of the previous.
	 *
	 * The height and width of each image, or level, in the mipmap is a power of two
	 * smaller than the previous level. Mipmaps do not have to be square. They are
	 * intended to increase rendering speed and reduce aliasing artifacts.
	 *
	 * A high-resolution mipmap image is used for objects that are close to the user.
	 * Lower-resolution images are used as the object appears farther away. Mipmapping
	 * improves the quality of rendered textures at the expense of using more memory.
	 */
	class Mipmap : public std::vector<Color>
	{

	public:

		std::uint32_t width = 0;
		std::uint32_t height = 0;

		float fwidth = 0.0f;
		float fheight = 0.0f;

		bool dirty = false;

		// Static Methods

		/**
		 * Original size is: 92 x 40
		 * 	Resize to: 46 x 20
		 * 	Resize to: 23 x 10
		 * 	Resize to: 11 x 5
		 * 	Resize to: 5 x 2
		 * 	Resize to: 2 x 1
		 * 	Resize to: 1 x 0
		 * End resize, total levels is: 6
		 *
		 * @return The number of texture levels in a multilevel texture.
		 */
		static std::uint32_t getLevelCount(const std::uint32_t width, const std::uint32_t height) noexcept;

		// Getter

		const Color& getColorAt(int _x, int _y) const noexcept;

		// Setter

		void setDirty(bool _dirty) noexcept;

		void setColorAt(int _x, int _y, const Color& _color) noexcept;
	};
}


#endif //LIBTCOD_PIXEL_HPP
