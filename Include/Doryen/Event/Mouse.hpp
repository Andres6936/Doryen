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

		/**
		 * The movement relative of mouse in the coordinate in X
		 * from the last event processed and send to user.
		 *
		 * @note Set to 0 each time that the events are processed.
		 */
		int movementRelativeX = 0;

		/**
		 * The movement relative of mouse in the coordinate in Y
		 * from the last event processed and send to user.
		 *
		 * @note Set to 0 each time that the events are processed.
		 */
		int movementRelativeY = 0;

		int positionCellX = 0;
		int positionCellY = 0;

		int dcx = 0;
		int dcy = 0;

		/**
		 * Determine if the button left, right, middle was pressed.
		 * Determine if wheel up or wheel down was pressed (aka: moved).
		 */
		MouseCode status = MouseCode::NONE;

	public:

		// Methods

		void addDx(int _dx);

		void addDy(int _dy);

		bool isPressedLeftButton() const;

		bool isPressedRightButton() const;

		bool isPressedMiddleButton() const;

		// Getters

		int getX() const;

		int getY() const;

		int getMovementRelativeX() const;

		int getMovementRelativeY() const;

		int getPositionCellX() const;

		int getPositionCellY() const;

		int getDcx() const;

		int getDcy() const;

		MouseCode getStatus() const;

		// Setters

		void setX(int _x);

		void setY(int _y);

		void setMovementRelativeX(int _dx);

		void setMovementRelativeY(int _dy);

		void setPositionCellX(int _cx);

		void setPositionCellY(int _cy);

		void setDcx(int _dcx);

		void setDcy(int _dcy);

		void setStatus(MouseCode _status);

	};
}

#endif //LIBTCOD_MOUSE_HPP
