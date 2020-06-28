// Joan Andrés (@Andres6936) Github.

#ifndef DORYEN_SDL_HPP
#define DORYEN_SDL_HPP

#include <Doryen/libtcod.hpp>

using namespace Doryen;

namespace Functor
{

	class SDL
	{

	private:

		Console sample;

	public:

		void operator()(KeyCode key, const Mouse& mouse);

	};

}

#endif //DORYEN_SDL_HPP
