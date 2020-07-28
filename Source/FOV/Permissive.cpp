#include "Doryen/FOV/Permissive.hpp"

void
Doryen::Permissive::operator()(Doryen::Map& map, int playerX, int playerY, int maxRadius, bool ligthWalls, int fovType)
{
	source = { static_cast<int16_t>(playerX), static_cast<int16_t>(playerY) };
	rangeLimit = maxRadius;

	for (int q = 0; q < 4; ++q)
	{
		quadrant.x = static_cast<std::int16_t>(q == 0 or q == 3 ? 1 : -1);
		quadrant.y = static_cast<std::int16_t>(q < 2 ? 1 : -1);

		computeQuadrant();
	}
}

void Doryen::Permissive::computeQuadrant()
{

}
