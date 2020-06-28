// Joan Andrés (@Andres6936) Github.

#ifndef DORYEN_OFFSCREEN_HPP
#define DORYEN_OFFSCREEN_HPP

#include <Doryen/libtcod.hpp>

using namespace Doryen;

namespace Functor
{
	class Offscreen
	{

	private:

		Console sample;

	public:

		void operator()(KeyCode key, const Mouse& mouse);

	};
}

#endif //DORYEN_OFFSCREEN_HPP
