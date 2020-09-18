#include "Doryen/Event/Key.hpp"

// Getters

Doryen::KeyCode Doryen::Key::getKeyCode() const
{
	return keyCode;
}

bool Doryen::Key::isPressed() const
{
	return pressed;
}

bool Doryen::Key::isRelease() const
{
	return release;
}

bool Doryen::Key::isLeftAltPressed() const
{
	return leftAltPressed;
}

bool Doryen::Key::isLeftCtrlPressed() const
{
	return leftCtrlPressed;
}

bool Doryen::Key::isRightAltPressed() const
{
	return rightAltPressed;
}

bool Doryen::Key::isRightCtrlPressed() const
{
	return rightCtrlPressed;
}

bool Doryen::Key::isShift() const
{
	return shift;
}

// Setters

void Doryen::Key::setKeyCode(Doryen::KeyCode _keyCode)
{
	keyCode = _keyCode;
}

void Doryen::Key::setPressed(bool _pressed)
{
	pressed = _pressed;
}

void Doryen::Key::setRelease(bool _release)
{
	release = _release;
}

void Doryen::Key::setLeftAltPressed(bool _leftAltPressed)
{
	leftAltPressed = _leftAltPressed;
}

void Doryen::Key::setLeftCtrlPressed(bool _leftCtrlPressed)
{
	leftCtrlPressed = _leftCtrlPressed;
}

void Doryen::Key::setRightAltPressed(bool _rightAltPressed)
{
	rightAltPressed = _rightAltPressed;
}

void Doryen::Key::setRightCtrlPressed(bool _rightCtrlPressed)
{
	rightCtrlPressed = _rightCtrlPressed;
}

void Doryen::Key::setShift(bool _shift)
{
	shift = _shift;
}

void Doryen::Key::resetState()
{
	keyCode = KeyCode::NONE;
}
