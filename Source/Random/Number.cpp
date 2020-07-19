// Joan Andrés (@Andres6936) Github.

#include "Doryen/Random/Number.hpp"

using namespace Doryen;

std::int32_t Random::Number::nextInteger(std::int32_t from, std::int32_t to) const
{
	std::uniform_int_distribution<std::int32_t> distribution{ from, to };
	return distribution(randomEngine);
}
