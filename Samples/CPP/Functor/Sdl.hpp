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

		SDL(const std::string _name, std::reference_wrapper<Console> _console);

		void render(KeyCode key, const Doryen::Mouse& mouse) override;

	};

}

#endif //DORYEN_SDL_HPP
