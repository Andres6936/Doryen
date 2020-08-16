// Joan Andrés (@Andres6936) Github.

#ifndef DORYEN_FRACTIONALBROWNIANMOTION_HPP
#define DORYEN_FRACTIONALBROWNIANMOTION_HPP

#include <array>

namespace Doryen::Algorithms
{

	/**
	 * Fractional Brownian Motion
	 */
	template<int Dimension>
	class FBM
	{

	public:

		float noise(const std::array<float, Dimension>& input, float octaves);

	};

}

#endif //DORYEN_FRACTIONALBROWNIANMOTION_HPP
