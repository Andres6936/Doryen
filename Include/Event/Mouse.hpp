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

		// Methods

		void addDx(int _dx);

		void addDy(int _dy);

		bool isPressedLeftButton() const;

		bool isPressedRightButton() const;

		bool isPressedMiddleButton() const;

		// Getters

		int getX() const;

		int getY() const;

		int getDx() const;

		int getDy() const;

		int getCx() const;

		int getCy() const;

		int getDcx() const;

		int getDcy() const;

		MouseCode getStatus() const;

		// Setters

		void setX(int _x);

		void setY(int _y);

		void setDx(int _dx);

		void setDy(int _dy);

		void setCx(int _cx);

		void setCy(int _cy);

		void setDcx(int _dcx);

		void setDcy(int _dcy);

		void setStatus(MouseCode _status);

	};
}

#endif //LIBTCOD_MOUSE_HPP
