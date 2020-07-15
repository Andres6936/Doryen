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

int Mouse::getPositionCellX() const
{
	return positionCellX;
}

int Mouse::getPositionCellY() const
{
	return positionCellY;
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

void Mouse::setPositionCellX(int _cx)
{
	positionCellX = _cx;
}

void Mouse::setPositionCellY(int _cy)
{
	positionCellY = _cy;
}

void Mouse::setStatus(MouseCode _status)
{
	status = _status;
}

// Methods

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

void Mouse::resetState()
{
	status = MouseCode::NONE;
}
