#include "Doryen/Algorithms/Util/Cell.hpp"

Doryen::Cell::Cell(bool isTransparent, bool isWalkable, bool isFOV) noexcept
{
	transparent = isTransparent;
	traversable = isWalkable;
	fov = isFOV;
}

// Getters

bool Doryen::Cell::isFov() const
{
	return fov;
}

bool Doryen::Cell::isTraversable() const
{
	return traversable;
}

bool Doryen::Cell::isTransparent() const
{
	return transparent;
}

// Setters

void Doryen::Cell::setFov(bool _fov)
{
	fov = _fov;
}

void Doryen::Cell::setTraversable(bool _traversable)
{
	traversable = _traversable;
}

void Doryen::Cell::setTransparent(bool _transparent)
{
	transparent = _transparent;
}
