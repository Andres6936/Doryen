// Joan Andrés (@Andres6936) Github.

#ifndef DORYEN_NUMBER_HPP
#define DORYEN_NUMBER_HPP

#include <random>
#include <cstdint>

namespace Doryen::Random
{

	class Number
	{

	private:

		// Without random device, I wanna a determinist behavior
		// If you wanna an un-determinist behavior, called to
		// setRandomSeed function.
		static std::mt19937 randomEngine;

	public:

		static void setRandomSeed();

		static std::int32_t nextInteger(std::int32_t from, std::int32_t to);

	};
}

#endif //DORYEN_NUMBER_HPP
