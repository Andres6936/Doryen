// Joan Andrés (@Andres6936) Github.

#ifndef DORYEN_SDL_HPP
#define DORYEN_SDL_HPP

#include <Doryen/libtcod.hpp>

#include "Interface/ISample.hpp"

using namespace Doryen;

namespace Functor
{

	class SDL : public ISample
	{

	private:

		Console sample;

	public:

		SDL(const std::string _name, const Console& _console);

		void operator()(KeyCode key, const Mouse& mouse);

	};

}

#endif //DORYEN_SDL_HPP
