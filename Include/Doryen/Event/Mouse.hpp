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

		/**
		 * Position in pixels relative to main console (root).
		 */
		int x = 0;

		/**
		 * Position in pixels relative to main console (root).
		 */
		int y = 0;

		/**
		 * Position in the cell relative to coordiante X of
		 * main console (root).
		 */
		int positionCellX = 0;

		/**
		 * Position in the cell relative to coordinate Y of
		 * main console (root).
		 */
		int positionCellY = 0;

		/**
		 * Determine if the button left, right, middle was pressed.
		 * Determine if wheel up or wheel down was pressed (aka: moved).
		 */
		MouseCode status = MouseCode::NONE;

	public:

		// Methods

		void resetState();

		bool isWheelUp() const;

		bool isWheelDown() const;

		bool isPressedLeftButton() const;

		bool isPressedRightButton() const;

		bool isPressedMiddleButton() const;

		// Getters

		int getX() const;

		int getY() const;

		int getPositionCellX() const;

		int getPositionCellY() const;

		MouseCode getStatus() const;

		// Setters

		void setX(int _x);

		void setY(int _y);

		void setPositionCellX(int _cx);

		void setPositionCellY(int _cy);

		void setStatus(MouseCode _status);

	};
}

#endif //LIBTCOD_MOUSE_HPP
