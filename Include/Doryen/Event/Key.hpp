#ifndef LIBTCOD_KEY_HPP
#define LIBTCOD_KEY_HPP

namespace Doryen
{
	enum class KeyCode : short
	{
		NONE,
		ESCAPE,
		BACKSPACE,
		TAB,
		ENTER,
		SHIFT,
		CONTROL,
		ALT,
		PAUSE,
		PAGE_UP,
		PAGE_DOWN,
		END,
		HOME,
		UP,
		LEFT,
		RIGHT,
		DOWN,
		PRINT_SCREEN,
		DELETE,
		SPACE,

		A,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		W,
		X,
		Y,
		Z,

		K_0,
		K_1,
		K_2,
		K_3,
		K_4,
		K_5,
		K_6,
		K_7,
		K_8,
		K_9,

		KP_0,
		KP_1,
		KP_2,
		KP_3,
		KP_4,
		KP_5,
		KP_6,
		KP_7,
		KP_8,
		KP_9,

		KP_ADD,
		KP_SUB,
		KP_DIV,
		KP_MUL,
		KP_DEC,
		KP_ENTER,

		F1,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,
	};

	/**
	 * Key data : special code or character
	 */
	class Key
	{

	private:

		KeyCode keyCode = KeyCode::NONE;

		bool pressed = false;

		bool release = false;

		bool leftAltPressed = false;

		bool leftCtrlPressed = false;

		bool rigthAltPressed = false;

		bool rigthCtrlPressed = false;

		bool shift = false;

	public:

		// Methods

		void resetState();

		// Getters

		KeyCode getKeyCode() const;

		bool isPressed() const;

		bool isRelease() const;

		bool isLeftAltPressed() const;

		bool isLeftCtrlPressed() const;

		bool isRigthAltPressed() const;

		bool isRigthCtrlPressed() const;

		bool isShift() const;

		// Setter

		void setKeyCode(KeyCode _keyCode);

		void setPressed(bool _pressed);

		void setRelease(bool _release);

		void setLeftAltPressed(bool _leftAltPressed);

		void setLeftCtrlPressed(bool _leftCtrlPressed);

		void setRigthAltPressed(bool _rigthAltPressed);

		void setRigthCtrlPressed(bool _rigthCtrlPressed);

		void setShift(bool _shift);

	};
}

#endif //LIBTCOD_KEY_HPP
