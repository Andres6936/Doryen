// Joan Andrés (@Andres6936) Github.

#ifndef DORYEN_NOISE_HPP
#define DORYEN_NOISE_HPP

#include <Doryen/libtcod.hpp>

using namespace Doryen;

namespace Functor
{
	class Noise
	{

	private:

		Console sample;

	public:

		void operator()(KeyCode key, const Mouse& mouse);

	};
}

#endif //DORYEN_NOISE_HPP
