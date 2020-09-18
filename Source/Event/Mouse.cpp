// Joan Andrés (@Andres6936) Github.

#include "Doryen/Event/Mouse.hpp"

using namespace Doryen;

// Getters

int Mouse::getX() const noexcept
{
	return x;
}

int Mouse::getY() const noexcept
{
	return y;
}

int Mouse::getPositionCellX() const noexcept
{
	return positionCellX;
}

int Mouse::getPositionCellY() const noexcept
{
	return positionCellY;
}

MouseCode Mouse::getStatus() const noexcept
{
	return status;
}

// Setters

void Mouse::setX(int _x) noexcept
{
	x = _x;
}

void Mouse::setY(int _y) noexcept
{
	y = _y;
}

void Mouse::setPositionCellX(int _cx) noexcept
{
	positionCellX = _cx;
}

void Mouse::setPositionCellY(int _cy) noexcept
{
	positionCellY = _cy;
}

void Mouse::setStatus(MouseCode _status) noexcept
{
	status = _status;
}

// Methods

bool Mouse::isPressedLeftButton() const noexcept
{
	return status == MouseCode::LEFT;
}

bool Mouse::isPressedRightButton() const noexcept
{
	return status == MouseCode::RIGHT;
}

bool Mouse::isPressedMiddleButton() const noexcept
{
	return status == MouseCode::MIDDLE;
}

void Mouse::resetState() noexcept
{
	status = MouseCode::NONE;
}

bool Mouse::isWheelUp() const noexcept
{
	return status == MouseCode::WHEEL_UP;
}

bool Mouse::isWheelDown() const noexcept
{
	return status == MouseCode::WHEEL_DOWN;
}
