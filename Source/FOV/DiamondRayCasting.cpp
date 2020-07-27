#include <cmath>

#include "Doryen/FOV/DiamondRayCasting.hpp"

using namespace Doryen;

void DiamondRayCasting::operator()(
		Doryen::Map& map, int playerX, int playerY,
		int maxRadius, bool ligthWalls)
{
	map.setProperties(playerX, playerY, true, true);

	for (int octant = 0; octant < 8; ++octant)
	{
		compute(octant, { playerX, playerY }, maxRadius, 1, { 1, 1 }, { 0, 1 });
	}
}

void
DiamondRayCasting::compute(std::uint32_t octant, Geometry::Point2D<> origin,
		std::uint32_t rangeLimit, std::uint32_t x,
		DiamondRayCasting::Slope top, DiamondRayCasting::Slope bottom)
{
	for (; x <= rangeLimit; ++x)
	{
		std::int32_t topY = 0;

		if (top.x == 1)
		{
			topY = x;
		}
		else
		{
			topY = ((x * 2 - 1) * top.y + top.x) / (top.x * 2);
			std::int32_t ay = (topY * 2 + 1) * top.x;

			if (blocksLight(x, topY, octant, origin))
			{
				if (top.greaterOrEqual(ay, x * 2))
				{
					topY += 1;
				}
			}
			else
			{
				if (top.greater(ay, x * 2 + 1))
				{
					topY += 1;
				}
			}
		}

		std::int32_t bottomY;

		if (bottom.x == 0)
		{
			bottomY = 0;
		}
		else
		{
			bottomY = bottom.y == 0 ? 0 : ((x * 2 - 1) * bottom.y + bottom.x) / (bottom.x * 2);
		}

		std::int32_t wasOpaque = -1; // 0:false, 1:true, -1:not applicable

		for (int y = topY; y >= bottomY; --y)
		{
			std::int32_t tx = origin.x, ty = origin.y;

			switch (octant) // translate local coordinates to map coordinates
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

			bool inRange = rangeLimit < 0 || getDistance(tx, ty) <= rangeLimit;

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
					if (wasOpaque == 0)
					{
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
				else
				{
					if (wasOpaque > 0)
					{
						top = { y * 2 + 1, static_cast<int32_t>(x * 2) };
					}

					wasOpaque = 0;
				}
			}
		}

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

std::int32_t DiamondRayCasting::getDistance(const std::int32_t a,
		const std::int32_t b)
{
	if (not(a >= 0))
	{
		throw std::logic_error("Invariant not satisfied A is zero.");
	}

	if (not(b >= 0))
	{
		throw std::logic_error("Invariant not satisfied B is zero.");
	}

	if (not(a >= b))
	{
		throw std::logic_error("Invariant not satisfied A not is greater that B.");
	}

	return std::sqrt(std::pow(a, 2) + std::pow(b, 2));
}
