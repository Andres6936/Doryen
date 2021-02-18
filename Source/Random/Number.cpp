// Joan Andrés (@Andres6936) Github.

#include "Doryen/Random/Number.hpp"

using namespace Doryen;

std::int32_t Random::Number::nextInteger(std::int32_t from, std::int32_t to)
{
	std::uniform_int_distribution<std::int32_t> distribution{ from, to };
	return distribution(randomEngine);
}

float Random::Number::nextFloat(float from, float to)
{
	std::uniform_real_distribution<float> distribution{ from, to };
	return distribution(randomEngine);
}

void Random::Number::setRandomSeed()
{
	// The class std::random_device this is a special class which produces
	// uniformly-distributed unsigned integers with 32 bits of length. It can
	// produce random numbers either by accessing the operational system's
	// entropy pool via system calls or by using hardware random number
	// generators such as Intel's RdRand when available (but not all
	// implementations of std::random_device allow this). Developers must be
	// warned, however, that even though 32-bit unsigned integers are generated,
	// the entropy of this random number generator may be lower than 32 bits.

	// Reference: https://diego.assencio.com/?index=6890b8c50169ef45b74db135063c227c

	// Rewrite the random engine with a new, but a difference of
	// original, this new random engine have a random device
	// that generate pseudo-random numbers
	randomEngine = std::mt19937{ std::random_device{}() };
}
