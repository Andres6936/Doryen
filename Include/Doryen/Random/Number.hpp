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

		std::random_device randomDevice;

		std::mt19937 randomEngine;

	public:

		Number() : randomEngine(randomDevice())
		{
		};

		std::int32_t nextInteger(std::int32_t from, std::int32_t to) const;

	};
}

#endif //DORYEN_NUMBER_HPP
