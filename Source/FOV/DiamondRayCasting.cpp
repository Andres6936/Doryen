#include <cmath>

#include "Doryen/FOV/DiamondRayCasting.hpp"

using namespace Doryen;

void DiamondRayCasting::operator()(
		Doryen::Map& map, int playerX, int playerY,
		int maxRadius, bool ligthWalls)
{
	const std::int32_t r2 = maxRadius * maxRadius;

	// Field of view origin
	const std::int32_t originX = playerX;
	const std::int32_t originY = playerY;

	std::vector<RayData> perim;

	perim.resize(map.getWidth() * map.getHeight());
	raymap.resize(map.getWidth() * map.getHeight());
	raymap2.resize(map.getWidth() * map.getHeight());

	expandPerimeterFrom(map, perim, newRay(map,
			{0, 0}, {originX, originY}));

	std::uint32_t perimidx = 0;

	while (perimidx < perim.size())
	{
		Iterator ray = perim.begin() + perimidx;

		int distance = r2 > 0 ? std::pow(ray->position.x, 2) + std::pow(ray->position.y, 2) : 0;

		perimidx += 1;

		if (distance <= r2)
		{

		}
		else
		{
			ray->ignore = true;
		}
	}
}

void DiamondRayCasting::expandPerimeterFrom(
		Map& map, std::vector<RayData>& perim, std::optional<Iterator> ray)
{

}

std::optional<DiamondRayCasting::Iterator> DiamondRayCasting::newRay(
		Map& _map, const Geometry::Point2D<>& _coordinate,
		const Geometry::Point2D<>& _origin)
{
	if (_coordinate.x + _origin.x >= _map.getWidth() or
		_coordinate.y + _origin.y >= _map.getHeight())
	{
		return std::nullopt;
	}

	// Else

	Iterator it = raymap2.begin() + (_coordinate.x + _origin.x +
									 (_coordinate.y + _origin.y) * _map.getWidth());

	(*it).position = _coordinate;

	return it;
}
