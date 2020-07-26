#include <cmath>

#include "Doryen/FOV/DiamondRayCasting.hpp"

using namespace Doryen;

void DiamondRayCasting::operator()(
		Doryen::Map& map, int playerX, int playerY,
		int maxRadius, bool ligthWalls)
{
	const std::int32_t r2 = maxRadius * maxRadius;

	// Field of view origin
	origin = { playerX, playerY };

	std::vector<RayData> perim;

	perim.resize(map.getWidth() * map.getHeight());
	raymap.resize(map.getWidth() * map.getHeight());
	raymap2.resize(map.getWidth() * map.getHeight());

	expandPerimeterFrom(map, perim, newRay(map, { 0, 0 }));

	std::uint32_t perimidx = 0;

	while (perimidx < perim.size())
	{
		RayData ray = perim.at(perimidx);

		int distance = r2 > 0 ? std::pow(ray.position.x, 2) + std::pow(ray.position.y, 2) : 0;

		perimidx += 1;

		if (distance <= r2)
		{
			mergeInput(map, ray);

			if (not ray.ignore)
			{
				expandPerimeterFrom(map, perim, ray);
			}
		}
		else
		{
			ray.ignore = true;
		}
	}

	std::uint32_t nbCells = map.getWidth() * map.getHeight();

	auto c = map.getIteratorBegin();
	auto r = raymap.begin();

	while (nbCells not_eq 0)
	{
		if (r == raymap.end())
		{
			c->fov = false;
		}
		else if ((*r).ignore or
				 ((*r).bresenham.x > 0 and (*r).bresenham.x <= (*r).obscurity.x) or
				 ((*r).bresenham.y > 0 and (*r).bresenham.y <= (*r).obscurity.y))
		{
			c->fov = false;
		}
		else
		{
			c->fov = true;
		}

		// Advance to next element.
		std::advance(r, 1);
		std::advance(c, 1);

		nbCells -= 1;
	}

	map.setProperties(origin.x, origin.y, true, true);
}

void DiamondRayCasting::expandPerimeterFrom(
		Map& map, std::vector<RayData>& perim, std::optional<RayData> ray)
{
	if (not ray.has_value())
	{
		std::runtime_error("Pass of arguments with a parameter null.");
	}

	if (ray.value().position.x >= 0)
	{
		Geometry::Point2D<> position = ray.value().position;
		position.x += 1;

		processRay(map, perim, newRay(map, position), ray.value());
	}

	if (ray.value().position.x <= 0)
	{
		Geometry::Point2D<> position = ray.value().position;
		position.x -= 1;

		processRay(map, perim, newRay(map, position), ray.value());
	}

	if (ray.value().position.y >= 0)
	{
		Geometry::Point2D<> position = ray.value().position;
		position.y += 1;

		processRay(map, perim, newRay(map, position), ray.value());
	}

	if (ray.value().position.y <= 0)
	{
		Geometry::Point2D<> position = ray.value().position;
		position.y -= 1;

		processRay(map, perim, newRay(map, position), ray.value());
	}
}

void DiamondRayCasting::processRay(Map& _map, std::vector<RayData>& perim,
		std::optional<DiamondRayCasting::RayData> newRay, DiamondRayCasting::RayData& inputRay)
{
	if (newRay.has_value())
	{
		RayData ray = newRay.value();

		const std::uint32_t mapX = origin.x + ray.position.x;
		const std::uint32_t mapY = origin.y + ray.position.y;
		const std::uint32_t newRayIdx = mapX + mapY * _map.getWidth();

		if (ray.position.y == inputRay.position.y)
		{
			ray.xInput.reset(&inputRay);
		}
		else
		{
			ray.yInput.reset(&inputRay);
		}

		if (not ray.added)
		{
			ray.added = true;
			perim.push_back(ray);
			raymap.at(newRayIdx) = ray;
		}
	}
}

std::optional<DiamondRayCasting::RayData> DiamondRayCasting::newRay(
		Map& _map, const Geometry::Point2D<>& _coordinate)
{
	if (_coordinate.x + origin.x >= _map.getWidth() or
		_coordinate.y + origin.y >= _map.getHeight())
	{
		return std::nullopt;
	}

	// Else

	RayData it = raymap2.at(_coordinate.x + origin.x +
							(_coordinate.y + origin.y) * _map.getWidth());

	it.position = _coordinate;

	return it;
}

void DiamondRayCasting::mergeInput(Map& _map, DiamondRayCasting::RayData& _ray)
{
	if (_ray.xInput not_eq nullptr)
	{
		processXInput(_ray, _ray.xInput);
	}

	if (_ray.yInput not_eq nullptr)
	{
		processYInput(_ray, _ray.yInput);
	}

	if (_ray.xInput == nullptr)
	{
		if (isObscure(_ray.yInput)) _ray.ignore = true;
	}
	else if (_ray.yInput == nullptr)
	{
		if (isObscure(_ray.xInput)) _ray.ignore = true;
	}
	else if (isObscure(_ray.xInput) and isObscure(_ray.yInput))
	{
		_ray.ignore = true;
	}

	if (not _ray.ignore and not _map.isTransparent(
			_ray.position.x + origin.x, _ray.position.y + origin.y))
	{
		_ray.bresenham.x = _ray.obscurity.x = std::abs(_ray.position.x);
		_ray.bresenham.y = _ray.obscurity.y = std::abs(_ray.position.y);
	}
}

void DiamondRayCasting::processXInput(DiamondRayCasting::RayData& newRay,
		std::shared_ptr<RayData> xInput)
{
	if (xInput->obscurity.equals({ 0, 0 }))
	{
		return;
	}

	if (xInput->bresenham.x > 0 and newRay.obscurity.x == 0)
	{
		newRay.bresenham.x = xInput->bresenham.x - xInput->obscurity.y;
		newRay.bresenham.y = xInput->bresenham.y + xInput->obscurity.y;
		newRay.obscurity = xInput->obscurity;
	}

	if (xInput->bresenham.y <= 0 and newRay.obscurity.y > 0 and newRay.bresenham.x > 0)
	{
		newRay.bresenham.x = xInput->bresenham.x - xInput->obscurity.y;
		newRay.bresenham.y = xInput->bresenham.y + xInput->obscurity.y;
		newRay.obscurity = xInput->obscurity;
	}
}

void DiamondRayCasting::processYInput(DiamondRayCasting::RayData& newRay,
		std::shared_ptr<RayData> yInput)
{
	if (yInput->obscurity.equals({ 0, 0 }))
	{
		return;
	}

	if (yInput->bresenham.y > 0 and newRay.obscurity.y == 0)
	{
		newRay.bresenham.y = yInput->bresenham.y - yInput->obscurity.x;
		newRay.bresenham.x = yInput->bresenham.x + yInput->obscurity.x;
		newRay.obscurity = yInput->obscurity;
	}

	if (yInput->bresenham.x <= 0 and yInput->obscurity.x > 0 and yInput->bresenham.y > 0)
	{
		newRay.bresenham.y = yInput->bresenham.y - yInput->obscurity.x;
		newRay.bresenham.x = yInput->bresenham.x + yInput->obscurity.x;
		newRay.obscurity = yInput->obscurity;
	}
}

bool DiamondRayCasting::isObscure(std::shared_ptr<RayData> ray)
{
	return ((ray->bresenham.x > 0 and ray->bresenham.x <= ray->obscurity.x) or
			(ray->bresenham.y > 0 and ray->bresenham.y <= ray->obscurity.y));
}
