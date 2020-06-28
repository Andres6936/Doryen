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

int Mouse::getPositionCellX() const
{
	return positionCellX;
}

int Mouse::getPositionCellY() const
{
	return positionCellY;
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

void Mouse::setMovementRelativeX(int _dx)
{
	movementRelativeX = _dx;
}

void Mouse::setMovementRelativeY(int _dy)
{
	movementRelativeY = _dy;
}

void Mouse::setPositionCellX(int _cx)
{
	positionCellX = _cx;
}

void Mouse::setPositionCellY(int _cy)
{
	positionCellY = _cy;
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
