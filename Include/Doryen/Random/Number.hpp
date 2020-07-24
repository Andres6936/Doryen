// Joan Andrés (@Andres6936) Github.

#ifndef DORYEN_NUMBER_HPP
#define DORYEN_NUMBER_HPP

#include <random>
#include <cstdint>

namespace Doryen::Random
{

	/**
	 * Generate random number with behavior determinist
	 * (for purpose of debug), if you not wanna the
	 * determinist behavior, call to function setRandomSeed
	 * before of generate a new number random.
	 */
	class Number
	{

	private:

		// Without random device, I wanna a determinist behavior
		// If you wanna an un-determinist behavior, called to
		// setRandomSeed function.
		inline static std::mt19937 randomEngine;

	public:

		static void setRandomSeed();

		/**
		 * Generate a random number between the range [from, to].
		 *
		 * @param from Minimum value to generate (Inclusive)
		 * @param to Maximum value to generate (Inclusive)
		 * @return Value between the range [from, to].
		 */
		static std::int32_t nextInteger(std::int32_t from, std::int32_t to);

		/**
		 * Generate a random number between the range [from, to].
		 *
		 * @param from Minimum value to generate (Inclusive)
		 * @param to Maximum value to generate (Inclusive)
		 * @return Value between the range [from, to].
		 */
		static float nextFloat(float from, float to);

	};
}

#endif //DORYEN_NUMBER_HPP
