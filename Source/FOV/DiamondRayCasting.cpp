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

void DiamondRayCasting::mergeInput(Map& _map, const Geometry::Point2D<>& _origin,
		DiamondRayCasting::Iterator _ray)
{
	if (_ray->xInput not_eq nullptr)
	{
		processXInput(_ray, _ray->xInput);
	}

	if (_ray->yInput not_eq nullptr)
	{
		processYInput(_ray, _ray->yInput);
	}

	if (_ray->xInput == nullptr)
	{
		if (isObscure(_ray->yInput)) _ray->ignore = true;
	}
	else if (_ray->yInput == nullptr)
	{
		if (isObscure(_ray->xInput)) _ray->ignore = true;
	}
	else if (isObscure(_ray->xInput) and isObscure(_ray->yInput))
	{
		_ray->ignore = true;
	}

	if (not _ray->ignore and not _map.isTransparent(
			_ray->position.x + _origin.x, _ray->position.y + _origin.y))
	{
		_ray->bresenham.x = _ray->obscurity.x = std::abs(_ray->position.x);
		_ray->bresenham.y = _ray->obscurity.y = std::abs(_ray->position.y);
	}
}

void DiamondRayCasting::processXInput(DiamondRayCasting::Iterator newRay,
		std::shared_ptr<RayData> xInput)
{
	if (xInput->obscurity.equals({0, 0}))
	{
		return;
	}

	if (xInput->bresenham.x > 0 and newRay->obscurity.x == 0)
	{
		newRay->bresenham.x = xInput->bresenham.x - xInput->obscurity.y;
		newRay->bresenham.y = xInput->bresenham.y + xInput->obscurity.y;
		newRay->obscurity = xInput->obscurity;
	}

	if (xInput->bresenham.y <= 0 and newRay->obscurity.y > 0 and newRay->bresenham.x > 0)
	{
		newRay->bresenham.x = xInput->bresenham.x - xInput->obscurity.y;
		newRay->bresenham.y = xInput->bresenham.y + xInput->obscurity.y;
		newRay->obscurity = xInput->obscurity;
	}
}

void DiamondRayCasting::processYInput(DiamondRayCasting::Iterator newRay,
		std::shared_ptr<RayData> yInput)
{
	if (yInput->obscurity.equals({ 0, 0}))
	{
		return;
	}

	if (yInput->bresenham.y > 0 and newRay->obscurity.y == 0)
	{
		newRay->bresenham.y = yInput->bresenham.y - yInput->obscurity.x;
		newRay->bresenham.x = yInput->bresenham.x + yInput->obscurity.x;
		newRay->obscurity = yInput->obscurity;
	}

	if (yInput->bresenham.x <= 0 and yInput->obscurity.x > 0 and yInput->bresenham.y > 0)
	{
		newRay->bresenham.y = yInput->bresenham.y - yInput->obscurity.x;
		newRay->bresenham.x = yInput->bresenham.x + yInput->obscurity.x;
		newRay->obscurity = yInput->obscurity;
	}
}

bool DiamondRayCasting::isObscure(std::shared_ptr<RayData> ray)
{
	return ((ray->bresenham.x > 0 and ray->bresenham.x <= ray->obscurity.x) or
			(ray->bresenham.y > 0 and ray->bresenham.y <= ray->obscurity.y));
}
