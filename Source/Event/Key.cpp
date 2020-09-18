#include "Doryen/Event/Key.hpp"

using namespace Doryen;

// Getters

KeyCode Key::getKeyCode() const noexcept
{
	return keyCode;
}

bool Key::isPressed() const noexcept
{
	return pressed;
}

bool Key::isRelease() const noexcept
{
	return release;
}

bool Key::isLeftAltPressed() const noexcept
{
	return leftAltPressed;
}

bool Key::isLeftCtrlPressed() const noexcept
{
	return leftCtrlPressed;
}

bool Key::isRightAltPressed() const noexcept
{
	return rightAltPressed;
}

bool Key::isRightCtrlPressed() const noexcept
{
	return rightCtrlPressed;
}

bool Key::isShift() const noexcept
{
	return shift;
}

// Setters

void Key::setKeyCode(KeyCode _keyCode) noexcept
{
	keyCode = _keyCode;
}

void Key::setPressed(bool _pressed) noexcept
{
	pressed = _pressed;
}

void Key::setRelease(bool _release) noexcept
{
	release = _release;
}

void Key::setLeftAltPressed(bool _leftAltPressed) noexcept
{
	leftAltPressed = _leftAltPressed;
}

void Key::setLeftCtrlPressed(bool _leftCtrlPressed) noexcept
{
	leftCtrlPressed = _leftCtrlPressed;
}

void Key::setRightAltPressed(bool _rightAltPressed) noexcept
{
	rightAltPressed = _rightAltPressed;
}

void Key::setRightCtrlPressed(bool _rightCtrlPressed) noexcept
{
	rightCtrlPressed = _rightCtrlPressed;
}

void Key::setShift(bool _shift) noexcept
{
	shift = _shift;
}

void Key::resetState() noexcept
{
	keyCode = KeyCode::NONE;
}
