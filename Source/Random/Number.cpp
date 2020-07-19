// Joan Andrés (@Andres6936) Github.

#include "Doryen/Random/Number.hpp"

using namespace Doryen;

std::int32_t Random::Number::nextInteger(std::int32_t from, std::int32_t to)
{
	std::uniform_int_distribution<std::int32_t> distribution{ from, to };
	return distribution(randomEngine);
}

void Random::Number::setRandomSeed()
{
	// Rewrite the random engine with a new, but a difference of
	// original, this new random engine have a random device
	// that generate pseudo-random numbers
	randomEngine = std::mt19937{ std::random_device{}() };
}
