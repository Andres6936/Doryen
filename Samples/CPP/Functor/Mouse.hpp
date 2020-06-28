// Joan Andrés (@Andres6936) Github.

#ifndef DORYEN_MOUSE_HPP
#define DORYEN_MOUSE_HPP

#include <Doryen/libtcod.hpp>

#include "Interface/ISample.hpp"

using namespace Doryen;

namespace Functor
{

	class Mouse : public ISample
	{

	public:

		Mouse(const std::string _name, std::reference_wrapper<Console> _console);

		void render(KeyCode key, const Doryen::Mouse& mouse) override;

	};

}

#endif //DORYEN_MOUSE_HPP
