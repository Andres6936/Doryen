#include <cmath>

#include "Doryen/FOV/Permissive.hpp"

using namespace Doryen;

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
		LinkedListNode<Field> current = activeFields.begin();

		for (int j = 0; j <= i; ++j)
		{
			dest.x = static_cast<std::int16_t>(i - j);
			dest.y = static_cast<std::int16_t>(j);

			current = visitSquare(dest, current, steepBumps, shallowBumps, activeFields);
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

void Permissive::addSteepBump(Permissive::Offset& point,
		Permissive::LinkedListNode<Permissive::Field> currentField,
		std::vector<Bump>& steepBumps)
{

}

void Permissive::addShallowBump(Permissive::Offset& point,
		Permissive::LinkedListNode<Permissive::Field> currentField,
		std::vector<Bump>& shallowBumps)
{

}

Permissive::LinkedListNode<Permissive::Field>
Permissive::checkField(Permissive::LinkedListNode<Permissive::Field> currentField,
		Permissive::LinkedList<Permissive::Field>& activeFields)
{
	return Doryen::Permissive::LinkedListNode<Permissive::Field>();
}

Permissive::LinkedListNode<Permissive::Field>
Permissive::visitSquare(Permissive::Offset& dest,
		Permissive::LinkedListNode<Field> currentField,
		std::vector<Bump>& steepBumps, std::vector<Bump>& shallowBumps,
		Permissive::LinkedList<Field>& activeFields)
{
	Offset topLeft{ dest.x, static_cast<int16_t>(dest.y + 1) };
	Offset bottomRight{ static_cast<int16_t>(dest.x + 1), dest.y };

	while (currentField not_eq activeFields.end() and currentField->steep.isBelowOrContains(bottomRight))
	{
		// Advance to next element
		std::advance(currentField, 1);
	}

	if (currentField == activeFields.end() or currentField->shallow.isAboveOrContains(topLeft) or
		not actIsBlocked(dest))
	{
		return currentField;
	}

	if (currentField->shallow.isAbove(bottomRight) and currentField->steep.isBelow(topLeft))
	{
		LinkedListNode <Field> next = std::next(currentField, 1);
		activeFields.erase(currentField);
		return next;
	}
	else if (currentField->shallow.isAbove(bottomRight))
	{
		addShallowBump(topLeft, currentField, shallowBumps);
		return checkField(currentField, activeFields);
	}
	else if (currentField->steep.isBelow(topLeft))
	{
		addSteepBump(bottomRight, currentField, steepBumps);
		return checkField(currentField, activeFields);
	}
	else
	{
		LinkedListNode <Field> steeper = currentField;
	}
}
