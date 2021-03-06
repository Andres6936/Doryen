// Joan Andrés (@Andres6936) Github.

#ifndef DORYEN_OFFSCREEN_HPP
#define DORYEN_OFFSCREEN_HPP

#include <Doryen/Doryen.hpp>

#include "Interface/ISample.hpp"

using namespace Doryen;

namespace Functor
{
	class Offscreen : public ISample
	{

	public:

		Offscreen(Console& _console);

		void render(KeyCode key, const Doryen::Mouse& mouse) override;

	};
}

#endif //DORYEN_OFFSCREEN_HPP
