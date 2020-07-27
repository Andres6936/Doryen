#include <cmath>
#include <iostream>

#include "Doryen/FOV/DiamondRayCasting.hpp"

using namespace Doryen;

void DiamondRayCasting::operator()(
		int playerX, int playerY,
		int maxRadius, bool ligthWalls)
{
	_map.resetFieldView();
	_map.setVisibleFieldView(playerX, playerY);

	for (int octant = 0; octant < 8; ++octant)
	{
		compute(octant, { playerX, playerY }, maxRadius == 0 ? -1 : maxRadius, 1, { 1, 1 }, { 0, 1 });
	}
}

void
DiamondRayCasting::compute(std::uint32_t octant, Geometry::Point2D<> origin,
		std::uint32_t rangeLimit, std::uint32_t x,
		DiamondRayCasting::Slope top, DiamondRayCasting::Slope bottom)
{
	// rangeLimit < 0 || x <= rangeLimit
	for (; x <= rangeLimit; ++x)
	{
		std::int32_t topY = 0;

		if (top.x == 1)
		{
			topY = x;
		}
		else
		{
			// get the tile that the top vector enters from the left
			topY = ((x * 2 - 1) * top.y + top.x) / (top.x * 2);
			const std::int32_t ay = (topY * 2 + 1) * top.x;

			// if the top tile is a wall...
			if (blocksLight(x, topY, octant, origin))
			{
				if (top.greaterOrEqual(ay, x * 2))
				{
					// but the top vector misses the wall and passes into the tile above, move up
					topY += 1;
				}
			}
				// the top tile is not a wall
			else
			{
				if (top.greater(ay, x * 2 + 1))
				{
					// so if the top vector passes into the tile above, move up
					topY += 1;
				}
			}
		}

		const std::int32_t bottomY = bottom.y == 0 ? 0 : ((x * 2 - 1) * bottom.y + bottom.x) / (bottom.x * 2);

		// 0:false, 1:true, -1:not applicable
		std::int32_t wasOpaque = -1;

		for (int y = topY; y >= bottomY; --y)
		{
			std::int32_t tx = origin.x, ty = origin.y;

			// translate local coordinates to map coordinates
			switch (octant)
			{
			case 0:
				tx += x;
				ty -= y;
				break;
			case 1:
				tx += y;
				ty -= x;
				break;
			case 2:
				tx -= y;
				ty -= x;
				break;
			case 3:
				tx -= x;
				ty -= y;
				break;
			case 4:
				tx -= x;
				ty += y;
				break;
			case 5:
				tx -= y;
				ty += x;
				break;
			case 6:
				tx += y;
				ty += x;
				break;
			case 7:
				tx += x;
				ty += y;
				break;
			}

			const bool inRange = rangeLimit < 0 || getDistance(tx, ty) <= rangeLimit;

			if (inRange)
			{
				_map.setVisibleFieldView(tx, ty);
			}

			bool isOpaque = !inRange || not _map.isTransparent(tx, ty);
			// if y == topY or y == bottomY, make sure the sector actually intersects the wall tile. if not, don't consider
			// it opaque to prevent the code below from moving the top vector up or the bottom vector down
			if (isOpaque &&
				(y == topY && top.lessOrEqual(y * 2 - 1, x * 2) && not blocksLight(x, y - 1, octant, origin) ||
				 y == bottomY && bottom.greaterOrEqual(y * 2 + 1, x * 2) && not blocksLight(x, y + 1, octant, origin)))
			{
				isOpaque = false;
			}

			if (x not_eq rangeLimit)
			{
				if (isOpaque)
				{
					// if we found a transition from clear to opaque, this sector is done in this column, so
					// adjust the bottom vector upwards and continue processing it in the next column.
					if (wasOpaque == 0)
					{
						// (x * 2 - 1, y * 2 + 1) is a vector to the top-left corner of the opaque block
						if (!inRange || y == bottomY)
						{
							bottom = { y * 2 + 1, static_cast<int32_t>(x * 2) };
							break;
						}
							// don't recurse unless necessary
						else
						{
							compute(octant, origin, rangeLimit, x + 1, top,
									{ y * 2 + 1, static_cast<int32_t>(x * 2) });
						}
					}

					wasOpaque = 1;
				}
					// adjust the top vector downwards and continue if we found a transition from opaque to clear
				else
				{
					// (x * 2 + 1, y * 2 + 1) is the top-right corner of the clear tile (i.e. the bottom-right
					// of the opaque tile)
					if (wasOpaque > 0)
					{
						top = { y * 2 + 1, static_cast<int32_t>(x * 2) };
					}

					wasOpaque = 0;
				}
			}
		}

		// if the column ended in a clear tile, continue processing the current sector
		if (wasOpaque not_eq 0) break;
	}
}

bool
DiamondRayCasting::blocksLight(std::int32_t x, std::int32_t y,
		std::uint32_t octant, const Geometry::Point2D<>& _origin)
{
	int nx = _origin.x, ny = _origin.y;

	switch (octant)
	{
	case 0:
		nx += x;
		ny -= y;
		break;
	case 1:
		nx += y;
		ny -= x;
		break;
	case 2:
		nx -= y;
		ny -= x;
		break;
	case 3:
		nx -= x;
		ny -= y;
		break;
	case 4:
		nx -= x;
		ny += y;
		break;
	case 5:
		nx -= y;
		ny += x;
		break;
	case 6:
		nx += y;
		ny += x;
		break;
	case 7:
		nx += x;
		ny += y;
		break;
	}

	return _map.isTransparent(nx, ny);
}

std::int32_t DiamondRayCasting::getDistance(std::int32_t a, std::int32_t b)
{
	if (not(a >= 0))
	{
		a = 0;
	}

	if (not(b >= 0))
	{
		b = 0;
	}

	return std::sqrt(std::pow(a, 2) + std::pow(b, 2));
}
