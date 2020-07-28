#include <cmath>

#include "Doryen/FOV/Permissive.hpp"

void
Doryen::Permissive::operator()(Doryen::Map& _map, int playerX, int playerY, int maxRadius, bool ligthWalls, int fovType)
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
	const std::int16_t INFINITY_16 = INT16_MAX;

	std::vector<Bump> steepBumps{};
	std::vector<Bump> shallowBumps{};

	LinkedList<Field> activeFields{};

	Field field{ .steep {{ 1, 0 },
						 { 0, INFINITY_16 }},
			.shallow{{ 0,           1 },
					 { INFINITY_16, 0 }}};

	activeFields.push_back(field);

	Offset dest{};

	actIsBlocked(dest);

	for (int i = 1; i < INFINITY_16 and activeFields.size() not_eq 0; ++i)
	{
		// Reference to current element
		Field& current = activeFields.front();

		for (int j = 0; j <= i; ++j)
		{
			dest.x = static_cast<std::int16_t>(i - j);
			dest.y = static_cast<std::int16_t>(j);
		}
	}
}

bool Doryen::Permissive::actIsBlocked(const Doryen::Permissive::Offset& pos)
{
	if (rangeLimit >= 0 and getDistance(std::max(pos.x, pos.y), std::min(pos.x, pos.y)) > rangeLimit)
	{
		return true;
	}
	else
	{
		const std::int32_t x = pos.x * quadrant.x + source.x;
		const std::int32_t y = pos.y * quadrant.y + source.y;

		map.setVisibleFieldView(x, y);

		return not map.isTransparent(x, y);
	}
}

std::int32_t Doryen::Permissive::getDistance(std::int32_t a, std::int32_t b)
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
