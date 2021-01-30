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

void Doryen::Cell::setFov(bool isFov)
{
	fov = isFov;
}

void Doryen::Cell::setTraversable(bool isTraversable)
{
	traversable = isTraversable;
}

void Doryen::Cell::setTransparent(bool isTransparent)
{
	transparent = isTransparent;
}
