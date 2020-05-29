#ifndef LIBTCOD_MOUSE_HPP
#define LIBTCOD_MOUSE_HPP

namespace Doryen
{
	enum class MouseCode : short
	{
		NONE,

		LEFT,
		MIDDLE,
		RIGHT,

		WHEEL_UP,
		WHEEL_DOWN,
	};

	class Mouse
	{

	private:

		int x = 0;
		int y = 0;

		int dx = 0;
		int dy = 0;

		int cx = 0;
		int cy = 0;

		int dcx = 0;
		int dcy = 0;

		/**
		 * Determine if the button left, right, middle was pressed.
		 * Determine if wheel up or wheel down was pressed (aka: moved).
		 */
		MouseCode status = MouseCode::NONE;

	public:

	};
}

#endif //LIBTCOD_MOUSE_HPP
