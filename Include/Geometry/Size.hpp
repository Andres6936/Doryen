// Design by Joan Andrés.

#ifndef LIBTCOD_SIZE_HPP
#define LIBTCOD_SIZE_HPP

class Size final
{

public:

	/**
	 * Width
	 */
	int w = 0;

	/**
	 * Height
	 */
	int h = 0;

	Size() = default;

	/**
	 * @param _w Width
	 * @param _h Height
	 */
	Size(int _w, int _h) noexcept
	{
		w = _w;
		h = _h;
	}
};

#endif //LIBTCOD_SIZE_HPP
