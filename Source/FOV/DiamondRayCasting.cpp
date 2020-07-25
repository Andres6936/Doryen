#include "Doryen/FOV/DiamondRayCasting.hpp"

void Doryen::DiamondRayCasting::operator()(
		Doryen::Map& map, int playerX, int playerY,
		int maxRadius, bool ligthWalls)
{
	const std::int32_t r2 = maxRadius * maxRadius;

	// Field of view origin
	const std::int32_t originX = playerX;
	const std::int32_t originY = playerY;


}
