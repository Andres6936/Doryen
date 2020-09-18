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

		void resetState() noexcept;

		bool isWheelUp() const noexcept;

		bool isWheelDown() const noexcept;

		bool isPressedLeftButton() const noexcept;

		bool isPressedRightButton() const noexcept;

		bool isPressedMiddleButton() const noexcept;

		// Getters

		int getX() const noexcept;

		int getY() const noexcept;

		int getPositionCellX() const noexcept;

		int getPositionCellY() const noexcept;

		MouseCode getStatus() const noexcept;

		// Setters

		void setX(int _x) noexcept;

		void setY(int _y) noexcept;

		void setPositionCellX(int _cx) noexcept;

		void setPositionCellY(int _cy) noexcept;

		void setStatus(MouseCode _status) noexcept;

	};
}

#endif //LIBTCOD_MOUSE_HPP
