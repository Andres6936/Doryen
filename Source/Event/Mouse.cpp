// Joan Andrés (@Andres6936) Github.

#include "Doryen/Event/Mouse.hpp"

using namespace Doryen;

// Getters

int Mouse::getX() const
{
	return x;
}

int Mouse::getY() const
{
	return y;
}

int Mouse::getMovementRelativeX() const
{
	return movementRelativeX;
}

int Mouse::getMovementRelativeY() const
{
	return movementRelativeY;
}

int Mouse::getCx() const
{
	return cx;
}

int Mouse::getCy() const
{
	return cy;
}

int Mouse::getDcx() const
{
	return dcx;
}

int Mouse::getDcy() const
{
	return dcy;
}

MouseCode Mouse::getStatus() const
{
	return status;
}

// Setters

void Mouse::setX(int _x)
{
	x = _x;
}

void Mouse::setY(int _y)
{
	y = _y;
}

void Mouse::setDx(int _dx)
{
	movementRelativeX = _dx;
}

void Mouse::setDy(int _dy)
{
	movementRelativeY = _dy;
}

void Mouse::setCx(int _cx)
{
	cx = _cx;
}

void Mouse::setCy(int _cy)
{
	cy = _cy;
}

void Mouse::setDcx(int _dcx)
{
	dcx = _dcx;
}

void Mouse::setDcy(int _dcy)
{
	dcy = _dcy;
}

void Mouse::setStatus(MouseCode _status)
{
	status = _status;
}

// Methods

void Mouse::addDx(int _dx)
{
	movementRelativeX += _dx;
}

void Mouse::addDy(int _dy)
{
	movementRelativeY += _dy;
}

bool Mouse::isPressedLeftButton() const
{
	return status == MouseCode::LEFT;
}

bool Mouse::isPressedRightButton() const
{
	return status == MouseCode::RIGHT;
}

bool Mouse::isPressedMiddleButton() const
{
	return status == MouseCode::MIDDLE;
}
